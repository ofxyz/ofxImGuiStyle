#include "ofApp.h"
#include "IconsFontAwesome5.h"
#include "imgui_internal.h"

void ofApp::setup() {
    ofSetWindowTitle("Luma Studio");

    ImGuiConfigFlags flags = ImGuiConfigFlags_DockingEnable;
#ifndef TARGET_OPENGLES
    flags |= ImGuiConfigFlags_ViewportsEnable;
#endif
    gui.setup(nullptr, true, flags, true);

    style.loadFonts(gui, 15.0f);
    ofxImGuiStyle::applyDarkTheme();

    // Luma Studio is meant to feel like a polished desktop app, so give the
    // shared theme a little more breathing room than the compact defaults.
    ImGuiStyle& imguiStyle = ImGui::GetStyle();
    imguiStyle.WindowPadding    = ImVec2(14, 12);
    imguiStyle.FramePadding     = ImVec2(10, 6);
    imguiStyle.ItemSpacing      = ImVec2(10, 8);
    imguiStyle.ItemInnerSpacing = ImVec2(8, 6);
    imguiStyle.CellPadding      = ImVec2(8, 5);
    imguiStyle.IndentSpacing    = 22.0f;
    imguiStyle.ScrollbarSize    = 10.0f;

    style.captureBaseStyle();

    consoleLog.push_back("[info]  Luma Studio initialized");
    consoleLog.push_back("[info]  Loaded project: Untitled.luma");
    consoleLog.push_back("[info]  GPU: OpenGL " + ofToString(glGetString(GL_VERSION)));
    consoleLog.push_back("[info]  Renderer ready");
}

void ofApp::update() {
    progress += 0.002f;
    if (progress > 1.0f) progress = 0.0f;
}

void ofApp::draw() {
    ofBackground(30, 30, 34);

    gui.begin();

    drawMenuBar();
    drawStatusBar();
    drawDockspace();
    if (showToolbar) drawToolbar();

    if (showScene)      drawScenePanel();
    if (showProperties) drawPropertiesPanel();
    if (showAssets)     drawAssetBrowser();
    if (showConsole)    drawConsole();

    drawViewport();
    drawStatusBar();

    if (showThemeEditor) {
        style.draw(&showThemeEditor);
    }

    if (showAbout) {
        ImGui::SetNextWindowSize(ImVec2(360, 200), ImGuiCond_FirstUseEver);
        if (ImGui::Begin(ICON_FA_INFO_CIRCLE " About Luma Studio", &showAbout)) {
            ImGui::TextColored(ImVec4(0.33f, 0.55f, 0.85f, 1.0f), ICON_FA_STAR " Luma Studio v1.0");
            ImGui::Spacing();
            ImGui::TextWrapped("A fictional creative production app showcasing ofxImGuiStyle theming, Input Sans font, and Font Awesome 5 icons.");
            ImGui::Spacing();
            ImGui::Separator();
            ImGui::TextDisabled("Built with openFrameworks + ofxImGui + ofxImGuiStyle");
        }
        ImGui::End();
    }

    gui.end();
}

void ofApp::drawDockspace() {
    ImGuiViewport* viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(viewport->WorkPos);
    ImGui::SetNextWindowSize(viewport->WorkSize);
    ImGui::SetNextWindowViewport(viewport->ID);

    ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoDocking
        | ImGuiWindowFlags_NoTitleBar
        | ImGuiWindowFlags_NoCollapse
        | ImGuiWindowFlags_NoResize
        | ImGuiWindowFlags_NoMove
        | ImGuiWindowFlags_NoBringToFrontOnFocus
        | ImGuiWindowFlags_NoNavFocus
        | ImGuiWindowFlags_NoBackground;

    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));

    ImGui::Begin("DockSpace###luma.dockspace", nullptr, windowFlags);
    ImGui::PopStyleVar(3);

    ImGuiID dockspaceId = ImGui::GetID("luma.dockspace");
    ImGui::DockSpace(dockspaceId, ImVec2(0, 0), ImGuiDockNodeFlags_PassthruCentralNode);

    if (!defaultDockLayoutBuilt) {
        buildDefaultDockLayout(dockspaceId);
        defaultDockLayoutBuilt = true;
    }

    ImGui::End();
}

