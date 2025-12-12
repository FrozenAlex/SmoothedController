#include "main.hpp"

#include "beatsaber-hook/shared/utils/il2cpp-functions.hpp"
#include "bsml/shared/BSML.hpp"
#include "custom-types/shared/register.hpp"
#include "logger.hpp"
#include "modInfo.hpp"
#include "scotland2/shared/modloader.h"
#include "SmoothedController.hpp"
#include "SmoothedControllerConfig.hpp"

/// @brief Called at the early stages of game loading
/// @param info The mod info.  Update this with your mod's info.
/// @return
MOD_EXPORT_FUNC void setup(CModInfo& info) {
    // Convert the mod info to a C struct and set that as the modloader info.
    info = modInfo.to_c();

    // Initialize the config
    getSmoothedControllerConfig().Init(modInfo);

    Logger.info("Completed setup!");
}

/// @brief Called early on in the game loading
/// @return
MOD_EXPORT_FUNC void load() {
    // Initialize il2cpp functions
    il2cpp_functions::Init();
    custom_types::Register::AutoRegister();
    BSML::Init();

    BSML::Register::RegisterSettingsMenu("SmoothedController", DidActivate, true);

    // Must make sure mod loading order is correct
    auto fingersaberModInfo = CModInfo{.id = "fingersaber"};
    modloader_require_mod(&fingersaberModInfo, CMatchType::MatchType_IdOnly);

    SmoothedController::Hooks::VRController();
}