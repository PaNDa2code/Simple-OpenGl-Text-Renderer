#include <stdint.h>

#include <glad/glad.h>

#include <ft2build.h>
#include FT_FREETYPE_H

#include <GLFW/glfw3.h>

#include "characters.h"
#include "orthographic_projection.h"

extern Character_t characters[128];
extern unsigned int VAO, VBO;

void render_text(GLuint shader_program, const char *text, float x, float y,
                 float scale, float color[3]) {
  glUseProgram(shader_program);
  glUniform3f(glGetUniformLocation(shader_program, "textColor"), color[0],
              color[1], color[2]);
  glActiveTexture(GL_TEXTURE0);
  glBindVertexArray(VAO);

  for (const char *p = text; *p != 0; p++) {
    Character_t ch = characters[*p];
    float xpos = x + ch.bearing.x * scale;
    float ypos = y - (ch.size.y - ch.bearing.y) * scale;
    float w = ch.size.x * scale;
    float h = ch.size.y * scale;
    float vertices[6][4] = {
        {xpos, ypos + h, 0.0f, 0.0f}, {xpos, ypos, 0.0f, 1.0f},
        {xpos + w, ypos, 1.0f, 1.0f}, {xpos, ypos + h, 0.0f, 0.0f},
        {xpos + w, ypos, 1.0f, 1.0f}, {xpos + w, ypos + h, 1.0f, 0.0f},
    };

    glBindTexture(GL_TEXTURE_2D, ch.TextureID);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    x += (ch.Advance >> 6) * scale;
  }
  glBindVertexArray(0);
  glBindTexture(GL_TEXTURE_2D, 0);

  char info_log[512];
  GLsizei info_log_size;

  glGetProgramInfoLog(shader_program, sizeof(info_log), &info_log_size,
                      info_log);

  if (info_log_size > 0) {
    fprintf(stderr, "Shader Program Info Log:\n%s\n", info_log);
  }
}
