
#include <glad/glad.h>

#include <GL/gl.h>

#include <GLFW/glfw3.h>

#include <stdint.h>
#include <math.h>

#include <render_text.h>
#include <stdio.h>

uint64_t frequancy = 0;
uint64_t timeTik = 0;
uint64_t timeTok = 0;

const int TARGET_FPS = 60;
uint64_t TargetTicksPerFrame = 0;

uint32_t frameCounter = 0;

uint64_t ticks = 0;

double fps = 0.0;

char buffer[50] = {};
float color[3] = {1, 1, 0};

void InitFpsCalc() {
  frequancy = glfwGetTimerFrequency();
  timeTik = glfwGetTimerValue();
  TargetTicksPerFrame = frequancy / TARGET_FPS;
};

void FpsCalc() {
  frameCounter++;
  timeTok = glfwGetTimerValue();
  ticks = timeTok - timeTik;
  if (ticks >= frequancy) {
    fps = (double)frameCounter * frequancy / ticks;
    frameCounter = 0;
    timeTik = timeTok;
    snprintf(buffer, sizeof(buffer), "FPS: %d", (int)ceil(fps));
  }
}

void ShowFps(GLuint shader_program) {
  FpsCalc();
  render_text(shader_program, buffer, 700, 430, 1, color);
}
