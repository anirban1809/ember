#include <cstring>
#include <iostream>
#include "../node_modules/node-addon-api/napi.h"
#include "platform/mac/platform_mac.h"

namespace {

static void *PtrFromBuffer(const Napi::Buffer<uint8_t> &buf) {
    uintptr_t p = 0;
    const size_t n = std::min(buf.Length(), sizeof(uintptr_t));
    std::memcpy(&p, buf.Data(), n);
    return reinterpret_cast<void *>(p);
}

Napi::Value Attach(const Napi::CallbackInfo &info) {
    Napi::Env env = info.Env();
    if (info.Length() < 5 || !info[0].IsBuffer()) return env.Null();

    auto buf = info[0].As<Napi::Buffer<uint8_t>>();
    void *nsWindowPtr = PtrFromBuffer(buf);  // <-- read pointer value

    const int x = info[1].As<Napi::Number>().Int32Value();
    const int y = info[2].As<Napi::Number>().Int32Value();
    const int w = info[3].As<Napi::Number>().Int32Value();
    const int h = info[4].As<Napi::Number>().Int32Value();

    Mac_AttachGLViewToWindow(nsWindowPtr, x, y, w, h);  // <-- pass window ptr
    return env.Null();
}

Napi::Value Resize(const Napi::CallbackInfo &info) {
    std::cout << "resizing..." << std::endl;
    Napi::Env env = info.Env();
    const int x = info[0].As<Napi::Number>().Int32Value();
    const int y = info[1].As<Napi::Number>().Int32Value();
    const int w = info[2].As<Napi::Number>().Int32Value();
    const int h = info[3].As<Napi::Number>().Int32Value();
    Mac_ResizeGLView(x, y, w, h);
    return env.Null();
}

Napi::Value SetIOSurfaceId(const Napi::CallbackInfo &info) {
    Napi::Env env = info.Env();
    const uint32_t id = info[0].As<Napi::Number>().Uint32Value();
    Mac_SetIOSurfaceId(id);
    return env.Null();
}

Napi::Value Destroy(const Napi::CallbackInfo &info) {
    Mac_DestroyGLView();
    return info.Env().Null();
}

Napi::Value Log(const Napi::CallbackInfo &info) {
    Napi::Env env = info.Env();
    std::string value = info[0].As<Napi::String>();
    std::cout << "Logged: " << value << std::endl;
    return Napi::Boolean::New(env, true);
}

Napi::Object Init(Napi::Env env, Napi::Object exports) {
    exports.Set("attachGLView", Napi::Function::New(env, Attach));
    exports.Set("resizeGLView", Napi::Function::New(env, Resize));
    exports.Set("setIOSurfaceId", Napi::Function::New(env, SetIOSurfaceId));
    exports.Set("destroyGLView", Napi::Function::New(env, Destroy));
    exports.Set("logMessage", Napi::Function::New(env, Log));
    return exports;
}

}  // namespace

NODE_API_MODULE(oglview, Init)
