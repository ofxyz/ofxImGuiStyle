#pragma once

#include "ofMain.h"
#include "ofxImGui.h"
#include "ImTheme.h"
#include "ImThemeRegistry.h"
#include "ImFonts.h"

class ofApp : public ofBaseApp {
public:
    void setup();
    void update();
    void draw();

private:
    void drawDockspace();
    void buildDefaultDockLayout(ImGuiID dockspaceId);
    void drawMenuBar();
    void drawToolbar();
    void drawScenePanel();
    void drawPropertiesPanel();
    void drawAssetBrowser();
    void drawConsole();
    void drawViewport();
    void drawStatusBar();

    ofxImGui::Gui         gui;
    ImTheme::TweakedTheme tweaks;  // for the embedded ShowThemeTweakGui

    bool showThemeEditor = false;
    bool showAbout = false;
    bool showConsole = true;
    bool showAssets = true;
    bool showProperties = true;
    bool showScene = true;
    bool showToolbar = true;
    bool defaultDockLayoutBuilt = false;

    // Imaginary app state
    int selectedNode = 0;
    float nodePosition[3] = {120.0f, 45.0f, 0.0f};
    float nodeRotation[3] = {0.0f, 0.0f, 0.0f};
    float nodeScale[3] = {1.0f, 1.0f, 1.0f};
    float nodeColor[4] = {0.33f, 0.55f, 0.85f, 1.0f};
    float nodeOpacity = 1.0f;
    bool nodeVisible = true;
    bool nodeLocked = false;
    int blendMode = 0;
    int renderPass = 0;
    float exposure = 1.0f;
    float gamma = 2.2f;
    bool autoSave = true;
    float masterVolume = 0.75f;
    int currentTool = 0;
    char searchBuf[128] = "";
    float progress = 0.0f;

    std::vector<std::string> consoleLog;
};
