#ifndef __FILEBROWSERPANEL_H__
#define __FILEBROWSERPANEL_H__

#include "../ImGuiPanel.h"
#include "UI/ApplicationState.h"

class GeneralDetailsPanel : public ImGuiPanel {
   public:
    GeneralDetailsPanel(ApplicationState&);
    void Render();

   private:
    ApplicationState& state;
    std::string current;
    std::vector<std::string> assets;
};

#endif  // __FILEBROWSERPANEL_H__
