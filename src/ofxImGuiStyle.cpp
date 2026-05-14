#include "ofxImGuiStyle.h"
#include "InputSans_Regular_ttf.h"
#include "fa_solid_900_ttf.h"
#include <fstream>
#include <random>

// ---------------------------------------------------------------------------
// Font loading
// ---------------------------------------------------------------------------

void ofxImGuiStyle::loadFonts(ofxImGui::Gui& gui, float fontSize, float dpiScale) {
	m_fontSize = fontSize;
	m_dpiScale = dpiScale;
	float pixelSize = fontSize * dpiScale;

	ImGuiIO& io = ImGui::GetIO();

	{
		void* fontCopy = ImGui::MemAlloc(InputSans_Regular_ttf_size);
		memcpy(fontCopy, InputSans_Regular_ttf_data, InputSans_Regular_ttf_size);
		ImFontConfig cfg;
		cfg.FontDataOwnedByAtlas = true;
		strncpy(cfg.Name, "Input Sans", sizeof(cfg.Name) - 1);
		ImFont* font = io.Fonts->AddFontFromMemoryTTF(
			fontCopy, InputSans_Regular_ttf_size, pixelSize, &cfg);
		if (font) gui.setDefaultFont(font);
	}

	{
		void* iconCopy = ImGui::MemAlloc(fa_solid_900_ttf_size);
		memcpy(iconCopy, fa_solid_900_ttf_data, fa_solid_900_ttf_size);
		ImFontConfig iconCfg;
		iconCfg.FontDataOwnedByAtlas = true;
		iconCfg.MergeMode = true;
		iconCfg.GlyphMinAdvanceX = pixelSize;
		iconCfg.PixelSnapH = true;
		strncpy(iconCfg.Name, "Font Awesome 5 Solid", sizeof(iconCfg.Name) - 1);
		static const ImWchar iconRanges[] = { ICON_MIN_FA, ICON_MAX_FA, 0 };
		io.Fonts->AddFontFromMemoryTTF(
			iconCopy, fa_solid_900_ttf_size, pixelSize, &iconCfg, iconRanges);
	}

	gui.rebuildFontsTexture();
}

void ofxImGuiStyle::rebuildFonts(ofxImGui::Gui& gui, float fontSize, float dpiScale) {
	ImGui::GetIO().Fonts->Clear();
	loadFonts(gui, fontSize, dpiScale);
}

// ---------------------------------------------------------------------------
// Theme presets
// ---------------------------------------------------------------------------

