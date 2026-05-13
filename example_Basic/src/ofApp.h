#pragma once

#include "ofMain.h"
#include "ofxImGui.h"
#include "ofxImGuiStyle.h"

class ofApp : public ofBaseApp {
public:
    void setup();
    void draw();

private:
    void applyTheme();
    void drawDockspace();
    void buildDefaultDockLayout(ImGuiID dockspaceId);
    void drawControls();
    void drawPreview();
    void drawStyleEditor();

    ofxImGui::Gui gui;
    ofxImGuiStyle style;

    enum class Theme {
        Dark,
        Light,
        Classic,
        RandomAccent,
    };

    Theme currentTheme = Theme::Dark;
    float uiScale = 1.0f;
    bool defaultDockLayoutBuilt = false;
    bool showStyleEditor = true;
    bool showDemoWindow = false;
    bool enabled = true;
    int choice = 1;
    float amount = 0.65f;
    float color[4] = {0.33f, 0.55f, 0.85f, 1.0f};
};
