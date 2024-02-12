#ifndef K_CAMERA_H_
#define K_CAMERA_H_

#include <ktype.h>

typedef struct kCamera {
    kVec3f pos;
    f32 r_yaw;
    f32 r_pitch;
    kVec3f r_forward;
    kVec3f r_right;
    kVec3f r_up;
} kCamera;

// +yaw is left (ccw)
// +pitch is up
kCamera kCameraAddYawPitch(kCamera self, f32 delta_yaw, f32 delta_pitch);

kMat4f kCameraViewMat(kCamera self);

#endif // K_CAMERA_H_
