#pragma once
#include "../../node_modules/node-addon-api/napi.h"
#include <string>
namespace glbackend {

Napi::Value LogMessage(Napi::Env env);

// Called first time a current GL context is available
void ensureInit();

// Draw one frame into the current context
void draw(int width, int height);

// Cleanup (optional)
void shutdown();

}  // namespace glbackend
