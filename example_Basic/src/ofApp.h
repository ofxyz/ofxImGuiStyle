#pragma once

#include "ofMain.h"
#include "ofxImGui.h"
#include "ImTheme.h"
#include "ImThemeRegistry.h"
#include "ImFonts.h"
#include "ImKnobs.h"

class ofApp : public ofBaseApp {
public:
    void setup();
    void draw();

private:
    void drawDockspace();
    void buildDefaultDockLayout(ImGuiID dockspaceId);
    void drawControls();
    void drawPreview();
    void drawStyleEditor();

    ofxImGui::Gui          gui;
    std::string            currentThemeId = "ImGuiColorsDark";
    ImTheme::TweakedTheme  tweaks;
    float uiScale = 1.0f;
    bool  defaultDockLayoutBuilt = false;
    bool  showStyleEditor = true;
    bool  showDemoWindow  = false;
    bool  enabled = true;
    int   choice = 1;
    float amount = 0.65f;
    float knobCutoff = 0.42f;
    float knobReso   = 0.18f;
    int   knobSteps  = 4;
    float color[4] = {0.33f, 0.55f, 0.85f, 1.0f};
};
