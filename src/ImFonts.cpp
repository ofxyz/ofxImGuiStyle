#include "ImFonts.h"

#include "IconsFontAwesome5.h"
#include "InputSans_Regular_ttf.h"
#include "JetBrainsMono_BoldItalic_ttf.h"
#include "JetBrainsMono_Bold_ttf.h"
#include "JetBrainsMono_Italic_ttf.h"
#include "JetBrainsMono_Regular_ttf.h"
#include "fa_solid_900_ttf.h"

#include <algorithm>
#include <cstdio>
#include <cstring>

namespace ImFonts
{
    namespace
    {
        ImFont* loadEmbeddedTtf(ImFontAtlas* atlas,
                                float pixelSize,
                                const unsigned char* data,
                                unsigned int size,
                                const char* name)
        {
            if (!atlas || !data || size == 0)
                return nullptr;

            void* fontCopy = ImGui::MemAlloc(size);
            std::memcpy(fontCopy, data, size);
            ImFontConfig cfg;
            cfg.FontDataOwnedByAtlas = true;
            cfg.OversampleH          = cfg.OversampleV = 1;
            cfg.PixelSnapH           = true;
            std::strncpy(cfg.Name, name, sizeof(cfg.Name) - 1);
            return atlas->AddFontFromMemoryTTF(fontCopy, (int)size, pixelSize, &cfg);
        }

        struct EmbeddedTtf
        {
            const unsigned char* data;
            unsigned int         size;
            const char*          name;
        };

        EmbeddedTtf jetBrainsMonoSource(JetBrainsMonoVariant variant)
        {
            switch (variant) {
            case JetBrainsMonoVariant::Regular:
                return { JetBrainsMono_Regular_ttf_data, JetBrainsMono_Regular_ttf_size,
                         "JetBrains Mono" };
            case JetBrainsMonoVariant::Bold:
                return { JetBrainsMono_Bold_ttf_data, JetBrainsMono_Bold_ttf_size,
                         "JetBrains Mono Bold" };
            case JetBrainsMonoVariant::Italic:
                return { JetBrainsMono_Italic_ttf_data, JetBrainsMono_Italic_ttf_size,
                         "JetBrains Mono Italic" };
            case JetBrainsMonoVariant::BoldItalic:
                return { JetBrainsMono_BoldItalic_ttf_data, JetBrainsMono_BoldItalic_ttf_size,
                         "JetBrains Mono Bold Italic" };
            default:
                return { nullptr, 0, "" };
            }
        }
    }

    const char* JetBrainsMonoVariantName(JetBrainsMonoVariant variant)
    {
        switch (variant) {
        case JetBrainsMonoVariant::Regular: return "JetBrains Mono";
        case JetBrainsMonoVariant::Bold:    return "JetBrains Mono Bold";
        case JetBrainsMonoVariant::Italic:  return "JetBrains Mono Italic";
        case JetBrainsMonoVariant::BoldItalic: return "JetBrains Mono Bold Italic";
        default:                            return "Unknown";
        }
    }

    ImFont* LoadDefaultFonts(ImFontAtlas* atlas, float pixelSize)
    {
        if (!atlas) return nullptr;

        ImFont* primary = nullptr;

        {
            void* fontCopy = ImGui::MemAlloc(InputSans_Regular_ttf_size);
            std::memcpy(fontCopy, InputSans_Regular_ttf_data, InputSans_Regular_ttf_size);
            ImFontConfig cfg;
            cfg.FontDataOwnedByAtlas = true;
            std::strncpy(cfg.Name, "Input Sans", sizeof(cfg.Name) - 1);
            primary = atlas->AddFontFromMemoryTTF(
                fontCopy, (int)InputSans_Regular_ttf_size, pixelSize, &cfg);
        }

        {
            void* iconCopy = ImGui::MemAlloc(fa_solid_900_ttf_size);
            std::memcpy(iconCopy, fa_solid_900_ttf_data, fa_solid_900_ttf_size);
            ImFontConfig iconCfg;
            iconCfg.FontDataOwnedByAtlas = true;
            iconCfg.MergeMode            = true;
            iconCfg.GlyphMinAdvanceX     = pixelSize;
            iconCfg.PixelSnapH           = true;
            std::strncpy(iconCfg.Name, "Font Awesome 5 Solid", sizeof(iconCfg.Name) - 1);
            static const ImWchar iconRanges[] = { ICON_MIN_FA, ICON_MAX_FA, 0 };
            atlas->AddFontFromMemoryTTF(
                iconCopy, (int)fa_solid_900_ttf_size, pixelSize, &iconCfg, iconRanges);
        }

        return primary;
    }

