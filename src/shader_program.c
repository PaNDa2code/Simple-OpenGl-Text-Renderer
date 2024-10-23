#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include <glad/glad.h>

#include <ft2build.h>
#include FT_FREETYPE_H

#include <GLFW/glfw3.h>

#include "orthographic_projection.h"

#ifndef VERTEX_SHADER_SOURCE
#define VERTEX_SHADER_SOURCE ""
#endif

#ifndef FRAGMENT_SHADER_SOURCE
#define FRAGMENT_SHADER_SOURCE ""
#endif

GLuint createShaderProgram() {
  GLint success = 0;

  const GLchar *vertex_shader_source = VERTEX_SHADER_SOURCE;
  GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertex_shader, 1, &vertex_shader_source, NULL);
  glCompileShader(vertex_shader);

  glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &success);

  if (!success) {
    char infoLog[512];
    glGetShaderInfoLog(vertex_shader, 512, NULL, infoLog);
    fprintf(stderr, "Error in Vertix shader compiling\n%s\n", infoLog);
    return 0;
  }

  const char *fragment_shader_source = FRAGMENT_SHADER_SOURCE;
  GLuint fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragment_shader, 1, &fragment_shader_source, NULL);
  glCompileShader(fragment_shader);

  glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &success);
  if (!success) {
    char infoLog[512];
    glGetShaderInfoLog(fragment_shader, 512, NULL, infoLog);
    fprintf(stderr, "Error in Framgment shader compiling\n%s\n", infoLog);
    goto EXIT;
  }

  GLuint shader_program = glCreateProgram();
  glAttachShader(shader_program, vertex_shader);
  glAttachShader(shader_program, fragment_shader);
  glLinkProgram(shader_program);
  glGetProgramiv(shader_program, GL_LINK_STATUS, &success);
  if (!success) {
    char infoLog[512];
    glGetProgramInfoLog(shader_program, 512, NULL, infoLog);
    fprintf(stderr, "Error while linking shader program\n%s\n", infoLog);
    goto EXIT;
  }

EXIT:

  if (vertex_shader)
    glDeleteShader(vertex_shader);

  if (fragment_shader)
    glDeleteShader(fragment_shader);

  if (!success && shader_program) {
    glDeleteProgram(shader_program);
    shader_program = 0;
  }

  return shader_program;
}