void ofxImGuiStyle::applyDarkTheme() {
	ImVec4* c = ImGui::GetStyle().Colors;

	ImVec4 bg       (0.10f, 0.10f, 0.12f, 1.00f);
	ImVec4 bgChild  (0.12f, 0.12f, 0.14f, 1.00f);
	ImVec4 bgPopup  (0.14f, 0.14f, 0.16f, 1.00f);
	ImVec4 border   (0.22f, 0.22f, 0.26f, 0.60f);
	ImVec4 text     (0.90f, 0.90f, 0.92f, 1.00f);
	ImVec4 textDim  (0.50f, 0.50f, 0.54f, 1.00f);
	ImVec4 accent   (0.33f, 0.55f, 0.85f, 1.00f);
	ImVec4 accentHov(0.40f, 0.62f, 0.92f, 1.00f);
	ImVec4 accentAct(0.26f, 0.46f, 0.75f, 1.00f);
	ImVec4 frame    (0.16f, 0.16f, 0.19f, 1.00f);
	ImVec4 frameHov (0.20f, 0.20f, 0.24f, 1.00f);
	ImVec4 frameAct (0.24f, 0.24f, 0.28f, 1.00f);
	ImVec4 header   (0.18f, 0.18f, 0.22f, 1.00f);
	ImVec4 tab      (0.14f, 0.14f, 0.17f, 1.00f);
	ImVec4 tabActive(0.20f, 0.20f, 0.24f, 1.00f);

	c[ImGuiCol_Text]                  = text;
	c[ImGuiCol_TextDisabled]          = textDim;
	c[ImGuiCol_WindowBg]              = bg;
	c[ImGuiCol_ChildBg]               = bgChild;
	c[ImGuiCol_PopupBg]               = bgPopup;
	c[ImGuiCol_Border]                = border;
	c[ImGuiCol_BorderShadow]          = ImVec4(0, 0, 0, 0);
	c[ImGuiCol_FrameBg]               = frame;
	c[ImGuiCol_FrameBgHovered]        = frameHov;
	c[ImGuiCol_FrameBgActive]         = frameAct;
	c[ImGuiCol_TitleBg]               = ImVec4(0.08f, 0.08f, 0.10f, 1.00f);
	c[ImGuiCol_TitleBgActive]         = ImVec4(0.12f, 0.12f, 0.15f, 1.00f);
	c[ImGuiCol_TitleBgCollapsed]      = ImVec4(0.06f, 0.06f, 0.08f, 0.75f);
	c[ImGuiCol_MenuBarBg]             = ImVec4(0.12f, 0.12f, 0.14f, 1.00f);
	c[ImGuiCol_ScrollbarBg]           = ImVec4(0.08f, 0.08f, 0.10f, 0.60f);
	c[ImGuiCol_ScrollbarGrab]         = ImVec4(0.28f, 0.28f, 0.32f, 1.00f);
	c[ImGuiCol_ScrollbarGrabHovered]  = ImVec4(0.36f, 0.36f, 0.40f, 1.00f);
	c[ImGuiCol_ScrollbarGrabActive]   = ImVec4(0.44f, 0.44f, 0.48f, 1.00f);
	c[ImGuiCol_CheckMark]             = accent;
	c[ImGuiCol_SliderGrab]            = accent;
	c[ImGuiCol_SliderGrabActive]      = accentAct;
	c[ImGuiCol_Button]                = frame;
	c[ImGuiCol_ButtonHovered]         = accentHov;
	c[ImGuiCol_ButtonActive]          = accentAct;
	c[ImGuiCol_Header]                = header;
	c[ImGuiCol_HeaderHovered]         = accentHov;
	c[ImGuiCol_HeaderActive]          = accentAct;
	c[ImGuiCol_Separator]             = border;
	c[ImGuiCol_SeparatorHovered]      = accentHov;
	c[ImGuiCol_SeparatorActive]       = accentAct;
	c[ImGuiCol_ResizeGrip]            = ImVec4(0.26f, 0.26f, 0.30f, 0.40f);
	c[ImGuiCol_ResizeGripHovered]     = accentHov;
	c[ImGuiCol_ResizeGripActive]      = accentAct;
	c[ImGuiCol_Tab]                   = tab;
	c[ImGuiCol_TabHovered]            = accentHov;
	c[ImGuiCol_TabSelected]           = tabActive;
	c[ImGuiCol_TabDimmed]             = tab;
	c[ImGuiCol_TabDimmedSelected]     = header;
	c[ImGuiCol_DockingPreview]        = ImVec4(accent.x, accent.y, accent.z, 0.40f);
	c[ImGuiCol_DockingEmptyBg]        = ImVec4(0.08f, 0.08f, 0.10f, 1.00f);
	c[ImGuiCol_PlotLines]             = accent;
	c[ImGuiCol_PlotLinesHovered]      = accentHov;
	c[ImGuiCol_PlotHistogram]         = accent;
	c[ImGuiCol_PlotHistogramHovered]  = accentHov;
	c[ImGuiCol_TextSelectedBg]        = ImVec4(accent.x, accent.y, accent.z, 0.30f);
	c[ImGuiCol_DragDropTarget]        = accentHov;
	c[ImGuiCol_NavHighlight]          = accent;
	c[ImGuiCol_NavWindowingHighlight] = ImVec4(1, 1, 1, 0.70f);
	c[ImGuiCol_NavWindowingDimBg]     = ImVec4(0.2f, 0.2f, 0.2f, 0.20f);
	c[ImGuiCol_ModalWindowDimBg]      = ImVec4(0.1f, 0.1f, 0.1f, 0.60f);

	ImGuiStyle& style = ImGui::GetStyle();
	style.WindowRounding    = 6.0f;
	style.FrameRounding     = 4.0f;
	style.GrabRounding      = 3.0f;
	style.TabRounding       = 4.0f;
	style.ScrollbarRounding = 6.0f;
	style.WindowBorderSize  = 1.0f;
	style.FrameBorderSize   = 0.0f;
	style.PopupBorderSize   = 1.0f;
	style.WindowPadding     = ImVec2(10, 10);
	style.FramePadding      = ImVec2(8, 4);
	style.ItemSpacing       = ImVec2(8, 6);
	applyCompactMetrics();
}

