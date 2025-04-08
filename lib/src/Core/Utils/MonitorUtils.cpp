#include "../../../include/Core/Utils/MonitorUtils.h"
#include <iostream>

GLFWmonitor* MonitorUtils::GetPrimaryMonitor() {
    GLFWmonitor* primary = glfwGetPrimaryMonitor();
    if (!primary) {
        std::cerr << "MonitorUtils: Failed to get primary monitor!"
                  << std::endl;
    }
    return primary;
}

MonitorWorkArea MonitorUtils::GetPrimaryMonitorWorkArea() {
    MonitorWorkArea area;

    GLFWmonitor* monitor = GetPrimaryMonitor();
    if (monitor) {
        glfwGetMonitorWorkarea(monitor, &area.x, &area.y, &area.width,
                               &area.height);
    }

    return area;
}
