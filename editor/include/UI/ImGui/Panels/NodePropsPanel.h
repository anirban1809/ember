#ifndef __NODEPROPSPANEL_H__
#define __NODEPROPSPANEL_H__

#include "../ImGuiPanel.h"
#include "../../ApplicationState.h"
#include "UI/SceneManager.h"

class NodePropsPanel : public ImGuiPanel {
   public:
    NodePropsPanel(ApplicationState&, SceneManager&);
    void Render();

   private:
    ApplicationState& state;
    SceneManager& m_SceneManager;
};
#endif  // __NODEPROPSPANEL_H__