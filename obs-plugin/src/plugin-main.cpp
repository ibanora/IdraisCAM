#include "idraiscam_source.hpp"
#include "network.hpp"
#include <obs-module.h>

OBS_DECLARE_MODULE()
OBS_MODULE_USE_DEFAULT_LOCALE("idraiscam", "pt-PT")

static std::unique_ptr<idraiscam::NetworkRuntime> g_network;

static const char* sourceName(void*) { return obs_module_text("IdraisCAM.Source"); }
static void* sourceCreate(obs_data_t* settings, obs_source_t* source) {
    return new idraiscam::IdraisCamSource(source, settings);
}
static void sourceDestroy(void* data) { delete static_cast<idraiscam::IdraisCamSource*>(data); }
static void sourceUpdate(void* data, obs_data_t* settings) {
    static_cast<idraiscam::IdraisCamSource*>(data)->update(settings);
}
static obs_properties_t* sourceProperties(void* data) { return idraiscam::IdraisCamSource::properties(data); }
static void sourceDefaults(obs_data_t* settings) { idraiscam::IdraisCamSource::defaults(settings); }

static obs_source_info sourceInfo{};

bool obs_module_load(void) {
    g_network = std::make_unique<idraiscam::NetworkRuntime>();
    if (!g_network->ok()) {
        blog(LOG_ERROR, "[IdraisCAM] Winsock initialization failed");
        return false;
    }
    sourceInfo.id = "idraiscam_source";
    sourceInfo.type = OBS_SOURCE_TYPE_INPUT;
    sourceInfo.output_flags = OBS_SOURCE_ASYNC_VIDEO | OBS_SOURCE_AUDIO;
    sourceInfo.get_name = sourceName;
    sourceInfo.create = sourceCreate;
    sourceInfo.destroy = sourceDestroy;
    sourceInfo.update = sourceUpdate;
    sourceInfo.get_defaults = sourceDefaults;
    sourceInfo.get_properties = sourceProperties;
    obs_register_source(&sourceInfo);
    blog(LOG_INFO, "[IdraisCAM] Plugin 1.0.0 loaded");
    return true;
}

void obs_module_unload(void) { g_network.reset(); }

MODULE_EXPORT const char* obs_module_description(void) {
    return "IdraisCAM — Android camera and microphone source for OBS Studio";
}
