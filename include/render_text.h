#pragma once
#include <GL/gl.h>

void render_text(GLuint shaderProgram, const char *text, float x, float y,
                 float scale, float color[3]);
