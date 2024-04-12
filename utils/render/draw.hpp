#pragma once
#include <any>
#include <deque>
#include <mutex>
#include <functional>
#include <shared_mutex>
#include "structures.hpp"
#include "../../menu/menu.hpp"
#include "../includes/imgui/imgui_impl_dx9.h"
#include "../includes/imgui/imgui_impl_win32.h"
#include "../includes/imgui/misc/freetype/imgui_freetype.h"
#include "../../sdk/math/vec3.hpp"

class imgui_render {
public:
	void setup();
	void setup_font();
	void reload_setup_font();
	void setup_menu_style();

	void unload();
	bool world_to_screen(const vec3_t& origin, ImVec2* screen);

	void drawline(const float x, const float y, const float x2, const float y2, const color_t& color, const float thickness = 1.0f);
	void text(const float x, const float y, const float fontsize, ImFont* font, const std::string& text, const bool centered, const color_t& color, const bool dropshadow = true, const bool outline = false);
	void drawrectfilled(const float x, const float y, const float w, const float h, const color_t& color, const ImDrawFlags flags = 0);
	void drawrectfilledgradient(const float x, const float y, const float w, const float h, const color_t& colUprLeft, const color_t& colUprRight, const color_t& colBotRight, const color_t& colBotLeft);
	void drawrect(const float x, const float y, const float w, const float h, const color_t& color, const ImDrawFlags flags = 0, const float thickness = 1.0f);
	void drawcornerbox(const float x, const float y, const float w, const float h, const float lenght, const color_t& colour);
	void drawcircle(const float x, const float y, const float radius, const int points, const color_t& color, const float thickness = 1.0f);
	void drawtrianglefilled(const ImVec2& p1, const ImVec2& p2, const ImVec2& p3, const color_t& color);
	void drawpolygon(const std::vector<ImVec2>& verts, const color_t& color);
	void drawpolyline(const std::vector<ImVec2>& verts, const color_t& color, const ImDrawFlags flags = ImDrawListFlags_AntiAliasedFill, const float thickness = 1.0f);
	int  get_text_size(std::string_view text, ImFont* font, float wrap_width = 0.0f, float font_size = -1.0f);
	void circle_filled_3d(const vec3_t& origin, float radius, const color_t& color);
	ImVec2 measure_text(std::string_view text, ImFont* font, float font_size = -1.0f);

	// add to present
	void render_present(ImDrawList* draw);

	// add to surface
	void add_to_render(const std::function<void()>& fun);
private:
	void clear_data();
	void swawp_data();

	std::deque<std::unique_ptr<drawing::Draw>> m_draw_data;
	std::deque<std::unique_ptr<drawing::Draw>> m_draw_data_safe;
	std::shared_mutex m_mutex;
};

inline imgui_render im_render;

namespace fonts {
	void reset_fonts();

	inline ImFont* menu_font_thin;
	inline ImFont* menu_font_bold;
	inline ImFont* scene_font;
	inline ImFont* sub_scene_font;
	inline ImFont* indicator_font;
	inline ImFont* sub_indicator_font;
	inline ImFont* esp_font;
	inline ImFont* sub_esp_font;
	inline ImFont* icon_font;

	inline std::string font_directory_scene = "C:/windows/fonts/tahomabd.ttf";
	inline std::string font_directory_sub_scene = "C:/windows/fonts/tahomabd.ttf";
	inline std::string font_directory_indicator = "C:/windows/fonts/tahomabd.ttf";
	inline std::string font_directory_sub_indicator = "C:/windows/fonts/tahomabd.ttf";
	inline std::string font_directory_esp = "C:/windows/fonts/tahoma.ttf";
	inline std::string font_directory_sub_esp = "C:/windows/fonts/tahoma.ttf";

	inline int selected_font_index_main_scene = -1;
	inline int selected_font_index_sub_scene = -1;
	inline int selected_font_index_main_indi = -1;
	inline int selected_font_index_sub_indi = -1;
	inline int selected_font_index_main_esp = -1;
	inline int selected_font_index_sub_esp = -1;
}