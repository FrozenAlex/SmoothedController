#pragma once

#include "HMUI/ViewController.hpp"

namespace SmoothedController {
    namespace Hooks {
        void VRController();
    }
}  // namespace SmoothedController

void DidActivate(HMUI::ViewController* self, bool firstActivation, bool addedToHierarchy, bool screenSystemEnabling);