void ofApp::buildDefaultDockLayout(ImGuiID dockspaceId) {
    ImGui::DockBuilderRemoveNode(dockspaceId);
    ImGui::DockBuilderAddNode(dockspaceId,
        ImGuiDockNodeFlags_DockSpace | ImGuiDockNodeFlags_PassthruCentralNode);
    ImGui::DockBuilderSetNodeSize(dockspaceId, ImGui::GetMainViewport()->WorkSize);

    ImGuiID left = 0;
    ImGuiID right = 0;
    ImGuiID bottom = 0;
    ImGuiID center = dockspaceId;

    ImGui::DockBuilderSplitNode(center, ImGuiDir_Left, 0.22f, &left, &center);
    ImGui::DockBuilderSplitNode(center, ImGuiDir_Right, 0.28f, &right, &center);
    ImGui::DockBuilderSplitNode(center, ImGuiDir_Down, 0.28f, &bottom, &center);

    ImGui::DockBuilderDockWindow(ICON_FA_SITEMAP " Scene", left);
    ImGui::DockBuilderDockWindow(ICON_FA_SLIDERS_H " Properties", right);
    ImGui::DockBuilderDockWindow(ICON_FA_IMAGES " Assets", bottom);
    ImGui::DockBuilderDockWindow(ICON_FA_TERMINAL " Console", bottom);
    ImGui::DockBuilderDockWindow(ICON_FA_TV " Viewport", center);
    ImGui::DockBuilderDockWindow(ICON_FA_PALETTE " Theme Editor", right);

    ImGui::DockBuilderFinish(dockspaceId);
}

// ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
// Menu Bar
// ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

