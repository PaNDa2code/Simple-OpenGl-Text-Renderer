#pragma once
#include <GL/gl.h>
#include <stdlib.h>

typedef struct {
  GLuint TextureID;
  struct {
    GLint x;
    GLint y;
  } size;
  struct {
    GLint x;
    GLint y;
  } bearing;
  GLuint Advance;
} Character_t;
