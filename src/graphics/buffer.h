#include "../glfw.h"

#ifndef LOVR_BUFFER_TYPES
#define LOVR_BUFFER_TYPES
typedef enum {
  BUFFER_POINTS = GL_POINTS,
  BUFFER_TRIANGLE_STRIP = GL_TRIANGLE_STRIP,
  BUFFER_TRIANGLES = GL_TRIANGLES,
  BUFFER_TRIANGLE_FAN = GL_TRIANGLE_FAN
} BufferDrawMode;

typedef enum {
  BUFFER_STATIC = GL_STATIC_DRAW,
  BUFFER_DYNAMIC = GL_DYNAMIC_DRAW,
  BUFFER_STREAM = GL_STREAM_DRAW
} BufferUsage;

typedef struct {
  int size;
  GLfloat* data;
  BufferDrawMode drawMode;
  BufferUsage usage;
  GLuint vao;
  GLuint vbo;
  int rangeStart;
  int rangeCount;
} Buffer;
#endif

void lovrBufferDestroy(Buffer* buffer);
void lovrBufferDraw(Buffer* buffer);
BufferDrawMode lovrBufferGetDrawMode(Buffer* buffer);
int lovrBufferSetDrawMode(Buffer* buffer, BufferDrawMode drawMode);
void lovrBufferGetVertex(Buffer* buffer, int index, float* x, float* y, float* z);
void lovrBufferSetVertex(Buffer* buffer, int index, float x, float y, float z);
void lovrBufferGetDrawRange(Buffer* buffer, int* start, int* count);
int lovrBufferSetDrawRange(Buffer* buffer, int start, int count);