void ofApp::drawMenuBar() {
    if (ImGui::BeginMainMenuBar()) {
        if (ImGui::BeginMenu(ICON_FA_FILE " File")) {
            if (ImGui::MenuItem(ICON_FA_FILE_ALT " New Project", "Ctrl+N")) {}
            if (ImGui::MenuItem(ICON_FA_FOLDER_OPEN " Open...", "Ctrl+O")) {}
            ImGui::Separator();
            if (ImGui::MenuItem(ICON_FA_SAVE " Save", "Ctrl+S")) {}
            if (ImGui::MenuItem(ICON_FA_FILE_EXPORT " Export...", "Ctrl+E")) {}
            ImGui::Separator();
            if (ImGui::MenuItem(ICON_FA_DOOR_OPEN " Quit", "Alt+F4")) { ofExit(); }
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu(ICON_FA_EYE " View")) {
            ImGui::MenuItem(ICON_FA_GRIP_VERTICAL " Toolbar", nullptr, &showToolbar);
            ImGui::MenuItem(ICON_FA_SITEMAP " Scene Hierarchy", nullptr, &showScene);
            ImGui::MenuItem(ICON_FA_SLIDERS_H " Properties", nullptr, &showProperties);
            ImGui::MenuItem(ICON_FA_IMAGES " Assets", nullptr, &showAssets);
            ImGui::MenuItem(ICON_FA_TERMINAL " Console", nullptr, &showConsole);
            ImGui::Separator();
            ImGui::MenuItem(ICON_FA_PALETTE " Theme Editor", nullptr, &showThemeEditor);
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu(ICON_FA_TOOLS " Tools")) {
            if (ImGui::MenuItem(ICON_FA_UNDO " Undo", "Ctrl+Z")) {}
            if (ImGui::MenuItem(ICON_FA_REDO " Redo", "Ctrl+Y")) {}
            ImGui::Separator();
            if (ImGui::MenuItem(ICON_FA_COG " Preferences...")) {}
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu(ICON_FA_QUESTION_CIRCLE " Help")) {
            if (ImGui::MenuItem(ICON_FA_BOOK " Documentation")) {}
            ImGui::Separator();
            if (ImGui::MenuItem(ICON_FA_INFO_CIRCLE " About")) { showAbout = true; }
            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();
    }
}

// ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
// Toolbar (left strip)
// ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

void ofApp::drawToolbar() {
    ImGui::SetNextWindowPos(ImVec2(18, 58), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowSize(ImVec2(0, 0), ImGuiCond_Always);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(6, 8));
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(3, 3));

    ImGuiWindowFlags flags = ImGuiWindowFlags_NoTitleBar
        | ImGuiWindowFlags_AlwaysAutoResize
        | ImGuiWindowFlags_NoScrollbar
        | ImGuiWindowFlags_NoCollapse
        | ImGuiWindowFlags_NoDocking;

    if (ImGui::Begin("Toolbar###luma.toolbar", &showToolbar, flags)) {
        ImGui::TextDisabled(ICON_FA_GRIP_VERTICAL);
        if (ImGui::IsItemHovered())
            ImGui::SetTooltip("Drag to move toolbar");
        if (ImGui::IsItemActive() && ImGui::IsMouseDragging(ImGuiMouseButton_Left)) {
            ImVec2 d = ImGui::GetIO().MouseDelta;
            ImVec2 p = ImGui::GetWindowPos();
            ImGui::SetWindowPos(ImVec2(p.x + d.x, p.y + d.y));
        }

        ImGui::SameLine();
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.7f, 0.2f, 0.2f, 0.7f));
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.9f, 0.1f, 0.1f, 1.0f));
        if (ImGui::SmallButton(ICON_FA_TIMES))
            showToolbar = false;
        if (ImGui::IsItemHovered())
            ImGui::SetTooltip("Close Toolbar");
        ImGui::PopStyleColor(3);

        ImGui::Separator();

        struct Tool { const char* icon; const char* tip; };
        Tool tools[] = {
            { ICON_FA_MOUSE_POINTER, "Select (V)" },
            { ICON_FA_ARROWS_ALT,    "Move (G)" },
            { ICON_FA_SYNC_ALT,      "Rotate (R)" },
            { ICON_FA_EXPAND_ALT,    "Scale (S)" },
            { ICON_FA_VECTOR_SQUARE, "Rectangle (U)" },
            { ICON_FA_CIRCLE,        "Ellipse (O)" },
            { ICON_FA_PEN,           "Pen (P)" },
            { ICON_FA_FILL_DRIP,     "Paint Bucket (B)" },
            { ICON_FA_FONT,          "Text (T)" },
            { ICON_FA_CROP_ALT,      "Crop (C)" },
        };
        int numTools = sizeof(tools) / sizeof(tools[0]);

        for (int i = 0; i < numTools; i++) {
            ImGui::PushID(i);
            bool selected = (currentTool == i);
            if (selected) {
                ImGui::PushStyleColor(ImGuiCol_Button, ImGui::GetStyleColorVec4(ImGuiCol_ButtonActive));
            }
            if (ImGui::Button(tools[i].icon, ImVec2(34, 34))) {
                currentTool = i;
            }
            if (ImGui::IsItemHovered()) {
                ImGui::SetTooltip("%s", tools[i].tip);
            }
            if (selected) {
                ImGui::PopStyleColor();
            }
            ImGui::PopID();
        }
    }
    ImGui::End();
    ImGui::PopStyleVar(2);
}

// ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
// Scene Hierarchy
// ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

