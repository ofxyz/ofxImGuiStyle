# ofxImGuiStyle

![preview-example_Basic](example_Basic/preview.png)

`ofxImGuiStyle` is a small companion addon for `ofxImGui` that centralizes
shared ImGui styling for openFrameworks apps.

It provides:

- bundled Input Sans font loading
- bundled Font Awesome 5 Solid icon merging
- reusable dark, light, and classic theme helpers
- compact scrollbar/grab metrics for HiDPI screens
- random accent-theme generation
- base-style capture and UI scale application
- binary `ImGuiStyle` save/load helpers
- a simple built-in style editor window

The addon is intentionally generic. Higher-level editor addons, such as
`ofxKit`, should own their menus, preferences, and persistence policy while
delegating reusable font/theme/style mechanics to `ofxImGuiStyle`.

## Usage

Add both addons to `addons.make`:

```make
ofxImGui
ofxImGuiStyle
```

Then load fonts after `ofxImGui::Gui::setup()` and before drawing the first
frame:

```cpp
#include "ofxImGui.h"
#include "ofxImGuiStyle.h"

ofxImGui::Gui gui;
ofxImGuiStyle style;

void ofApp::setup() {
    gui.setup();

    style.loadFonts(gui, 15.0f);
    ofxImGuiStyle::applyDarkTheme();
    style.captureBaseStyle();
}

void ofApp::draw() {
    gui.begin();

    if (ImGui::Button("Random Theme")) {
        ofxImGuiStyle::applyRandomAccentTheme();
        style.captureBaseStyle();
    }

    gui.end();
}
```

## Scaling

Use `captureBaseStyle()` after applying a theme, then call `applyScale()` when
the UI scale changes. This avoids compounding paddings and margins every time
the scale or theme changes.

```cpp
ofxImGuiStyle::applyDarkTheme();
style.captureBaseStyle();

style.applyScale(1.5f);
```

## Theme Files

Themes are saved as binary snapshots of `ImGuiStyle`.

```cpp
ofxImGuiStyle::saveTheme(ofToDataPath("theme.bin", true));

if (ofxImGuiStyle::loadTheme(ofToDataPath("theme.bin", true))) {
    ofxImGuiStyle::applyCompactMetrics();
    style.captureBaseStyle();
}
```

## Icon Buttons

`ofxImGuiStyle` provides two static helpers that produce correctly sized and
vertically centred Font Awesome icon buttons with no manual `PushStyleVar` at
the call site.

### How it works

The math is pre-computed inside the helper from two ImGui runtime values that
are always available:

| Value | Meaning |
|-------|---------|
| `ImGui::GetFrameHeight()` | Current row height (`fontSize + 2 × FramePadding.y`) |
| `ImGui::GetFontSize()` | Rendered glyph height |

```
padY = (GetFrameHeight() − GetFontSize()) / 2
```

This makes the button exactly as tall as the current row, with the glyph
visually centred top-to-bottom. A fixed compact `padX = 3 px` keeps icon-only
buttons narrow.

### API

```cpp
// Solid button (visible background).
bool ofxImGuiStyle::IconButton(const char* icon,
                               const char* id    = "##ib",
                               bool        ghost = false);

// Ghost variant — transparent background, hover/active tint only.
// Ideal for overlay icons (eye, lock, etc.) inside list rows.
bool ofxImGuiStyle::IconButtonGhost(const char* icon,
                                    const char* id = "##ib");
```

- `icon` — Font Awesome glyph string, e.g. `ICON_FA_EYE`
- `id`   — ImGui ID suffix; must be unique within the parent window, e.g. `"##eye"`
- Returns `true` on click (same as `ImGui::Button`)

### Example

```cpp
#include <ofxImGuiStyle/src/ofxImGuiStyle.h>
#include <ofxImGuiStyle/src/IconsFontAwesome5.h>

// Ghost eye toggle — centres itself in whatever row height is current
if (ofxImGuiStyle::IconButtonGhost(
        visible ? ICON_FA_EYE : ICON_FA_EYE_SLASH, "##eye")) {
    visible = !visible;
}
ImGui::SameLine();

// Normal (solid) icon button
if (ofxImGuiStyle::IconButton(ICON_FA_PLUS, "##add"))
    addItem();
```

- `example_Basic` demonstrates the focused API surface: fonts, icons, presets,
  random accent themes, scaling, save/load, and the style editor.
- `example_LumaStudio` is a larger fictional app mockup showing what a styled
  ImGui application can feel like.
