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

private:
	float m_fontSize = 16.0f;
	float m_dpiScale = 1.0f;
	ImGuiStyle m_baseStyle;
	bool m_hasBaseStyle = false;
};
