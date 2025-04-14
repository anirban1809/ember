#ifndef __LIGHTSYSTEM_H__
#define __LIGHTSYSTEM_H__

#include "ECS/System.h"
class LightSystem : public System {
   public:
    void Update(Scene&);
};
#endif  // __LIGHTSYSTEM_H__