#include <stdbool.h>
#include <stdint.h>

#pragma once

#define MAX_EVENT_NAME_LENGTH 32

struct Thread;

typedef enum {
  EVENT_QUIT,
  EVENT_RESTART,
  EVENT_FOCUS,
  EVENT_RESIZE,
  EVENT_MOUSEMOVED,
  EVENT_MOUSEPRESSED,
  EVENT_MOUSERELEASED,
  EVENT_MOUSESCROLLED,
  EVENT_KEYPRESSED,
  EVENT_KEYRELEASED,
  EVENT_TEXTINPUT,
  EVENT_CUSTOM,
#ifdef LOVR_ENABLE_THREAD
  EVENT_THREAD_ERROR,
#endif
} EventType;

typedef enum {
  TYPE_NIL,
  TYPE_BOOLEAN,
  TYPE_NUMBER,
  TYPE_STRING,
  TYPE_OBJECT
} VariantType;

typedef union {
  bool boolean;
  double number;
  char* string;
  struct {
    void* pointer;
    const char* type;
    void (*destructor)(void*);
  } object;
} VariantValue;

typedef struct Variant {
  VariantType type;
  VariantValue value;
} Variant;

typedef struct {
  int exitCode;
} QuitEvent;

typedef struct {
  bool value;
} BoolEvent;

typedef struct {
  uint32_t width;
  uint32_t height;
} ResizeEvent;

typedef struct {
  uint32_t code;
  uint32_t scancode;
  bool repeat;
} KeyEvent;

typedef struct {
  double x;
  double y;
  int button;
} MouseButtonEvent;

typedef struct {
  double x;
  double y;
  double dx;
  double dy;
} MouseMoveEvent;

typedef struct {
  double dx;
  double dy;
} MouseScrollEvent;

typedef struct {
  char utf8[4];
  uint32_t codepoint;
} TextEvent;

typedef struct {
  struct Thread* thread;
  char* error;
} ThreadEvent;

typedef struct {
  char name[MAX_EVENT_NAME_LENGTH];
  Variant data[4];
  uint32_t count;
} CustomEvent;

typedef union {
  QuitEvent quit;
  BoolEvent boolean;
  ResizeEvent resize;
  KeyEvent key;
  MouseButtonEvent mouseButton;
  MouseMoveEvent mouseMove;
  MouseScrollEvent mouseScroll;
  TextEvent text;
  ThreadEvent thread;
  CustomEvent custom;
} EventData;

typedef struct {
  EventType type;
  EventData data;
} Event;

void lovrVariantDestroy(Variant* variant);

bool lovrEventInit(void);
void lovrEventDestroy(void);
void lovrEventPump(void);
void lovrEventPush(Event event);
bool lovrEventPoll(Event* event);
void lovrEventClear(void);
