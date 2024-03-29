#ifndef K_CAMERA_H_
#define K_CAMERA_H_

#include <ktype.h>

/// A world-space camera
/// Update the `pos` field
/// All other fields are read-only
typedef struct kCamera {
    kVec3f pos;
    f32 r_yaw;
    f32 r_pitch;
    kVec3f r_forward;
    kVec3f r_right;
    kVec3f r_up;
} kCamera;

/// +yaw is left (ccw)
/// +pitch is up
/// sets the camera's view angles
/// pitch must be in the range (-90,90)
/// invalid view angle inputs won't change the current view angle
kCamera kCameraSetYawPitch(kCamera self, f32 yaw, f32 pitch);

/// Adds an angle to the camera's current view angle
kCamera kCameraAddYawPitch(kCamera self, f32 delta_yaw, f32 delta_pitch);


/// Create a view matrix with the camera's parameters
kMat4f kCameraViewMat(kCamera self);

#endif // K_CAMERA_H_