    ImFont* LoadJetBrainsMonoFont(ImFontAtlas* atlas,
                                  float pixelSize,
                                  JetBrainsMonoVariant variant)
    {
        const EmbeddedTtf source = jetBrainsMonoSource(variant);
        return loadEmbeddedTtf(atlas, pixelSize, source.data, source.size, source.name);
    }

    JetBrainsMonoFonts LoadJetBrainsMono(ImFontAtlas* atlas, float pixelSize)
    {
        JetBrainsMonoFonts fonts;
        fonts.regular    = LoadJetBrainsMonoFont(atlas, pixelSize, JetBrainsMonoVariant::Regular);
        fonts.bold       = LoadJetBrainsMonoFont(atlas, pixelSize, JetBrainsMonoVariant::Bold);
        fonts.italic     = LoadJetBrainsMonoFont(atlas, pixelSize, JetBrainsMonoVariant::Italic);
        fonts.boldItalic = LoadJetBrainsMonoFont(atlas, pixelSize, JetBrainsMonoVariant::BoldItalic);
        return fonts;
    }

    ImFont* LoadCodeEditorFont(ImFontAtlas* atlas, float pixelSize)
    {
        return LoadJetBrainsMonoFont(atlas, pixelSize, JetBrainsMonoVariant::Regular);
    }

    ImFont* RebuildDefaultFonts(ImFontAtlas* atlas, float pixelSize)
    {
        if (!atlas) return nullptr;
        atlas->Clear();
        return LoadDefaultFonts(atlas, pixelSize);
    }

    bool IconButton(const char* icon, const char* id, bool ghost)
    {
        // Height: match the current row height exactly so this button never
        // shrinks the selection highlight or misaligns sibling widgets. padY
        // is derived so the glyph lands exactly in the vertical centre.
        const float rowH = ImGui::GetFrameHeight();
        const float fs   = ImGui::GetFontSize();
        const float padY = std::max(0.f, (rowH - fs) * 0.5f);
        constexpr float kPadX = 3.f;

        ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(kPadX, padY));

        if (ghost) {
            ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.f, 0.f, 0.f, 0.f));
            ImGui::PushStyleColor(ImGuiCol_ButtonActive,
                                  ImGui::GetStyleColorVec4(ImGuiCol_ButtonActive));
        }

        // Compose "glyph##id" — glyph is the visible label, ##id makes the
        // ImGui ID unique without affecting the rendered text.
        char label[128];
        std::snprintf(label, sizeof(label), "%s%s", icon, id);
        const bool clicked = ImGui::Button(label);

        if (ghost) ImGui::PopStyleColor(2);
        ImGui::PopStyleVar();
        return clicked;
    }

    bool ToolbarIconButton(const char* icon, const char* id, bool ghost)
    {
        const float fs  = ImGui::GetFontSize();
        const float pad = std::max(2.f, fs * 0.12f);
        const float side = fs + pad * 2.f;
        const ImVec2 btnSize(side, side);

        ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(pad, pad));

        if (ghost) {
            ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.f, 0.f, 0.f, 0.f));
            ImGui::PushStyleColor(ImGuiCol_ButtonActive,
                                  ImGui::GetStyleColorVec4(ImGuiCol_ButtonActive));
        }

        char label[128];
        std::snprintf(label, sizeof(label), "%s%s", icon, id);
        const bool clicked = ImGui::Button(label, btnSize);

        if (ghost) ImGui::PopStyleColor(2);
        ImGui::PopStyleVar();
        return clicked;
    }
}