void ofxImGuiStyle::applyLightTheme() {
	ImGui::StyleColorsLight();
	applyCompactMetrics();
}

void ofxImGuiStyle::applyClassicTheme() {
	ImGui::StyleColorsClassic();
	applyCompactMetrics();
}

void ofxImGuiStyle::applyCompactMetrics(float scrollbarSize,
                                        float scrollbarRounding,
                                        float grabMinSize) {
	ImGuiStyle& style = ImGui::GetStyle();
	style.ScrollbarSize = scrollbarSize;
	style.ScrollbarRounding = scrollbarRounding;
	style.GrabMinSize = grabMinSize;
}

void ofxImGuiStyle::applyRandomAccentTheme() {
	// Reset metrics to Dear ImGui defaults before applying colours. Without this,
	// callers that `captureBaseStyle()` then `ScaleAllSizes(uiScale)` (e.g. ofxKit)
	// would stack scale on already-scaled padding every time Random is clicked.
	ImGui::GetStyle() = ImGuiStyle{};

	// Keep the random theme readable by using dark as the base and varying only
	// the accent hue/saturation.
	ImGui::StyleColorsDark();

	static std::random_device rd;
	static std::mt19937 rng(rd());
	std::uniform_real_distribution<float> hueDist(0.0f, 1.0f);
	std::uniform_real_distribution<float> satDist(0.5f, 0.9f);

	const float hue = hueDist(rng);
	const float sat = satDist(rng);
	auto accent = [&](float alpha) -> ImVec4 {
		float r = 0.0f, g = 0.0f, b = 0.0f;
		ImGui::ColorConvertHSVtoRGB(hue, sat, 0.85f, r, g, b);
		return ImVec4(r, g, b, alpha);
	};

	ImVec4* c = ImGui::GetStyle().Colors;
	c[ImGuiCol_CheckMark]         = accent(1.0f);
	c[ImGuiCol_SliderGrab]        = accent(1.0f);
	c[ImGuiCol_SliderGrabActive]  = accent(1.0f);
	c[ImGuiCol_Button]            = accent(0.4f);
	c[ImGuiCol_ButtonHovered]     = accent(1.0f);
	c[ImGuiCol_ButtonActive]      = accent(0.8f);
	c[ImGuiCol_Header]            = accent(0.35f);
	c[ImGuiCol_HeaderHovered]     = accent(0.80f);
	c[ImGuiCol_HeaderActive]      = accent(1.00f);
	c[ImGuiCol_FrameBgHovered]    = accent(0.35f);
	c[ImGuiCol_FrameBgActive]     = accent(0.60f);
	c[ImGuiCol_TabHovered]        = accent(0.80f);
	c[ImGuiCol_TabSelected]       = accent(0.60f);
	c[ImGuiCol_SeparatorHovered]  = accent(0.78f);
	c[ImGuiCol_SeparatorActive]   = accent(1.00f);
	c[ImGuiCol_ResizeGripHovered] = accent(0.67f);
	c[ImGuiCol_ResizeGripActive]  = accent(0.95f);
	c[ImGuiCol_DockingPreview]    = accent(0.70f);
	c[ImGuiCol_TitleBgActive]     = accent(0.40f);

	applyCompactMetrics();
}

// ---------------------------------------------------------------------------
// Base style capture & scale
// ---------------------------------------------------------------------------

