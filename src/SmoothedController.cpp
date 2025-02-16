#include "bsml/shared/BSML-Lite/Creation/Layout.hpp"
#include "SmoothedControllerConfig.hpp"
#include "UnityEngine/GameObject.hpp"

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
