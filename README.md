# ofxImGuiStyle

![preview-example_Basic](example_Basic/preview.png)

`ofxImGuiStyle` is an umbrella addon for `ofxImGui` that ships four
independent, pure Dear ImGui sub-libraries plus a tiny openFrameworks-aware
glue layer:

- **`ImTheme`** (`ImTheme.{h,cpp}` + `ImThemeRegistry.{h,cpp}`)
  Theme system: 17 built-in themes vendored from
  [pthom/hello_imgui](https://github.com/pthom/hello_imgui)
  (`Darcula`, `DarculaDarker`, `LightRounded`, `ImGuiColorsDark`,
  `MaterialFlat`, ...) plus an open-ended `RegisterCustom` registry for
  addons / apps to contribute their own (`tb303`, `tr808`, `tr909`,
  `wasp`, ...), plus a `ShowSelector` / `ShowThemeTweakGui` UI.
- **`ImFonts`** (`ImFonts.{h,cpp}`)
  Font / icon loader: bundled **Input Sans Regular** TTF (UI font) merged
  with **Font Awesome 5 Solid** for icons, plus `IconButton` /
  `IconButtonGhost` widgets sized from the current row height.
- **`ImKnobs`** (`ImKnobs.h` + vendored `src/imgui-knobs/`)
  Rotary knob widgets from [imgui-knobs](https://github.com/altschuler/imgui-knobs)
  (MIT). Multiple visual variants; reads colours from the active ImGui style.
- **`ImMidi`** (`ImMidi.h`, `ImMidiMapper.h`, `ImMidiRegistry.{h,cpp}`)
  MIDI learn / CC mapping for any ImGui control from
  [ImMidiMapper](https://github.com/ofxyz/ImMidiMapper) (MIT). Optional
  `ofxMidi` I/O when that addon is linked; otherwise use `pushCC()` yourself.
- **`IconsFontAwesome5.h`** — vendored constants from
  [juliettef/IconFontCppHeaders](https://github.com/juliettef/IconFontCppHeaders).

Fonts and themes are deliberately kept in the same addon: in real apps the
two are coupled (different themes presume specific x-height / weight, icon
buttons depend on FA glyphs being merged into the active font).

Both APIs are pure Dear ImGui — they don't include any openFrameworks header.
The only OF coupling lives in callers that already own an `ofxImGui::Gui`
(passed in by the caller for `setDefaultFont` / `rebuildFontsTexture`).

## What's not here

`ofxImGuiStyle` no longer ships:

- A `class ofxImGuiStyle` C++ object — replaced by the free-function
  `ImTheme::` / `ImFonts::` APIs. Instances are unnecessary because all
  meaningful state (the captured base style, the selected theme id, the
  custom-theme registry) is genuinely global.
- Hardware-specific themes (TB-303, TR-808, TR-909, EDP Wasp, ...) — those
  live with the instruments that own them (`ofx303`, `ofx808`, `ofx909`,
  `ofxWasp`). Each instrument's `Kit.h` registers its theme through
  `ImTheme::RegisterCustom` behind a `__has_include` guard.

## Quick start

Add both addons to `addons.make`:

```make
ofxImGui
ofxImGuiStyle
```

Then load fonts + apply a theme after `ofxImGui::Gui::setup()`:

```cpp
#include "ofxImGui.h"
#include <ofxImGuiStyle/src/ImTheme.h>
#include <ofxImGuiStyle/src/ImThemeRegistry.h>
#include <ofxImGuiStyle/src/ImFonts.h>

ofxImGui::Gui gui;

void ofApp::setup() {
    gui.setup();

    if (ImFont* f = ImFonts::LoadDefaultFonts(ImGui::GetIO().Fonts, 15.f))
        gui.setDefaultFont(f);
    gui.rebuildFontsTexture();

    // Theme + HiDPI scale in one call (DetectOsScale when uiScale <= 0).
    ImTheme::Setup("DarculaDarker");
}
```

## Selecting + tweaking themes at runtime

```cpp
static std::string themeId = "DarculaDarker";

if (ImTheme::ShowSelector(themeId)) { }  // applies + re-commits scale

ImTheme::ShowThemeTweakGui(&tweaks);
```

`ShowSelector` applies the picked theme and automatically snapshots the
unscaled baseline and re-applies the stored UI scale — no extra calls needed.

## UI scale

```cpp
float uiScale = ImTheme::UIScale();

if (ImGui::SliderFloat("Scale", &uiScale, 0.75f, 2.5f))
    ImTheme::SetUIScale(uiScale);
```

`Setup`, `ApplyByName`, and `ShowSelector` all re-commit scale when the theme
changes. `SetUIScale` only rescales from the existing baseline (fast path for
sliders).

## Hand-tweaking metrics

If you edit `ImGui::GetStyle()` after a theme (extra padding, rounding, etc.),
call `Commit()` once to snapshot and re-apply scale:

```cpp
ImTheme::Setup(ImTheme::Theme_DarculaDarker);
ImGui::GetStyle().WindowPadding = ImVec2(14, 12);
// ...
ImTheme::Commit();
```

## Registering a custom theme from an addon

```cpp
ImTheme::RegisterCustom({
    "myaddon",
    "My Addon",
    [] {
        ImGui::GetStyle() = ImGuiStyle{};
        ImGui::StyleColorsDark();
        // ...
    }
});

ImTheme::Setup("myaddon");
```

Guard with `__has_include` if the addon must build without `ofxImGuiStyle`
(see `ofx303 / 808 / 909 / Wasp`).

## Style snapshot files (.bin)

```cpp
ImTheme::SaveStyle(ofToDataPath("my_style.bin", true).c_str());

if (ImTheme::LoadStyle(path)) {
    ImTheme::ApplyCompactMetrics();
    ImTheme::Commit();
}
```

## Icon buttons (ImFonts)

```cpp
#include <ofxImGuiStyle/src/ImFonts.h>
#include <ofxImGuiStyle/src/IconsFontAwesome5.h>

if (ImFonts::IconButtonGhost(visible ? ICON_FA_EYE : ICON_FA_EYE_SLASH, "##eye"))
    visible = !visible;
```

## Knobs (ImKnobs)

```cpp
#include <ofxImGuiStyle/src/ImKnobs.h>

float cutoff = 0.5f;
if (ImGuiKnobs::Knob("Cutoff", &cutoff, 0.f, 1.f, 0.f, "%.2f",
                     ImGuiKnobVariant_WiperDot, 42.f))
{
    // value changed
}
```

## MIDI mapping (ImMidi)

Requires **ofxMidi** in `addons.make` for hardware ports. Wire in your app
(e.g. `tb303::midiBridge::setup()` via `tb303::kit::setupMidi()`).

```cpp
#include <ofxImGuiStyle/src/ImMidi.h>

ImMidi::Setup(ofToDataPath("midiMapper.json", true));

void ofApp::update() {
    ImMidi::Update();
}

// inside ImGui:
ImGuiKnobs::Knob("Cutoff", &cutoff, 0.f, 1.f);
ImMidi::Mapper().watchLast("cutoff", &cutoff, 0.f, 1.f, "Cutoff");

ImMidi::DrawEditor(&showMidiMapper);
```

Right-click any registered control → **MIDI Learn…** → move a hardware knob.

## Examples

- `example_Basic` — fonts, icons, knobs, theme selector, scale slider, `.bin` I/O.
- `example_LumaStudio` — desktop-app mockup with custom padding + `Commit()`.

## Attribution

- `ImTheme.{h,cpp}` — adapted from
  [pthom/hello_imgui](https://github.com/pthom/hello_imgui) (MIT).
- `IconsFontAwesome5.h` — [juliettef/IconFontCppHeaders](https://github.com/juliettef/IconFontCppHeaders) (MIT).
- `src/imgui-knobs/` — [altschuler/imgui-knobs](https://github.com/altschuler/imgui-knobs) (MIT).
- `ImMidiMapper.h` — ImMidiMapper (MIT).
- Bundled fonts: **Input Sans Regular** and **Font Awesome 5 Free Solid**.
