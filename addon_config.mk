meta:
	ADDON_NAME = ofxImGuiStyle
	ADDON_DESCRIPTION = Shared fonts, icons, and theme editor for ofxImGui apps
	ADDON_AUTHOR = Bruno Herfst
	ADDON_URL = https://github.com/ofxyz/ofxImGuiStyle

common:
	ADDON_DEPENDENCIES = ofxImGui
	ADDON_SOURCES += src/imgui-knobs/imgui-knobs.cpp
	ADDON_SOURCES_EXCLUDE = src/gen_font_headers.py
