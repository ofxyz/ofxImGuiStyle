#pragma once

#include "ofxImGui.h"
#include "IconsFontAwesome5.h"

class ofxImGuiStyle {
public:
	void loadFonts(ofxImGui::Gui& gui, float fontSize = 16.0f, float dpiScale = 1.0f);
	void rebuildFonts(ofxImGui::Gui& gui, float fontSize, float dpiScale = 1.0f);

	void draw(bool* open = nullptr);

	static void applyDarkTheme();
	static void applyLightTheme();
	static void applyClassicTheme();
	static void applyCompactMetrics(float scrollbarSize = 8.0f,
	                                float scrollbarRounding = 4.0f,
	                                float grabMinSize = 8.0f);
	static void applyRandomAccentTheme();

	static bool saveTheme(const std::string& path);
	static bool loadTheme(const std::string& path);

	void captureBaseStyle();
	void applyScale(float scale);
	bool hasBaseStyle() const { return m_hasBaseStyle; }

	float getFontSize() const { return m_fontSize; }

	// -------------------------------------------------------------------------
	// Icon button helpers
	//
	// These handle all sizing/centering math so call sites don't need to push
	// FramePadding manually.
	//
	//   icon  — FA glyph string, e.g. ICON_FA_EYE
	//   id    — ImGui ID suffix, e.g. "##eye"
	//   ghost — transparent background, hover tint only (good for eye/lock icons)
	//
	// The button is automatically sized to the full current row height with the
	// glyph centred inside, and uses compact horizontal padding.
	// -------------------------------------------------------------------------
	static bool IconButton(const char* icon, const char* id = "##ib",
	                       bool ghost = false);

	/// Shorthand for ghost = true.
	static bool IconButtonGhost(const char* icon, const char* id = "##ib") {
		return IconButton(icon, id, true);
	}

private:
	float m_fontSize = 16.0f;
	float m_dpiScale = 1.0f;
	ImGuiStyle m_baseStyle;
	bool m_hasBaseStyle = false;
};
