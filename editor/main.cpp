#include "Editor.h"
#include "EmberEditor.h"

int main() {
    // Editor *app = new Editor(2560, 1330, "Editor");
    // app->Run();

    EmberEditor *app = new EmberEditor(2560, 1330, "Ember :: 0.0.1");
    app->Run();
    return 0;
}
