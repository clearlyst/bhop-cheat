#include "color.hpp"
#include "../../includes/imgui/imgui_impl_dx9.h"

ImVec4 color_t::get_imgui_color(const color_t& color) {
	return ImVec4{ color.r(), color.g(), color.b(), color.a() };
}

color_t color_t::get_color_edit_alpha(const float amount) const {
	color_t clr = *this;
	clr.at(3) = amount;
	return clr;
}

color_t color_t::get_color_edit_alpha_int(const int amount) const {
	color_t clr = *this;
	clr.at(3) = amount / 255.0f;
	return clr;
}

ImU32 color_t::U32(const color_t& color)
{
	return ImGui::GetColorU32(color_t::get_imgui_color(color));
}
