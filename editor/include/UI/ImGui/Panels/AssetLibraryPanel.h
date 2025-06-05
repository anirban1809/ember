#ifndef __FILEBROWSERPANEL_H__
#define __FILEBROWSERPANEL_H__

#include "../ImGuiPanel.h"
#include "UI/ApplicationState.h"
#include "UI/SceneManager.h"

class GeneralDetailsPanel : public ImGuiPanel {
   public:
    GeneralDetailsPanel(ApplicationState&, SceneManager&);
    void Render();

   private:
    ApplicationState& state;
    SceneManager& m_SceneManager;
    std::string current;
    std::vector<std::string> assets;
};

#endif  // __FILEBROWSERPANEL_H__
