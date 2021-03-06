#include "event/event.h"
#include "thread/thread.h"
#include "core/arr.h"
#include "core/os.h"
#include "core/ref.h"
#include "core/util.h"
#include "core/utf.h"
#include <stdlib.h>
#include <string.h>

static struct {
  bool initialized;
  arr_t(Event) events;
  size_t head;
} state;

static double last_mouse_x = 0;
static double last_mouse_y = 0;

static void onKeyboardEvent(ButtonAction action, KeyboardKey key, uint32_t scancode, bool repeat) {
  lovrEventPush((Event) {
    .type = action == BUTTON_PRESSED ? EVENT_KEYPRESSED : EVENT_KEYRELEASED,
    .data.key.code = key,
    .data.key.scancode = scancode,
    .data.key.repeat = repeat
  });
}

static void onMouseButtonEvent(double x, double y, MouseButton button, ButtonAction action) {
  lovrEventPush((Event) {
    .type = action == BUTTON_PRESSED ? EVENT_MOUSEPRESSED : EVENT_MOUSERELEASED,
    .data.mouseButton.button = button + 1,
    .data.mouseButton.x = x,
    .data.mouseButton.y = y
  });
}

static void onMouseMoveEvent(double x, double y) {
  double dx = x - last_mouse_x;
  double dy = y - last_mouse_y;
  last_mouse_x = x;
  last_mouse_y = y;

  lovrEventPush((Event) {
    .type = EVENT_MOUSEMOVED,
    .data.mouseMove.dx = dx,
    .data.mouseMove.dy = dy,
    .data.mouseMove.x = x,
    .data.mouseMove.y = y
  });
}

static void onMouseScrollEvent(double dx, double dy) {
  lovrEventPush((Event) {
    .type = EVENT_MOUSESCROLLED,
    .data.mouseScroll.dx = dx,
    .data.mouseScroll.dy = dy
  });
}

static void onTextEvent(uint32_t codepoint) {
  Event event;
  event.type = EVENT_TEXTINPUT;
  event.data.text.codepoint = codepoint;
  memset(&event.data.text.utf8, 0, sizeof(event.data.text.utf8));
  utf8_encode(codepoint, event.data.text.utf8);
  lovrEventPush(event);
}

void lovrVariantDestroy(Variant* variant) {
  switch (variant->type) {
    case TYPE_STRING: free(variant->value.string); return;
    case TYPE_OBJECT: _lovrRelease(variant->value.object.pointer, variant->value.object.destructor); return;
    default: return;
  }
}

bool lovrEventInit() {
  if (state.initialized) return false;
  arr_init(&state.events);
  lovrPlatformOnMouseButtonEvent(onMouseButtonEvent);
  lovrPlatformOnMouseMoveEvent(onMouseMoveEvent);
  lovrPlatformOnMouseScrollEvent(onMouseScrollEvent);
  lovrPlatformOnKeyboardEvent(onKeyboardEvent);
  lovrPlatformOnTextEvent(onTextEvent);
  return state.initialized = true;
}

void lovrEventDestroy() {
  if (!state.initialized) return;
  for (size_t i = state.head; i < state.events.length; i++) {
    Event* event = &state.events.data[i];
    switch (event->type) {
#if LOVR_ENABLE_THREAD
      case EVENT_THREAD_ERROR: lovrRelease(Thread, event->data.thread.thread); break;
#endif
      case EVENT_CUSTOM:
        for (uint32_t j = 0; j < event->data.custom.count; j++) {
          lovrVariantDestroy(&event->data.custom.data[j]);
        }
        break;
      default: break;
    }
  }
  arr_free(&state.events);
  lovrPlatformOnMouseButtonEvent(NULL);
  lovrPlatformOnMouseMoveEvent(NULL);
  lovrPlatformOnMouseScrollEvent(NULL);
  lovrPlatformOnKeyboardEvent(NULL);
  lovrPlatformOnTextEvent(NULL);
  memset(&state, 0, sizeof(state));
}

void lovrEventPump() {
  lovrPlatformPollEvents();
}

void lovrEventPush(Event event) {
#ifdef LOVR_ENABLE_THREAD
  if (event.type == EVENT_THREAD_ERROR) {
    lovrRetain(event.data.thread.thread);
  }
#endif

  arr_push(&state.events, event);
}

bool lovrEventPoll(Event* event) {
  if (state.head == state.events.length) {
    state.head = state.events.length = 0;
    return false;
  }

  *event = state.events.data[state.head++];
  return true;
}

void lovrEventClear() {
  arr_clear(&state.events);
  state.head = 0;
}
