#pragma once

#ifndef GL_SILENCE_DEPRECATION
#define GL_SILENCE_DEPRECATION 1
#endif

#include <OpenGL/gl3.h>

namespace tri {
// Lazily creates the GL program + VAO/VBO (safe to call per-frame).
void ensureInit();

// Draw a single blue triangle covering the center of the screen.
// Assumes viewport is already set by caller.
void drawBlueTriangle();

// Cleanup (optional).
void destroy();
} // namespace tri
