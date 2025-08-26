#pragma once

namespace glbackend {

// Called first time a current GL context is available
void ensureInit();

// Draw one frame into the current context
void draw(int width, int height);

// Cleanup (optional)
void shutdown();

}  // namespace glbackend
