#pragma once

#include "sombrero/shared/QuaternionUtils.hpp"
#include "sombrero/shared/Vector3Utils.hpp"

namespace SmoothedController {
    class Wrapper {
       public:
        Sombrero::FastVector3 smoothedPosition;
        Sombrero::FastQuaternion smoothedRotation;
        float angleVelocitySnap;

        Wrapper() {
            using namespace UnityEngine;
            smoothedPosition = Sombrero::FastVector3::zero();
            smoothedRotation = Sombrero::FastQuaternion::identity();
            angleVelocitySnap = 1.0f;
        }
    };
}  // namespace SmoothedController
