#include "../../include/Core/RenderAPI.h"

GraphicsAPI RenderAPI::current = GraphicsAPI::OpenGL;  // default

void RenderAPI::Set(GraphicsAPI api) { current = api; }

GraphicsAPI RenderAPI::Get() { return current; }