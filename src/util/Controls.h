#pragma once

#include <imgui.h>

bool SliderProjection(const char* label, float* v, float v_min, float v_max, const char* format = "%.3f", ImGuiSliderFlags flags = 0);