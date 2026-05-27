#include "ImThemeRegistry.h"

#include <algorithm>
#include <cstring>
#include <fstream>
#include <random>

// Optional GLFW dependency: only used by DetectOsScale(). Guarded with
// __has_include so addons that pull in ImThemeRegistry without a GLFW backend
// (e.g. raw OpenGL ES targets) still compile.
#if !defined(TARGET_OPENGLES) && !defined(TARGET_RASPBERRY_PI) \
    && defined(__has_include) && __has_include(<GLFW/glfw3.h>)
#  define IMTHEME_HAS_GLFW 1
#  include <GLFW/glfw3.h>
#endif

namespace ImTheme
{
    // -------------------------------------------------------------------------
    // Internal state
    // -------------------------------------------------------------------------
    static std::vector<CustomTheme> g_customs;
    static std::string              g_currentId;

    static ImGuiStyle               g_baseStyle;
    static bool                     g_hasBaseStyle = false;
    static float                    g_uiScale      = 1.f;

    static float clampUIScale(float scale)
    {
        return std::clamp(scale, 0.5f, 4.f);
    }

    static void commitStyle()
    {
        g_baseStyle    = ImGui::GetStyle();
        g_hasBaseStyle = true;
        ImGuiStyle& style = ImGui::GetStyle();
        style             = g_baseStyle;
        style.ScaleAllSizes(g_uiScale);
        ImGui::GetIO().FontGlobalScale = g_uiScale;
    }

    // -------------------------------------------------------------------------
    // Custom registry
    // -------------------------------------------------------------------------
    void RegisterCustom(CustomTheme theme)
    {
        for (auto& existing : g_customs)
        {
            if (existing.id == theme.id)
            {
                existing = std::move(theme);
                return;
            }
        }
        g_customs.push_back(std::move(theme));
    }

    const std::vector<CustomTheme>& Customs()
    {
        return g_customs;
    }

    // -------------------------------------------------------------------------
    // Setup
    // -------------------------------------------------------------------------
    bool Setup(const std::string& themeId, float uiScale)
    {
        if (uiScale <= 0.f)
            uiScale = DetectOsScale();
        g_uiScale = clampUIScale(uiScale);
        return ApplyByName(themeId);
    }

    bool Setup(Theme_ theme, float uiScale)
    {
        if (uiScale <= 0.f)
            uiScale = DetectOsScale();
        g_uiScale = clampUIScale(uiScale);
        ApplyTheme(theme);
        g_currentId = Name(theme) ? Name(theme) : "";
        commitStyle();
        return true;
    }

    // -------------------------------------------------------------------------
    // ApplyByName / CurrentName
    // -------------------------------------------------------------------------
    bool ApplyByName(const std::string& id)
    {
        // 1. Custom registry first.
        for (const auto& c : g_customs)
        {
            if (c.id == id)
            {
                if (c.apply)
                {
                    // Reset to ImGui defaults so the custom apply() lambda
                    // starts from a clean slate rather than stacking on top of
                    // whatever was already in place.
                    ImGui::GetStyle() = ImGuiStyle{};
                    c.apply();
                }
                g_currentId = id;
                commitStyle();
                return true;
            }
        }

        // 2. Built-in theme by upstream Name() string.
        Theme_ resolved = FromName(id.c_str());
        const char* resolvedName = Name(resolved);
        // FromName returns Theme_ImGuiColorsClassic on unknown input. To
        // distinguish "explicitly classic" from "unknown", compare names.
        const bool isKnown = (resolvedName && id == resolvedName);
        if (isKnown)
        {
            ApplyTheme(resolved); // fires SetThemeChangedCallback if set
            g_currentId = id;
            commitStyle();
            return true;
        }

        return false;
    }

    const std::string& CurrentName()
    {
        return g_currentId;
    }

    // -------------------------------------------------------------------------
    // Selector widget — combined built-ins + customs
    // -------------------------------------------------------------------------
    bool ShowSelector(std::string& currentId)
    {
        bool changed = false;

        struct ThemeEntry {
            std::string id;
            const char* label;
        };
        std::vector<ThemeEntry> entries;
        entries.reserve(Theme_Count + g_customs.size());

        const int nb = (int)Theme_Count;
        for (int i = 3; i < nb + 3; ++i) {
            Theme_ theme_i = (Theme_)(i % nb);
            const char* themeName = Name(theme_i);
            if (themeName && themeName[0] != '\0')
                entries.push_back({themeName, themeName});
        }
        for (const auto& c : g_customs)
            entries.push_back({c.id, c.label.c_str()});

        if (entries.empty())
            return false;

        std::vector<const char*> labels;
        labels.reserve(entries.size());
        int currentIdx = 0;
        for (size_t i = 0; i < entries.size(); ++i) {
            labels.push_back(entries[i].label);
            if (entries[i].id == currentId)
                currentIdx = (int)i;
        }

        ImGui::SetNextItemWidth(-1.f);
        if (ImGui::Combo("Theme##ImThemeSelector", &currentIdx, labels.data(),
                         (int)labels.size())) {
            currentId = entries[(size_t)currentIdx].id;
            ApplyByName(currentId);
            changed = true;
        }

        return changed;
    }

