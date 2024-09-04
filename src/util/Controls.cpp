#include "Controls.h"

#define _USE_MATH_DEFINES
#include <math.h>

bool SliderProjection(const char* label, float* proj, float v_min, float v_max, const char* format, ImGuiSliderFlags flags)
{
    float fov = atanf(287.39136f / *proj) * 2.f;
    bool value_changed = ImGui::SliderFloat(label, &fov, v_min, v_max, format, flags);
    *proj = 287.39136f / tanf(fov * 0.5f);
    return value_changed;
}