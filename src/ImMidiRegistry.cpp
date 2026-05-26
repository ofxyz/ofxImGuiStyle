#include "ImMidiRegistry.h"

namespace ImMidi
{

namespace {

ImMidiMapper g_mapper;
std::string  g_mapPath;

} // namespace

ImMidiMapper& Mapper() { return g_mapper; }

void Setup(const std::string& mapPath)
{
    g_mapPath = mapPath;
    if (!g_mapPath.empty()) {
        g_mapper.defaultMapPath = g_mapPath;
        g_mapper.load(g_mapPath);
    }
}

void Update() { g_mapper.update(); }

void DrawEditor(bool* open) { g_mapper.drawEditor(open); }

void DrawPortsUI()
{
    if (g_mapper.drawPortsUI)
        g_mapper.drawPortsUI();
}

bool HasBackend() { return g_mapper.drawPortsUI != nullptr; }

bool OpenDefaultPorts() { return false; }

bool Save()
{
    if (g_mapPath.empty())
        return false;
    return g_mapper.save(g_mapPath);
}

bool Load()
{
    if (g_mapPath.empty())
        return false;
    return g_mapper.load(g_mapPath);
}

} // namespace ImMidi
