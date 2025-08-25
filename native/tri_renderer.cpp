#ifndef GL_SILENCE_DEPRECATION
#define GL_SILENCE_DEPRECATION 1
#endif

#include "tri_renderer.h"
#include <cstdio>

namespace {
GLuint gProg = 0, gVAO = 0, gVBO = 0;

const char *kVS = R"(#version 150
  in vec2 aPos;
  void main(){
    gl_Position = vec4(aPos, 0.0, 1.0);
  })";

const char *kFS = R"(#version 150
  out vec4 oColor;
  void main(){
    oColor = vec4(0.20, 1.0, 1.0, 1.0);
  })";

GLuint compile(GLenum type, const char *src) {
  GLuint s = glCreateShader(type);
  glShaderSource(s, 1, &src, nullptr);
  glCompileShader(s);
  GLint ok = 0;
  glGetShaderiv(s, GL_COMPILE_STATUS, &ok);
  if (!ok) {
    GLchar log[2048];
    GLsizei n = 0;
    glGetShaderInfoLog(s, 2048, &n, log);
    std::fprintf(stderr, "[tri] shader error:\n%s\n", log);
  }
  return s;
}

void ensureProgram() {
  if (gProg)
    return;
  GLuint vs = compile(GL_VERTEX_SHADER, kVS);
  GLuint fs = compile(GL_FRAGMENT_SHADER, kFS);
  gProg = glCreateProgram();
  glAttachShader(gProg, vs);
  glAttachShader(gProg, fs);
  glLinkProgram(gProg);
  glDeleteShader(vs);
  glDeleteShader(fs);

  // Fullscreen-ish single triangle (NDC)
  const float tri[] = {-0.8f, -0.7f, 0.8f, -0.7f, 0.0f, 0.75f};

  glGenVertexArrays(1, &gVAO);
  glBindVertexArray(gVAO);
  glGenBuffers(1, &gVBO);
  glBindBuffer(GL_ARRAY_BUFFER, gVBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(tri), tri, GL_STATIC_DRAW);

  GLint loc = glGetAttribLocation(gProg, "aPos");
  glEnableVertexAttribArray(loc);
  glVertexAttribPointer(loc, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float),
                        (void *)0);

  glBindVertexArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
}
} // namespace

namespace tri {
void ensureInit() { ensureProgram(); }

void drawBlueTriangle() {
  ensureProgram();
  glUseProgram(gProg);
  glBindVertexArray(gVAO);
  glDrawArrays(GL_TRIANGLES, 0, 3);
  glBindVertexArray(0);
  glUseProgram(0);
}

void destroy() {
  if (gVBO) {
    glDeleteBuffers(1, &gVBO);
    gVBO = 0;
  }
  if (gVAO) {
    glDeleteVertexArrays(1, &gVAO);
    gVAO = 0;
  }
  if (gProg) {
    glDeleteProgram(gProg);
    gProg = 0;
  }
}
} // namespace tri
