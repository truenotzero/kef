#include <render/kcamera.h>

#include <math.h>
#include "core/kmath.h"

#define K_CAMERA_MIN_PITCH kDegf(-90.0f)
#define K_CAMERA_MAX_PITCH kDegf(90.0f)

// updates the camera's forward, up and right vectors
static kCamera calculate_base(kCamera self) {
    kVec3f f;
    f32 cos_pitch = cosf(self.r_pitch);
    f.x = cosf(self.r_yaw) * cos_pitch;
    f.y = sinf(self.r_pitch);
    f.z = sinf(self.r_yaw) * cos_pitch;
    self.r_forward = kVecNorm3f(f);

    self.r_right = kVecNorm3f(kVecCrossf(K_VEC3F_UP, self.r_forward));
    self.r_up = kVecCrossf(self.r_forward, self.r_right);
    return self;
}

kCamera kCameraAddYawPitch(kCamera self, f32 delta_yaw, f32 delta_pitch) {
    self.r_yaw += delta_yaw;
    // pitch must be in the range (-90,90) for the current lookAt algorithm
    // to work properly
    f32 new_pitch = self.r_pitch + delta_pitch; 
    if (new_pitch > K_CAMERA_MIN_PITCH && new_pitch < K_CAMERA_MAX_PITCH) {
        self.r_pitch = new_pitch;
    }

    return calculate_base(self);
}

kMat4f kCameraViewMat(kCamera self) {
    kMat4f rot = kMatIdentity4f();
    rot.xy[0][0] = self.r_right.x;
    rot.xy[1][0] = self.r_right.y;
    rot.xy[2][0] = self.r_right.z;
    rot.xy[0][1] = self.r_up.x;
    rot.xy[1][1] = self.r_up.y;
    rot.xy[2][1] = self.r_up.z;
    rot.xy[0][2] = self.r_forward.x;
    rot.xy[1][2] = self.r_forward.y;
    rot.xy[2][2] = self.r_forward.z;
    kMat4f trans = kMatTranslate(-self.pos.x, -self.pos.y, -self.pos.z);
    return kMatMul4f(rot, trans);
}
