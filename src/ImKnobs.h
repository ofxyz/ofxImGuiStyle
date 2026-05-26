#pragma once
//
// ImKnobs
// -------
// Vendored imgui-knobs rotary widgets (MIT, Simon Altschuler).
// https://github.com/altschuler/imgui-knobs
//
// Pure Dear ImGui — picks up colours from the active ImGuiStyle, so knobs
// match whatever theme ImTheme applied.
//
// Usage:
//   ImGuiKnobs::Knob("Cutoff", &cutoff, 0.f, 1.f, 0.f, "%.2f",
//                    ImGuiKnobVariant_WiperDot, 42.f);
//
// Pair with ImMidiMapper::watchLast() for MIDI learn on any knob.
//

#include "imgui-knobs/imgui-knobs.h"
