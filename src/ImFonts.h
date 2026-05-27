#pragma once
//
// ImFonts
// -------
// Pure-ImGui font and icon helpers that pair with ImTheme.
//
// Ships:
//   - LoadDefaultFonts(atlas, pixelSize):
//       loads the bundled Input Sans (regular) typeface and merges Font Awesome 5
//       Solid icons into the same font, into a caller-supplied ImFontAtlas.
//       Returns the primary font handle; the caller is responsible for
//       triggering its renderer's atlas upload (e.g.
//       `ofxImGui::Gui::rebuildFontsTexture()` in openFrameworks).
//   - LoadJetBrainsMono / LoadJetBrainsMonoFont:
//       bundled JetBrains Mono (Regular, Bold, Italic, BoldItalic) for code editors,
//       markdown (`ofxImGuiMarkdown`), and any monospace UI.
//   - LoadCodeEditorFont(atlas, pixelSize):
//       shorthand for JetBrains Mono Regular at `pixelSize`.
//   - IconButton / IconButtonGhost:
//       row-height-aware FontAwesome icon buttons that compute padding from
//       the current style so callers don't push/pop FramePadding manually.
//
// No openFrameworks dependencies — this header is safe to include from any
// ImGui project.
//
// Icon constants (e.g. ICON_FA_PALETTE) live in IconsFontAwesome5.h, vendored
// from juliettef/IconFontCppHeaders (MIT).
//

#include "imgui.h"

namespace ImFonts
{
    enum class JetBrainsMonoVariant
    {
        Regular = 0,
        Bold,
        Italic,
        BoldItalic,
        Count
    };

    const char* JetBrainsMonoVariantName(JetBrainsMonoVariant variant);

    struct JetBrainsMonoFonts
    {
        ImFont* regular    = nullptr;
        ImFont* bold       = nullptr;
        ImFont* italic     = nullptr;
        ImFont* boldItalic = nullptr;
    };

    // Load the bundled Input Sans + Font Awesome 5 Solid (merged) fonts into
    // `atlas` at `pixelSize`. Returns the Input Sans font handle (the default
    // font), or nullptr on failure.
    //
    // The caller owns the atlas texture rebuild step. For openFrameworks:
    //
    //     ImFont* font = ImFonts::LoadDefaultFonts(ImGui::GetIO().Fonts, 15.f);
    //     gui.setDefaultFont(font);
    //     gui.rebuildFontsTexture();
    //
    ImFont* LoadDefaultFonts(ImFontAtlas* atlas, float pixelSize);

    // Load one JetBrains Mono face into `atlas`. Safe to call after
    // LoadDefaultFonts on the same atlas.
    ImFont* LoadJetBrainsMonoFont(ImFontAtlas* atlas,
                                  float pixelSize,
                                  JetBrainsMonoVariant variant);

    // Load Regular + Bold + Italic + BoldItalic at the same pixel size.
    JetBrainsMonoFonts LoadJetBrainsMono(ImFontAtlas* atlas, float pixelSize);

    // Monospace font for code editors — JetBrains Mono Regular.
    // Load into the same atlas as LoadDefaultFonts, then rebuild the texture.
    ImFont* LoadCodeEditorFont(ImFontAtlas* atlas, float pixelSize);

    // Same as LoadDefaultFonts but clears the atlas first.
    ImFont* RebuildDefaultFonts(ImFontAtlas* atlas, float pixelSize);

    // -------------------------------------------------------------------------
    // FontAwesome icon button helpers
    // -------------------------------------------------------------------------
    //
    // These size the button to the full current row height and centre the glyph
    // vertically, using compact horizontal padding. Callers don't push/pop
    // FramePadding themselves.
    //
    //   icon  - FA glyph string, e.g. ICON_FA_EYE
    //   id    - ImGui ID suffix, e.g. "##eye"
    //   ghost - transparent background, hover tint only (eye/lock-style icons)
    //
    bool IconButton(const char* icon, const char* id = "##ib", bool ghost = false);

    // Shorthand for ghost = true.
    inline bool IconButtonGhost(const char* icon, const char* id = "##ib")
    {
        return IconButton(icon, id, true);
    }

    // Square FA button for vertical toolbars; side length tracks ImGui::GetFontSize()
    // so icons stay centred when UI scale / font size changes.
    bool ToolbarIconButton(const char* icon, const char* id = "##tb", bool ghost = false);
}
