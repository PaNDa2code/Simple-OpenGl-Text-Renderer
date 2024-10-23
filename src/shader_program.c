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

GLuint compile_shader(const char *source, GLenum shader_type,
                      const char *shader_name) {
  GLuint shader = glCreateShader(shader_type);
  glShaderSource(shader, 1, &source, NULL);
  glCompileShader(shader);

  // Check for compilation errors
  GLint success;
  glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
  if (!success) {
    char infoLog[512];
    glGetShaderInfoLog(shader, 512, NULL, infoLog);
    fprintf(stderr, "Error compiling %s shader:\n%s\n", shader_name, infoLog);
    return 0;
  }

  return shader;
}

GLuint TextShaderProgram() {
  GLint success = 0;

  GLuint vertex_shader =
      compile_shader(VERTEX_SHADER_SOURCE, GL_VERTEX_SHADER, "vertex_shader");

  if (!vertex_shader)
    return 0;

  GLuint fragment_shader = compile_shader(
      FRAGMENT_SHADER_SOURCE, GL_FRAGMENT_SHADER, "fragment_shader");

  if (!fragment_shader){
    glDeleteShader(vertex_shader);
    return 0;
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
  }

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