void ofxImGuiStyle::captureBaseStyle() {
	m_baseStyle = ImGui::GetStyle();
	m_hasBaseStyle = true;
}

void ofxImGuiStyle::applyScale(float scale) {
	if (!m_hasBaseStyle) {
		captureBaseStyle();
	}
	ImGuiStyle& style = ImGui::GetStyle();
	style = m_baseStyle;
	style.ScaleAllSizes(scale);
	ImGui::GetIO().FontGlobalScale = scale;
}

// ---------------------------------------------------------------------------
// Theme persistence
// ---------------------------------------------------------------------------

bool ofxImGuiStyle::saveTheme(const std::string& path) {
	std::ofstream f(path, std::ios::binary);
	if (!f.is_open()) return false;
	ImGuiStyle& style = ImGui::GetStyle();
	f.write(reinterpret_cast<const char*>(&style), sizeof(ImGuiStyle));
	return true;
}

bool ofxImGuiStyle::loadTheme(const std::string& path) {
	std::ifstream f(path, std::ios::binary);
	if (!f.is_open()) return false;
	ImGuiStyle& style = ImGui::GetStyle();
	f.read(reinterpret_cast<char*>(&style), sizeof(ImGuiStyle));
	return true;
}

// ---------------------------------------------------------------------------
// Theme editor window
// ---------------------------------------------------------------------------

void ofxImGuiStyle::draw(bool* open) {
	if (open && !*open) return;

	ImGui::SetNextWindowSize(ImVec2(500, 600), ImGuiCond_FirstUseEver);
	if (ImGui::Begin(ICON_FA_PALETTE " Theme Editor", open)) {
		if (ImGui::Button("Dark")) {
			applyDarkTheme();
			captureBaseStyle();
		}
		ImGui::SameLine();
		if (ImGui::Button("Light")) {
			applyLightTheme();
			captureBaseStyle();
		}
		ImGui::SameLine();
		if (ImGui::Button("Classic")) {
			applyClassicTheme();
			captureBaseStyle();
		}
		ImGui::SameLine();
		if (ImGui::Button("ImGui Dark")) {
			ImGui::StyleColorsDark();
			captureBaseStyle();
		}

		ImGui::Spacing();
		if (ImGui::Button(ICON_FA_SAVE " Save Theme")) {
			saveTheme(ofToDataPath("theme.bin", true));
		}
		ImGui::SameLine();
		if (ImGui::Button(ICON_FA_FOLDER_OPEN " Load Theme")) {
			if (loadTheme(ofToDataPath("theme.bin", true))) {
				captureBaseStyle();
			}
		}

	ImGui::Separator();
	ImGui::ShowStyleEditor();
}
ImGui::End();
}

// ---------------------------------------------------------------------------
// Icon button helpers
// ---------------------------------------------------------------------------

bool ofxImGuiStyle::IconButton(const char* icon, const char* id, bool ghost)
{
	// Height: match the current row height exactly so this button never shrinks
	// the selection highlight or misaligns sibling widgets.  padY is derived so
	// the glyph lands exactly in the vertical centre.
	const float rowH = ImGui::GetFrameHeight();
	const float fs   = ImGui::GetFontSize();
	const float padY = std::max(0.f, (rowH - fs) * 0.5f);
	// Compact horizontal padding keeps icon-only buttons narrow.
	constexpr float kPadX = 3.f;

	ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(kPadX, padY));

	if (ghost) {
		// Transparent background; hover/active colours are left as-is so the
		// button still reacts visually to mouse interaction.
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.f, 0.f, 0.f, 0.f));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive,
		                      ImGui::GetStyleColorVec4(ImGuiCol_ButtonActive));
	}

	// Compose "glyph##id" — the glyph is the visible label, the ##id suffix
	// makes the ImGui ID unique without affecting the rendered text.
	char label[128];
	std::snprintf(label, sizeof(label), "%s%s", icon, id);
	const bool clicked = ImGui::Button(label);

	if (ghost) ImGui::PopStyleColor(2);
	ImGui::PopStyleVar();
	return clicked;
}
