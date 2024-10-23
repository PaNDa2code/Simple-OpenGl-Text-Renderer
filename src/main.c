#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include <glad/glad.h>

#include <ft2build.h>
#include FT_FREETYPE_H

#include <GL/glu.h>

#include <GLFW/glfw3.h>

#include "characters.h"
#include "fps_calc.h"
#include "orthographic_projection.h"
#include "render_text.h"
#include "shader_program.h"

Character_t characters[128];
unsigned int VAO, VBO;
GLfloat ortho_matrix[16];

#define WINDOW_WIDTH 800
#define WINDOW_HIGHT 450

void framebuffer_size_callback(GLFWwindow *window, int width, int height);

int main() {
  int exit_code = -1;
  const char *font_path =
      "/usr/share/fonts/truetype/firacode/FiraCode-Bold.ttf";
  FT_Library lib;
  FT_Face face;
  GLFWwindow *window;
  GLenum err;
  GLuint text_shader_program;
  GLuint bloom_shader_program;

  if (glfwInit() != GL_TRUE) {
    fprintf(stderr, "Couldn\'t initalize glfw\n");
    return -1;
  }

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HIGHT, "Panda Text Render",
                            NULL, NULL);

  if (window == NULL) {
    fprintf(stderr, "Couldn\'t create glfw window\n");
    goto CLEANUP;
  }

  glfwMakeContextCurrent(window);
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
  gladLoadGL();

  if (FT_Init_FreeType(&lib)) {
    fprintf(stderr, "Couldn\'t initalize FreeFont Library\n");
    goto CLEANUP;
  }

  if (FT_New_Face(lib, font_path, 0, &face)) {
    fprintf(stderr, "Couldn\'t initalize FreeFont Face\n");
    FT_Done_FreeType(lib);
    goto CLEANUP;
  }

  FT_Set_Pixel_Sizes(face, 0, 48);
  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

  for (GLubyte c = 0; c < 128; c++) {
    if (FT_Load_Char(face, c, FT_LOAD_RENDER)) {
      fprintf(stderr, "Faild to load glyph\n");
      continue;
    }
    uint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, face->glyph->bitmap.width,
                 face->glyph->bitmap.rows, 0, GL_RED, GL_UNSIGNED_BYTE,
                 face->glyph->bitmap.buffer);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    Character_t character = {
        texture,
        {face->glyph->bitmap.width, face->glyph->bitmap.rows},
        {face->glyph->bitmap_left, face->glyph->bitmap_top},
        face->glyph->advance.x};

    characters[c] = character;
  }

  glBindTexture(GL_TEXTURE_2D, 0);
  FT_Done_Face(face);
  FT_Done_FreeType(lib);

  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glDisable(GL_DEPTH_TEST);

  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);
  glBindVertexArray(VAO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);

  // V-Sync
  glfwSwapInterval(1);

  glViewport(0, 0, WINDOW_WIDTH, WINDOW_HIGHT);

  text_shader_program = TextShaderProgram();

  if (!text_shader_program || !bloom_shader_program) {
    goto CLEANUP;
  }

  glUseProgram(text_shader_program);

  ortho(0.0f, WINDOW_WIDTH, 0.0f, WINDOW_HIGHT, -1.0f, 1.0f, ortho_matrix);

  glUniformMatrix4fv(glGetUniformLocation(text_shader_program, "projection"), 1,
                     GL_FALSE, (const GLfloat *)ortho_matrix);

  char info_log[512];
  GLsizei info_log_size;

  glGetProgramInfoLog(text_shader_program, sizeof(info_log), &info_log_size,
                      info_log);

  if (info_log_size > 0) {
    fprintf(stderr, "Shader Program Info Log:\n%s\n", info_log);
    goto CLEANUP;
  }

  glClearColor(0.08, 0.08, 0.08, 1);

  InitFpsCalc();

  while (!glfwWindowShouldClose(window)) {

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    GLfloat color[3] = {1, 1, 1};
    RenderFps(text_shader_program);
    render_text(text_shader_program, "ABCDEFGHIKJLMNOPQRSTUVWXYZ", 25, 400, 0.9,
                color);
    render_text(text_shader_program, "abcdefghikjlmnopqrstuvwxyz", 25, 350, 0.9,
                color);
    render_text(text_shader_program, "1234567890!@#$%^&*(){}[]+=", 25, 300, 0.9, color);
    render_text(text_shader_program, "_-|\\\"\'?><.,", 25, 250, 0.9, color);

    render_text(text_shader_program, "I\'m BATMAN", 250, 200, 0.9, color);

    glfwSwapBuffers(window);
    glfwPollEvents();

    while ((err = glGetError()) != GL_NO_ERROR) {
      const GLubyte *err_str = gluErrorString(err);
      fprintf(stderr, "OpenGl Error: %s\n", err_str);
    }
  }

  exit_code = 0;

CLEANUP:

  if (VAO)
    glDeleteVertexArrays(1, &VAO);

  if (VBO)
    glDeleteBuffers(1, &VBO);

  if (text_shader_program)
    glDeleteProgram(text_shader_program);

  for (GLuint c = 0; c < 128; c++) {
    if (characters[c].TextureID) {
      glDeleteTextures(1, &characters[c].TextureID);
    }
  }

  if (window != NULL)
    glfwDestroyWindow(window);

  glfwTerminate();

  return exit_code;
}

void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
  // make sure the viewport matches the new window dimensions; note that width
  // and height will be significantly larger than specified on retina displays.
  ortho(0.0f, width, 0.0f, height, -1.0f, 1.0f, ortho_matrix);
  glViewport(0, 0, width, height);
}
