#include "main.hpp"
#include "SmoothedController.hpp"
#include "SmoothedControllerConfig.hpp"
#include "logging.hpp"



extern "C" __attribute__((visibility("default"))) void setup(CModInfo& info) {
    info.id = MOD_ID;
    info.version = VERSION;
    info.version_long = 0;
    modInfo.assign(info);

    getSmoothedControllerConfig().Init(modInfo);
    INFO("Completed SmoothedController setup!");
}

extern "C" __attribute__((visibility("default"))) void late_load() {
    il2cpp_functions::Init();

    INFO("Installing SmoothedController...");
    SmoothedController::Install();
    INFO("Installed SmoothedController!");
}