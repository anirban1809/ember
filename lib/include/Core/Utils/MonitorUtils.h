#ifndef __MONITORUTILS_H__
#define __MONITORUTILS_H__

#include <GLFW/glfw3.h>

struct MonitorWorkArea {
    int x = 0;
    int y = 0;
    int width = 0;
    int height = 0;
};

class MonitorUtils {
   public:
    // Returns the primary monitor, or nullptr if unavailable
    static GLFWmonitor* GetPrimaryMonitor();

    // Returns the work area of the primary monitor
    static MonitorWorkArea GetPrimaryMonitorWorkArea();
};

#endif  // __MONITORUTILS_H__