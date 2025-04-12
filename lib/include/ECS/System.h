#ifndef __SYSTEM_H__
#define __SYSTEM_H__

#include "ECS/Scene.h"
#include <string>
class System {
   public:
    virtual ~System() = default;

    virtual void Init(Scene&) = 0;
    virtual void Update(Scene&) = 0;
    virtual std::string GetName() const = 0;
};
#endif  // __SYSTEM_H__