void ofApp::drawScenePanel() {
    if (ImGui::Begin(ICON_FA_SITEMAP " Scene")) {
        ImGui::SetNextItemWidth(-1);
        ImGui::InputTextWithHint("##search", ICON_FA_SEARCH " Search nodes...", searchBuf, sizeof(searchBuf));
        ImGui::Spacing();

        struct SceneNode { const char* icon; const char* name; int depth; };
        SceneNode nodes[] = {
            { ICON_FA_GLOBE_AMERICAS, "World",           0 },
            { ICON_FA_LIGHTBULB,      "Main Light",      1 },
            { ICON_FA_VIDEO,          "Camera",          1 },
            { ICON_FA_CUBE,           "Hero Cube",       1 },
            { ICON_FA_VECTOR_SQUARE,  "Background Plane",1 },
            { ICON_FA_CIRCLE,         "Particle System", 1 },
            { ICON_FA_CIRCLE,         "  Emitter A",     2 },
            { ICON_FA_CIRCLE,         "  Emitter B",     2 },
            { ICON_FA_FONT,           "Title Text",      1 },
            { ICON_FA_MUSIC,          "Audio Reactive",  1 },
        };

        for (int i = 0; i < 10; i++) {
            ImGui::PushID(i);
            bool sel = (selectedNode == i);
            std::string label = std::string(nodes[i].icon) + " " + nodes[i].name;

            if (nodes[i].depth > 1) {
                ImGui::Indent(16.0f);
            }

            if (ImGui::Selectable(label.c_str(), sel)) {
                selectedNode = i;
            }

            if (nodes[i].depth > 1) {
                ImGui::Unindent(16.0f);
            }

            ImGui::PopID();
        }
    }
    ImGui::End();
}

// ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
// Properties Panel
// ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

void ofApp::drawPropertiesPanel() {
    if (ImGui::Begin(ICON_FA_SLIDERS_H " Properties")) {
        // Transform section
        if (ImGui::CollapsingHeader(ICON_FA_ARROWS_ALT " Transform", ImGuiTreeNodeFlags_DefaultOpen)) {
            ImGui::DragFloat3("Position", nodePosition, 0.5f);
            ImGui::DragFloat3("Rotation", nodeRotation, 0.5f, -360, 360);
            ImGui::DragFloat3("Scale", nodeScale, 0.01f, 0.01f, 100.0f);
        }

        // Appearance section
        if (ImGui::CollapsingHeader(ICON_FA_PAINT_BRUSH " Appearance", ImGuiTreeNodeFlags_DefaultOpen)) {
            ImGui::ColorEdit4(ICON_FA_PALETTE " Color", nodeColor);
            ImGui::SliderFloat(ICON_FA_ADJUST " Opacity", &nodeOpacity, 0.0f, 1.0f);

            const char* blendModes[] = { "Normal", "Additive", "Multiply", "Screen" };
            ImGui::Combo(ICON_FA_LAYER_GROUP " Blend", &blendMode, blendModes, 4);
        }

        // Rendering section
        if (ImGui::CollapsingHeader(ICON_FA_IMAGE " Rendering")) {
            const char* passes[] = { "Forward", "Deferred", "Post-Process" };
            ImGui::Combo("Render Pass", &renderPass, passes, 3);
            ImGui::SliderFloat("Exposure", &exposure, 0.0f, 5.0f);
            ImGui::SliderFloat("Gamma", &gamma, 0.5f, 4.0f);
        }

        // Flags section
        if (ImGui::CollapsingHeader(ICON_FA_FLAG " Flags")) {
            ImGui::Checkbox(ICON_FA_EYE " Visible", &nodeVisible);
            ImGui::Checkbox(ICON_FA_LOCK " Locked", &nodeLocked);
            ImGui::Checkbox(ICON_FA_SAVE " Auto-save", &autoSave);
        }

        ImGui::Separator();
        ImGui::Spacing();

        float w = ImGui::GetContentRegionAvail().x;
        if (ImGui::Button(ICON_FA_COPY " Duplicate", ImVec2(w * 0.48f, 0))) {
            consoleLog.push_back("[info]  Node duplicated");
        }
        ImGui::SameLine();
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.6f, 0.15f, 0.15f, 1.0f));
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.7f, 0.2f, 0.2f, 1.0f));
        if (ImGui::Button(ICON_FA_TRASH_ALT " Delete", ImVec2(w * 0.48f, 0))) {
            consoleLog.push_back("[warn]  Node deleted");
        }
        ImGui::PopStyleColor(2);
    }
    ImGui::End();
}

// ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
// Asset Browser
// ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

void ofApp::drawAssetBrowser() {
    if (ImGui::Begin(ICON_FA_IMAGES " Assets")) {
        // Category tabs
        if (ImGui::BeginTabBar("##assetTabs")) {
            if (ImGui::BeginTabItem(ICON_FA_IMAGE " Textures")) {
                struct Asset { const char* icon; const char* name; const char* size; };
                Asset items[] = {
                    { ICON_FA_IMAGE, "diffuse_map.png",   "2048x2048" },
                    { ICON_FA_IMAGE, "normal_map.png",    "2048x2048" },
                    { ICON_FA_IMAGE, "roughness.png",     "1024x1024" },
                    { ICON_FA_IMAGE, "environment.hdr",   "4096x2048" },
                    { ICON_FA_IMAGE, "noise_perlin.png",  "512x512" },
                    { ICON_FA_IMAGE, "gradient_ramp.png", "256x1" },
                };
                for (auto& a : items) {
                    ImGui::Text("%s  %s", a.icon, a.name);
                    ImGui::SameLine(320);
                    ImGui::TextDisabled("%s", a.size);
                }
                ImGui::EndTabItem();
            }
            if (ImGui::BeginTabItem(ICON_FA_CUBE " Models")) {
                ImGui::Text(ICON_FA_CUBE "  hero_character.obj");
                ImGui::Text(ICON_FA_CUBE "  terrain_chunk.fbx");
                ImGui::Text(ICON_FA_CUBE "  props_set.gltf");
                ImGui::EndTabItem();
            }
            if (ImGui::BeginTabItem(ICON_FA_MUSIC " Audio")) {
                ImGui::Text(ICON_FA_MUSIC "  ambient_loop.wav");
                ImGui::Text(ICON_FA_MUSIC "  impact_sfx.ogg");
                ImGui::Text(ICON_FA_MUSIC "  soundtrack.mp3");
                ImGui::Spacing();
                ImGui::SliderFloat(ICON_FA_VOLUME_UP " Master Volume", &masterVolume, 0.0f, 1.0f);
                ImGui::EndTabItem();
            }
            if (ImGui::BeginTabItem(ICON_FA_CODE " Shaders")) {
                ImGui::Text(ICON_FA_CODE "  pbr_lighting.frag");
                ImGui::Text(ICON_FA_CODE "  post_bloom.frag");
                ImGui::Text(ICON_FA_CODE "  particle.vert");
                ImGui::EndTabItem();
            }
            ImGui::EndTabBar();
        }
    }
    ImGui::End();
}

// ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
// Console
// ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

void ofApp::drawConsole() {
    if (ImGui::Begin(ICON_FA_TERMINAL " Console")) {
        if (ImGui::Button(ICON_FA_ERASER " Clear")) { consoleLog.clear(); }
        ImGui::SameLine();
        ImGui::TextDisabled("%d messages", (int)consoleLog.size());
        ImGui::Separator();

        ImGui::BeginChild("##log", ImVec2(0, 0), false, ImGuiWindowFlags_HorizontalScrollbar);
        for (auto& line : consoleLog) {
            ImVec4 color(0.9f, 0.9f, 0.92f, 1.0f);
            if (line.find("[warn]") != std::string::npos)
                color = ImVec4(1.0f, 0.85f, 0.3f, 1.0f);
            else if (line.find("[error]") != std::string::npos)
                color = ImVec4(1.0f, 0.35f, 0.35f, 1.0f);
            else if (line.find("[info]") != std::string::npos)
                color = ImVec4(0.5f, 0.8f, 0.5f, 1.0f);

            ImGui::TextColored(color, "%s", line.c_str());
        }
        if (ImGui::GetScrollY() >= ImGui::GetScrollMaxY())
            ImGui::SetScrollHereY(1.0f);
        ImGui::EndChild();
    }
    ImGui::End();
}

// ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
// Viewport (center area, just decorative)
// ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

void ofApp::drawViewport() {
    ImGuiWindowFlags flags = ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse;
    if (ImGui::Begin(ICON_FA_TV " Viewport", nullptr, flags)) {
        ImVec2 avail = ImGui::GetContentRegionAvail();
        ImVec2 cursor = ImGui::GetCursorScreenPos();

        ImDrawList* dl = ImGui::GetWindowDrawList();
        dl->AddRectFilled(cursor, ImVec2(cursor.x + avail.x, cursor.y + avail.y),
            IM_COL32(20, 20, 25, 255));

        // Fake grid
        ImU32 gridCol = IM_COL32(50, 50, 60, 100);
        float step = 40.0f;
        for (float x = cursor.x; x < cursor.x + avail.x; x += step)
            dl->AddLine(ImVec2(x, cursor.y), ImVec2(x, cursor.y + avail.y), gridCol);
        for (float y = cursor.y; y < cursor.y + avail.y; y += step)
            dl->AddLine(ImVec2(cursor.x, y), ImVec2(cursor.x + avail.x, y), gridCol);

        // Fake object
        float cx = cursor.x + avail.x * 0.5f;
        float cy = cursor.y + avail.y * 0.5f;
        float t = ofGetElapsedTimef();
        float r = 60.0f + 10.0f * sinf(t);
        ImU32 objCol = ImGui::ColorConvertFloat4ToU32(ImVec4(nodeColor[0], nodeColor[1], nodeColor[2], nodeColor[3]));
        dl->AddCircleFilled(ImVec2(cx, cy), r, objCol, 48);
        dl->AddCircle(ImVec2(cx, cy), r + 3, IM_COL32(255, 255, 255, 60), 48, 1.5f);

        // Crosshair
        dl->AddLine(ImVec2(cx - 10, cy), ImVec2(cx + 10, cy), IM_COL32(255, 255, 255, 40));
        dl->AddLine(ImVec2(cx, cy - 10), ImVec2(cx, cy + 10), IM_COL32(255, 255, 255, 40));

        // FPS overlay
        char fps[32];
        snprintf(fps, sizeof(fps), "%.0f fps", ofGetFrameRate());
        dl->AddText(ImVec2(cursor.x + avail.x - 60, cursor.y + 6), IM_COL32(120, 120, 130, 200), fps);
    }
    ImGui::End();
}

// ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
// Status Bar
// ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

void ofApp::drawStatusBar() {
    ImGuiWindowFlags flags = ImGuiWindowFlags_NoScrollbar
                           | ImGuiWindowFlags_NoSavedSettings
                           | ImGuiWindowFlags_MenuBar;
    const float h = ImGui::GetFrameHeight();

    if (ImGui::BeginViewportSideBar("##StatusBar", nullptr, ImGuiDir_Down, h, flags)) {
        if (ImGui::BeginMenuBar()) {
            ImGui::Text(ICON_FA_CHECK_CIRCLE " Ready");
            ImGui::SameLine(200);
            ImGui::TextDisabled(ICON_FA_CUBE " Objects: 10");
            ImGui::SameLine(380);
            ImGui::TextDisabled(ICON_FA_MEMORY " VRAM: 128 MB");
            ImGui::SameLine(560);
            ImGui::ProgressBar(progress, ImVec2(120, 0), "");
            ImGui::SameLine();
            ImGui::TextDisabled("Baking...");
            ImGui::SameLine(ImGui::GetWindowWidth() - 90.0f);
            ImGui::TextDisabled("%.0f fps", ofGetFrameRate());
            ImGui::EndMenuBar();
        }
    }
    ImGui::End();
}
