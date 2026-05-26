#include "ofApp.h"
#include "IconsFontAwesome5.h"
#include "imgui_internal.h"

void ofApp::setup() {
    ofSetWindowTitle("ofxImGuiStyle Basic Example");

    ImGuiConfigFlags flags = ImGuiConfigFlags_DockingEnable;
#ifndef TARGET_OPENGLES
    flags |= ImGuiConfigFlags_ViewportsEnable;
#endif
    gui.setup(nullptr, false, flags, true);

    if (ImFont* f = ImFonts::LoadDefaultFonts(ImGui::GetIO().Fonts, 15.0f))
        gui.setDefaultFont(f);
    gui.rebuildFontsTexture();

    ImTheme::Setup(currentThemeId);
    uiScale = ImTheme::UIScale();
}

void ofApp::draw() {
    ofBackground(24, 24, 28);

    gui.begin();

    drawDockspace();
    drawControls();
    drawPreview();
    if (showStyleEditor) drawStyleEditor();
    if (showDemoWindow) ImGui::ShowDemoWindow(&showDemoWindow);

    gui.end();
    gui.draw();
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

    ImGui::Begin("DockSpace###ofxImGuiStyleBasicDockspace", nullptr, windowFlags);
    ImGui::PopStyleVar(3);

    ImGuiID dockspaceId = ImGui::GetID("ofxImGuiStyleBasicDockspace");
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
    ImGuiID center = dockspaceId;
    ImGui::DockBuilderSplitNode(center, ImGuiDir_Left, 0.30f, &left, &center);
    ImGui::DockBuilderSplitNode(center, ImGuiDir_Right, 0.35f, &right, &center);

    ImGui::DockBuilderDockWindow(ICON_FA_PALETTE " ofxImGuiStyle", left);
    ImGui::DockBuilderDockWindow(ICON_FA_STAR " Preview Widgets", center);
    ImGui::DockBuilderDockWindow(ICON_FA_PALETTE " Theme Editor", right);
    ImGui::DockBuilderDockWindow("Dear ImGui Demo", center);

    ImGui::DockBuilderFinish(dockspaceId);
}

void ofApp::drawControls() {
    if (ImGui::Begin(ICON_FA_PALETTE " ofxImGuiStyle")) {
        ImGui::TextWrapped("Reusable style helpers: bundled fonts + icons (ImFonts), "
                           "the vendored ImTheme registry + tweaker, and the OF wrapper.");

        ImGui::SeparatorText("Themes");
        if (ImTheme::ShowSelector(currentThemeId))
            uiScale = ImTheme::UIScale();

        if (ImGui::Button(ICON_FA_DICE " Random Accent", ImVec2(-1, 0)))
            ImTheme::ApplyRandomAccent();

        ImGui::SeparatorText("Scale");
        ImGui::SetNextItemWidth(-1);
        if (ImGui::SliderFloat("##scale", &uiScale, 0.75f, 2.5f, "%.2fx"))
            ImTheme::SetUIScale(uiScale);
        if (ImGui::Button("1x"))   { uiScale = 1.0f; ImTheme::SetUIScale(uiScale); }
        ImGui::SameLine();
        if (ImGui::Button("1.5x")) { uiScale = 1.5f; ImTheme::SetUIScale(uiScale); }
        ImGui::SameLine();
        if (ImGui::Button("2x"))   { uiScale = 2.0f; ImTheme::SetUIScale(uiScale); }

        ImGui::SeparatorText("Style Snapshot (.bin)");
        const std::string themePath = ofToDataPath("ofxImGuiStyle-basic-theme.bin", true);
        if (ImGui::Button(ICON_FA_SAVE " Save Style", ImVec2(-1, 0))) {
            ImTheme::SaveStyle(themePath.c_str());
        }
        if (ImGui::Button(ICON_FA_FOLDER_OPEN " Load Style", ImVec2(-1, 0))) {
            if (ImTheme::LoadStyle(themePath.c_str())) {
                ImTheme::ApplyCompactMetrics();
                ImTheme::Commit();
            }
        }
        ImGui::TextDisabled("%s", themePath.c_str());

        ImGui::SeparatorText("Windows");
        ImGui::Checkbox("Style Editor", &showStyleEditor);
        ImGui::Checkbox("ImGui Demo Window", &showDemoWindow);
    }
    ImGui::End();
}

void ofApp::drawPreview() {
    if (ImGui::Begin(ICON_FA_STAR " Preview Widgets")) {
        ImGui::Text("Font: Input Sans");
        ImGui::Text("Icons: " ICON_FA_CUBE " " ICON_FA_CAMERA " " ICON_FA_LIGHTBULB " " ICON_FA_MUSIC);
        ImGui::Spacing();

        ImGui::SeparatorText("Common Controls");
        ImGui::Checkbox("Enabled", &enabled);
        ImGui::SliderFloat("Amount", &amount, 0.0f, 1.0f);
        ImGui::ColorEdit4("Accent Color", color);

        ImGui::SeparatorText("Knobs (imgui-knobs)");
        ImGui::BeginGroup();
        ImGuiKnobs::Knob("Cutoff", &knobCutoff, 0.f, 1.f, 0.f, "%.2f",
                         ImGuiKnobVariant_WiperDot, 42.f);
        ImGui::SameLine();
        ImGuiKnobs::Knob("Reso", &knobReso, 0.f, 1.f, 0.f, "%.2f",
                         ImGuiKnobVariant_Dot, 42.f);
        ImGui::SameLine();
        ImGuiKnobs::KnobInt("Step", &knobSteps, 0, 16, 0.f, "%d",
                            ImGuiKnobVariant_Stepped, 42.f);
        ImGui::EndGroup();

        const char* items[] = {"Small", "Medium", "Large"};
        ImGui::Combo("Choice", &choice, items, 3);

        if (ImGui::Button(ICON_FA_CHECK " Primary Action"))
            amount = 1.0f;
        ImGui::SameLine();
        if (ImGui::Button(ICON_FA_TIMES " Reset"))
            amount = 0.0f;

        ImGui::SeparatorText("Tree / Tabs");
        if (ImGui::TreeNode(ICON_FA_FOLDER " Scene")) {
            ImGui::BulletText(ICON_FA_CUBE " Mesh");
            ImGui::BulletText(ICON_FA_LIGHTBULB " Light");
            ImGui::BulletText(ICON_FA_CAMERA " Camera");
            ImGui::TreePop();
        }

        if (ImGui::BeginTabBar("PreviewTabs")) {
            if (ImGui::BeginTabItem("Info")) {
                ImGui::TextWrapped("Use this window to see how theme colours, compact metrics, icons, and scale affect normal ImGui widgets.");
                ImGui::EndTabItem();
            }
            if (ImGui::BeginTabItem("Progress")) {
                ImGui::ProgressBar(amount, ImVec2(-1, 0));
                ImGui::EndTabItem();
            }
            ImGui::EndTabBar();
        }
    }
    ImGui::End();
}

void ofApp::drawStyleEditor() {
    bool open = showStyleEditor;
    ImGui::SetNextWindowSize(ImVec2(500, 600), ImGuiCond_FirstUseEver);
    if (ImGui::Begin(ICON_FA_PALETTE " Theme Editor", &open)) {
        // ImTheme's tabbed "Theme Tweaks" + "Style Editor" widget.
        ImTheme::ShowThemeTweakGui(&tweaks);
    }
    ImGui::End();
    showStyleEditor = open;
}