    // -------------------------------------------------------------------------
    // .bin save / load
    // -------------------------------------------------------------------------
    bool SaveStyle(const char* path)
    {
        if (!path) return false;
        std::ofstream f(path, std::ios::binary);
        if (!f.is_open()) return false;
        const ImGuiStyle& style = ImGui::GetStyle();
        f.write(reinterpret_cast<const char*>(&style), sizeof(ImGuiStyle));
        return f.good();
    }

    bool LoadStyle(const char* path)
    {
        if (!path) return false;
        std::ifstream f(path, std::ios::binary);
        if (!f.is_open()) return false;
        ImGuiStyle& style = ImGui::GetStyle();
        f.read(reinterpret_cast<char*>(&style), sizeof(ImGuiStyle));
        return f.good();
    }

    // -------------------------------------------------------------------------
    // ApplyRandomAccent (moved from ofxImGuiStyle::applyRandomAccentTheme)
    // -------------------------------------------------------------------------
    void ApplyRandomAccent()
    {
        // Reset metrics to Dear ImGui defaults before applying colours. Without
        // this, callers that CaptureBaseStyle() then ScaleAllSizes(uiScale)
        // (e.g. ofxKit) would stack scale on already-scaled padding every time.
        ImGui::GetStyle() = ImGuiStyle{};
        ImGui::StyleColorsDark();

        static std::random_device rd;
        static std::mt19937       rng(rd());
        std::uniform_real_distribution<float> hueDist(0.0f, 1.0f);
        std::uniform_real_distribution<float> satDist(0.5f, 0.9f);

        const float hue = hueDist(rng);
        const float sat = satDist(rng);
        auto accent = [&](float alpha) -> ImVec4 {
            float r = 0.f, g = 0.f, b = 0.f;
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

        ApplyCompactMetrics();
        commitStyle();
    }

    // -------------------------------------------------------------------------
    // ApplyCompactMetrics
    // -------------------------------------------------------------------------
    void ApplyCompactMetrics(float scrollbarSize,
                             float scrollbarRounding,
                             float grabMinSize)
    {
        ImGuiStyle& style       = ImGui::GetStyle();
        style.ScrollbarSize     = scrollbarSize;
        style.ScrollbarRounding = scrollbarRounding;
        style.GrabMinSize       = grabMinSize;
    }

    // -------------------------------------------------------------------------
    // UIScale / Commit / CaptureBaseStyle / ApplyScale
    // -------------------------------------------------------------------------
    float UIScale()
    {
        return g_uiScale;
    }

    void SetUIScale(float scale)
    {
        g_uiScale = clampUIScale(scale);
        if (g_hasBaseStyle)
        {
            ImGuiStyle& style = ImGui::GetStyle();
            style             = g_baseStyle;
            style.ScaleAllSizes(g_uiScale);
            ImGui::GetIO().FontGlobalScale = g_uiScale;
        }
    }

    void Commit()
    {
        commitStyle();
    }

    void CaptureBaseStyle()
    {
        g_baseStyle    = ImGui::GetStyle();
        g_hasBaseStyle = true;
    }

    bool HasBaseStyle()
    {
        return g_hasBaseStyle;
    }

    void ApplyScale(float scale)
    {
        SetUIScale(scale);
        if (!g_hasBaseStyle)
            commitStyle();
    }

    // -------------------------------------------------------------------------
    // DetectOsScale
    // -------------------------------------------------------------------------
    float DetectOsScale()
    {
#ifdef IMTHEME_HAS_GLFW
        if (GLFWmonitor* monitor = glfwGetPrimaryMonitor())
        {
            float xs = 1.f, ys = 1.f;
            glfwGetMonitorContentScale(monitor, &xs, &ys);
            const float scale = std::max(xs, ys);
            if (scale > 0.1f && scale < 8.f)
                return scale;
        }
#endif
        return 1.0f;
    }

} // namespace ImTheme
