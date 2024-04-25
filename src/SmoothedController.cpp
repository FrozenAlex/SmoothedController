#include "SmoothedController.hpp"
#include "SmoothedControllerConfig.hpp"
#include "logging.hpp"
#include "bsml/shared/BSML/MainThreadScheduler.hpp"
#include "bsml/shared/BSML.hpp"
#include "bsml/shared/BSML-Lite/Creation/Text.hpp"
#include "bsml/shared/BSML-Lite/Creation/Layout.hpp"

void DidActivate(HMUI::ViewController* self, bool firstActivation, bool addedToHierarchy, bool screenSystemEnabling) {
    using namespace UnityEngine;

    if (firstActivation) {
        UnityW<GameObject> container = BSML::Lite::CreateScrollableSettingsContainer(self->get_transform());
        
        AddConfigValueToggle(container->get_transform(), getSmoothedControllerConfig().Enabled);
        AddConfigValueIncrementFloat(container->get_transform(), getSmoothedControllerConfig().PositionSmoothing, 2, .5f, 0, 20);
        AddConfigValueIncrementFloat(container->get_transform(), getSmoothedControllerConfig().RotationSmoothing, 2, .5f, 0, 20);
        AddConfigValueIncrementFloat(container->get_transform(), getSmoothedControllerConfig().SmallMovementThresholdAngle, 2, .5f, 0.f, 15);
    }
}

void SmoothedController::Install() {
    custom_types::Register::AutoRegister();
    BSML::Init();

    BSML::Register::RegisterSettingsMenu("SmoothedController", DidActivate, true);
    
    SmoothedController::Hooks::VRController();
}