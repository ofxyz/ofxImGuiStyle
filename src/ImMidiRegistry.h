#pragma once
//
// ImMidiRegistry — singleton access to ImMidiMapper (no ofxMidi here).
//
// Wire hardware MIDI from your app/addon (e.g. ofx303MidiBridge) by setting:
//   ImMidi::Mapper().drawPortsUI
//   ImMidi::Mapper().onSendCC
// and calling ImMidi::Mapper().pushCC() from your MIDI callback.
//

#include "ImMidiMapper.h"

#include <string>

namespace ImMidi
{

ImMidiMapper& Mapper();

/// Load saved mappings. Pass an empty path to skip auto-load.
void Setup(const std::string& mapPath = "midiMapper.json");

/// Apply queued MIDI events — call once per frame before drawing mapped widgets.
void Update();

/// Full mapping editor window.
void DrawEditor(bool* open = nullptr);

/// Calls Mapper().drawPortsUI when set.
void DrawPortsUI();

/// True when Mapper().drawPortsUI has been wired.
bool HasBackend();

/// No-op in core — apps with ofxMidi open ports in their own bridge.
bool OpenDefaultPorts();

bool Save();
bool Load();

} // namespace ImMidi
