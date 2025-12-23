#include <map>

#include "SmoothedController.hpp"
#include "SmoothedControllerConfig.hpp"
#include "sombrero/shared/QuaternionUtils.hpp"
#include "sombrero/shared/Vector3Utils.hpp"
#include "types/Wrapper.hpp"
#include "logger.hpp"
#include "beatsaber-hook/shared/utils/hooking.hpp"

// GlobbalNamespace
#include "GlobalNamespace/IVRPlatformHelper.hpp"
#include "GlobalNamespace/VRController.hpp"
#include "GlobalNamespace/VRControllerTransformOffset.hpp"

// UnityEngine
#include "UnityEngine/Time.hpp"
#include "UnityEngine/XR/XRNode.hpp"

std::map<UnityEngine::XR::XRNode, SmoothedController::Wrapper*> wrappers;

long long CurrentTimeMs() {
    // Set up a timer
    auto lastUpdate = std::chrono::system_clock::now();
    auto milis = std::chrono::duration_cast<std::chrono::milliseconds>(lastUpdate.time_since_epoch());
    auto now = milis.count();
    return now;
}

void SmoothController(UnityW<GlobalNamespace::VRController> instance) {
    using namespace System;
    using namespace System::Collections::Generic;
    using namespace UnityEngine;
    using namespace UnityEngine::XR;
    using namespace Sombrero;

    static float posSmoth = 20.f - std::clamp(getSmoothedControllerConfig().PositionSmoothing.GetValue(), 0.f, 20.f);
    static float rotSmoth = 20.f - std::clamp(getSmoothedControllerConfig().RotationSmoothing.GetValue(), 0.f, 20.f);

    auto transform = instance->get_transform();
    auto node = instance->____node;

    SmoothedController::Wrapper* wrapperI;
    if (!wrappers.contains(node)) {
        wrapperI = new SmoothedController::Wrapper();
        wrappers[node] = wrapperI;
    } else {
        wrapperI = wrappers[node];
    }

    float angDiff = FastQuaternion::Angle(wrapperI->smoothedRotation, transform->get_localRotation());
    wrapperI->angleVelocitySnap = std::min(wrapperI->angleVelocitySnap + angDiff, 90.f);

    float snapMulti = std::clamp(wrapperI->angleVelocitySnap / getSmoothedControllerConfig().SmallMovementThresholdAngle.GetValue(), .1f, 2.5f);

    if (wrapperI->angleVelocitySnap > .1f) {
        wrapperI->angleVelocitySnap -= std::max(.4f, wrapperI->angleVelocitySnap / 1.7f);
    }

    if (getSmoothedControllerConfig().PositionSmoothing.GetValue() > 0.f) {
        wrapperI->smoothedPosition =
            FastVector3::Lerp(wrapperI->smoothedPosition, transform->get_localPosition(), posSmoth * Time::get_deltaTime() * snapMulti);
        transform->set_localPosition(wrapperI->smoothedPosition);
    }

    if (getSmoothedControllerConfig().RotationSmoothing.GetValue() > 0.f) {
        wrapperI->smoothedRotation =
            FastQuaternion::Lerp(wrapperI->smoothedRotation, transform->get_localRotation(), rotSmoth * Time::get_deltaTime() * snapMulti);
        transform->set_localRotation(wrapperI->smoothedRotation);
    }
}

MAKE_HOOK_MATCH(
    VRController_Update,
    &GlobalNamespace::VRController::Update,
    void,
    GlobalNamespace::VRController* self
) {
    // If smoothing is disabled, just call the original method
    if (!getSmoothedControllerConfig().Enabled.GetValue()) {
        return VRController_Update(self);
    }

    using namespace UnityEngine;
    using namespace UnityEngine::XR;
    using namespace Sombrero;

    // Because Quest is dumb and we don't have transpilers, we gotta reimplement this entire method :D

    Vector3 lastTrackedPosition;
    Quaternion localRotation;
    if (!self->_vrPlatformHelper->GetNodePose(self->_node, self->_nodeIndex, lastTrackedPosition, localRotation)) {
        if (FastVector3::zero() != self->_lastTrackedPosition) {
            lastTrackedPosition = self->_lastTrackedPosition;
        } else if (self->_node == XRNode::LeftHand) {
            lastTrackedPosition = FastVector3(-.2f, .05f, .0f);
        } else if (self->_node == XRNode::RightHand) {
            lastTrackedPosition = FastVector3(.2f, .05f, .0f);
        }
    } else {
        self->_lastTrackedPosition = lastTrackedPosition;
    }

    auto selftransform = self->get_transform();
    selftransform->set_localPosition(lastTrackedPosition);
    selftransform->set_localRotation(localRotation);

    // optimized if (self->get_gameObject()->get_name().starts_with("Controller")) {
    auto selfGo = self->get_gameObject();
    auto goName = selfGo->get_name();
    if (goName->_stringLength > 0 && goName[0] == 'C') {
        SmoothController(self);
    }
}

void SmoothedController::Hooks::VRController() {
    INSTALL_HOOK(Logger, VRController_Update);
}