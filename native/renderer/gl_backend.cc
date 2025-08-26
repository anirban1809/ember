#include <string>
#ifndef GL_SILENCE_DEPRECATION
#define GL_SILENCE_DEPRECATION 1
#endif

#define GLEW_NO_GLU

#include "../../vendor/glew-2.2.0/include/GL/glew.h"
// Optional: you may link GLFW but we DO NOT create windows/contexts here.
// #include <GLFW/glfw3.h>

#include <cstdio>
#include "gl_backend.h"

namespace {
bool inited = false;
GLuint prog = 0, vao = 0, vbo = 0;

const char *kVS = R"(
    #version 150
    in vec2 aPos;
    void main(){ 
        gl_Position = vec4(aPos, 0.0, 1.0); 
    }
)";
const char *kFS = R"(#version 150
    out vec4 oColor;
    void main(){ oColor = vec4(1.0, 0.0, 1.0, 1.0); })";

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
        std::fprintf(stderr, "[gl] shader error:\n%s\n", log);
    }
    return s;
}

void makePipeline() {
    GLuint vs = compile(GL_VERTEX_SHADER, kVS);
    GLuint fs = compile(GL_FRAGMENT_SHADER, kFS);
    prog = glCreateProgram();
    glAttachShader(prog, vs);
    glAttachShader(prog, fs);
    glLinkProgram(prog);
    glDeleteShader(vs);
    glDeleteShader(fs);

    const float tri[] = {-0.8f, -0.7f, 0.8f, -0.7f, 0.0f, 0.75f};
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(tri), tri, GL_STATIC_DRAW);
    GLint loc = glGetAttribLocation(prog, "aPos");
    glEnableVertexAttribArray(loc);
    glVertexAttribPointer(loc, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float),
                          (void *)0);
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}
}  // namespace

namespace glbackend {

void ensureInit() {
    if (inited) return;

    // If you really want GLFW for helper APIs, you can initialize it here:
    // glfwInit();

    glewExperimental = GL_TRUE;
    GLenum err = glewInit();
    if (err != GLEW_OK) {
        std::fprintf(stderr, "[gl] glewInit failed: %s\n",
                     (const char *)glewGetErrorString(err));
    }
    // clear GLEW's spurious GL error on mac core profile
    glGetError();

    makePipeline();
    inited = true;
}

void draw(int width, int height) {
    ensureInit();

    glViewport(0, 0, width, height);
    glDisable(GL_DEPTH_TEST);
    glClearColor(0.f, 0.f, 0.f, 1.f);
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(prog);
    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    glBindVertexArray(0);
    glUseProgram(0);
}

void shutdown() {
    // glfwTerminate(); // if you called glfwInit()
    if (vbo) {
        glDeleteBuffers(1, &vbo);
        vbo = 0;
    }
    if (vao) {
        glDeleteVertexArrays(1, &vao);
        vao = 0;
    }
    if (prog) {
        glDeleteProgram(prog);
        prog = 0;
    }
    inited = false;
}

}  // namespace glbackend
