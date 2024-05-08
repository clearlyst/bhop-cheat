#include "../hooks.hpp"
#include "../../features/visuals/visuals.hpp"
#include "../../features/movement/movement.hpp"
#include "../../features/misc/misc.hpp"
#include "../../features/notifications/notifications.hpp"

void __stdcall sdk::hooks::paint_traverse::paint_traverse(unsigned int panel, bool force_repaint, bool allow_force) {
	auto panel_to_draw = fnv::hash(interfaces::panel->get_panel_name(panel));

	if (c::visuals::scope::enable && strstr(interfaces::panel->get_panel_name(panel), "HudZoom"))
	{
		return;
	}

	switch (panel_to_draw) {
	case fnv::hash("MatSystemTopPanel"):

		break;
	case fnv::hash("FocusOverlayPanel"):

		if (interfaces::engine->is_in_game() && interfaces::engine->is_connected() && g::local) {
			for (int i = 0; i < 4; i++)
				for (int j = 0; j < 4; j++)
					math::view_matrix[i][j] = interfaces::engine->world_to_screen_matrix()[i][j];
		}

		im_render.add_to_render([]() {
			features::notification::draw();
			features::misc::spread_circle();
			features::misc::sniper_crosshair();
			features::misc::spectators_list();
			features::misc::big_scene_indicators();
			features::misc::hitmarker::draw();
			features::visuals::player::run();
			features::visuals::draw_scope();
			features::grenade_prediction::draw();
			features::movement::indicators();
			features::movement::keys_indicator();
			features::movement::stamina_indicator();
			features::movement::velocity_indicator();
			features::movement::stamina_graph_indicator();
			features::movement::velocity_graph_indicator();
		});

		break;
	}

	ofunc(interfaces::panel, panel, force_repaint, allow_force);
}