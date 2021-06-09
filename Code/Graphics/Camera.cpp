#include "Camera.h"

#include "imgui.h"

void ge2::Camera::LookAt(Vector3f lookAtLocation)
{
    if (transform)
    {
        transform->LookAt(lookAtLocation);
    }
}

ge2::Matrix4x4f ge2::Camera::Matrix() const
{
    if (!transform)
    {
        return Matrix4x4f::Identity();
    }

    //Move
    Matrix4x4f result = {
        1.f, 0.f, 0.f, -transform->position.x,
        0.f, 1.f, 0.f, -transform->position.y,
        0.f, 0.f, 1.f, -transform->position.z,
        0.f, 0.f, 0.f, 1.f
    };

    //Rotate and flip so +z is forward;
    return (Quaternion::FromEuler(Vector3f{ 0.f, PI, 0.f }) * transform->rotation.Inverse()).RotationMatrix() * result;
}
