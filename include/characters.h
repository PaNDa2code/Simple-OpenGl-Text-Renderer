#pragma once
#include <stdlib.h>
#include <GL/gl.h>

typedef struct {
  GLuint TextureID;
  GLint size[2];
  GLint Bearing[2];
  GLuint Advance;
} Character_t;

