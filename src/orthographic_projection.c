#include <GL/gl.h>

void ortho(GLfloat left, GLfloat right, GLfloat bottom, GLfloat top,
           GLfloat near, GLfloat far, GLfloat matrix[16]) {
  matrix[0] = 2.0f / (right - left); // Column 0, Row 0
  matrix[1] = 0.0f;                  // Column 0, Row 1
  matrix[2] = 0.0f;                  // Column 0, Row 2
  matrix[3] = 0.0f;                  // Column 0, Row 3

  matrix[4] = 0.0f;                  // Column 1, Row 0
  matrix[5] = 2.0f / (top - bottom); // Column 1, Row 1
  matrix[6] = 0.0f;                  // Column 1, Row 2
  matrix[7] = 0.0f;                  // Column 1, Row 3

  matrix[8] = 0.0f;   // Column 2, Row 0
  matrix[9] = 0.0f;   // Column 2, Row 1
  matrix[10] = -1.0f; // Column 2, Row 2
  matrix[11] = 0.0f;  // Column 2, Row 3

  matrix[12] = -(right + left) / (right - left); // Column 3, Row 0
  matrix[13] = -(top + bottom) / (top - bottom); // Column 3, Row 1
  matrix[14] = 0.0f;                             // Column 3, Row 2
  matrix[15] = 1.0f;                             // Column 3, Row 3
}
