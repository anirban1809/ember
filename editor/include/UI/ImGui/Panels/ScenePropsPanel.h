#ifndef __SCENEPROPSPANEL_H__
#define __SCENEPROPSPANEL_H__

#include "../ImGuiPanel.h"
#include "../../ApplicationState.h"
#include "UI/SceneManager.h"
#include <memory>

class ScenePropsPanel : public ImGuiPanel {
   public:
    ScenePropsPanel(ApplicationState&, SceneManager&);
    void Render();

   private:
    ApplicationState& m_AppState;
    SceneManager& m_SceneManager;
};

#endif  // __SCENEPROPSPANEL_H__