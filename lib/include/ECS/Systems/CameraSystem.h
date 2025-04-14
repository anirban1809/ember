#ifndef __CAMERASYSTEM_H__
#define __CAMERASYSTEM_H__

#include "ECS/System.h"
class CameraSystem : public System {
   public:
    void Init(Scene&);
    void UpdateView(Scene&);
};
#endif  // __CAMERASYSTEM_H__