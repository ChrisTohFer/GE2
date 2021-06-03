#include "Camera.h"

#include "imgui.h"

void ge2::Camera::LookAt(Vector3f lookAtLocation)
{
    Vector3f worldUp = Vector3f::UnitY();
    Vector3f worldForward = Vector3f::UnitZ();

    Vector3f forward = (lookAtLocation - position).Normalise();
    Vector3f right = CrossProduct(forward, worldUp).Normalise();
    Vector3f planeForward = CrossProduct(worldUp, right).Normalise();
    Vector3f up = CrossProduct(right, forward).Normalise();

    ImGui::Begin("LookAt");
    ImGui::Text("Forward: %f, %f, %f", forward.x, forward.y, forward.z);
    ImGui::Text("Right: %f, %f, %f", right.x, right.y, right.z);
    ImGui::Text("PlaneForward: %f, %f, %f", planeForward.x, planeForward.y, planeForward.z);
    ImGui::Text("Up: %f, %f, %f", up.x, up.y, up.z);
    ImGui::End();

    if (forward == Vector3f::Zero())
    {
        return;
    }
    
    //
    forward = -forward;
    float trace = right.x + up.y + forward.z;
    if (trace > 0.0f)
    {
        float s = 0.5 / sqrtf(trace + 1.f);
        rotation.w = 0.25f / s;
        rotation.v.x = (up.z - forward.y) * s;
        rotation.v.y = (forward.x - right.z) * s;
        rotation.v.z = (right.y - up.x) * s;
    }
    else if (right.x > up.y && right.x > forward.z)
    {
        float s = 2.0f * sqrt(1.0f + right.x - up.y - forward.z);
        rotation.w = (up.z - forward.y) / s;
        rotation.v.x = 0.25f * s;
        rotation.v.y = (up.x + right.y) / s;
        rotation.v.z = (forward.x + right.z) / s;
    }
    else if (up.y > forward.z)
    {
        float s = 2.0 * sqrt(1.0f + up.y - right.x - forward.z);
        rotation.w = (forward.x - right.z) / s;
        rotation.v.x = (up.x + right.y) / s;
        rotation.v.y = 0.25f * s;
        rotation.v.z = (forward.y + up.z) / s;
    }
    else
    {
        float s = 2.0f * sqrt(1.0f + forward.z - right.x - up.y);
        rotation.w = (right.y - up.x) / s;
        rotation.v.x = (forward.x + right.z) / s;
        rotation.v.y = (forward.y + up.z) / s;
        rotation.v.z = 0.25f * s;
    }

    rotation = rotation.Inverse();

    //Works but rolls
    //Vector3f rotAxis = CrossProduct(worldForward, forward);
    //float dot = DotProduct(worldForward, forward);
    //
    //rotation.w = dot + 1;
    //rotation.v = rotAxis;
    //rotation = rotation.Normalised();

}
