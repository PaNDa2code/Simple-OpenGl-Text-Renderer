#pragma once

#include <GL/gl.h>

void ortho(GLfloat left, GLfloat right, GLfloat bottom, GLfloat top,
           GLfloat near, GLfloat far, GLfloat matrix[16]);
