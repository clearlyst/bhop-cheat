#include "../visuals/visuals.hpp"
#include "../../menu/config/config.hpp"
#include <algorithm>

bool features::visuals::get_player_box(player_t* entity, bbox_t& in)
{
	if (!entity)
	{
		return false;
	}

	auto trans = entity->coordinate_frame();

	auto collideable = entity->collideable();

	if (!collideable)
	{
		return false;
	}

	vec3_t min = collideable->mins();
	vec3_t max = collideable->maxs();

	vec3_t points[] = 
	{ 
		vec3_t(min.x, min.y, min.z),
		vec3_t(min.x, max.y, min.z),
		vec3_t(max.x, max.y, min.z),
		vec3_t(max.x, min.y, min.z),
		vec3_t(max.x, max.y, max.z),
		vec3_t(min.x, max.y, max.z),
		vec3_t(min.x, min.y, max.z),
		vec3_t(max.x, min.y, max.z)
	};

	vec3_t points_transformed[8];

	for (int i = 0; i < 8; i++) 
	{
		math::transform_vector(points[i], trans, points_transformed[i]);
	}

	vec3_t pos = entity->abs_origin(), flb, brt, blb, frt, frb, brb, blt, flt;

	if (!interfaces::debug_overlay->world_to_screen(points_transformed[3], flb) ||
		!interfaces::debug_overlay->world_to_screen(points_transformed[5], brt) ||
		!interfaces::debug_overlay->world_to_screen(points_transformed[0], blb) ||
		!interfaces::debug_overlay->world_to_screen(points_transformed[4], frt) ||
		!interfaces::debug_overlay->world_to_screen(points_transformed[2], frb) ||
		!interfaces::debug_overlay->world_to_screen(points_transformed[1], brb) ||
		!interfaces::debug_overlay->world_to_screen(points_transformed[6], blt) ||
		!interfaces::debug_overlay->world_to_screen(points_transformed[7], flt))
	{
		return false;
	}

	vec3_t arr[] = { flb, brt, blb, frt, frb, brb, blt, flt };

	float left = flb.x;
	float top = flb.y;
	float right = flb.x;
	float bottom = flb.y;

	for (int i = 1; i < 8; i++)
	{
		if (left > arr[i].x)
		{
			left = arr[i].x;
		}

		if (bottom < arr[i].y)
		{
			bottom = arr[i].y;
		}

		if (right < arr[i].x)
		{
			right = arr[i].x;
		}

		if (top > arr[i].y)
		{
			top = arr[i].y;
		}
	}

	in.x = static_cast<int>(left);
	in.y = static_cast<int>(top);
	in.w = static_cast<int>(right) - static_cast<int>(left);
	in.h = static_cast<int>(bottom) - static_cast<int>(top);

	return true;
}

bool features::visuals::get_entity_box(entity_t* entity, bbox_t& in)
{
	if (!entity)
	{
		return false;
	}

	vec3_t min, max, flb, brt, blb, frt, frb, brb, blt, flt;

	auto collideable = entity->collideable();

	if (!collideable)
	{
		return false;
	}

	min = collideable->mins();
	max = collideable->maxs();

	matrix_t trans = entity->coordinate_frame();

	vec3_t points[] =
	{
		vec3_t(min.x, min.y, min.z),
		vec3_t(min.x, max.y, min.z),
		vec3_t(max.x, max.y, min.z),
		vec3_t(max.x, min.y, min.z),
		vec3_t(max.x, max.y, max.z),
		vec3_t(min.x, max.y, max.z),
		vec3_t(min.x, min.y, max.z),
		vec3_t(max.x, min.y, max.z)
	};

	vec3_t points_transformed[8];

	for (int i = 0; i < 8; i++)
	{
		math::transform_vector(points[i], trans, points_transformed[i]);
	}


	if (!interfaces::debug_overlay->world_to_screen(points_transformed[3], flb) ||
		!interfaces::debug_overlay->world_to_screen(points_transformed[5], brt) ||
		!interfaces::debug_overlay->world_to_screen(points_transformed[0], blb) ||
		!interfaces::debug_overlay->world_to_screen(points_transformed[4], frt) ||
		!interfaces::debug_overlay->world_to_screen(points_transformed[2], frb) ||
		!interfaces::debug_overlay->world_to_screen(points_transformed[1], brb) ||
		!interfaces::debug_overlay->world_to_screen(points_transformed[6], blt) ||
		!interfaces::debug_overlay->world_to_screen(points_transformed[7], flt))
	{
		return false;
	}

	vec3_t arr[] = { flb, brt, blb, frt, frb, brb, blt, flt };

	float left = flb.x;
	float top = flb.y;
	float right = flb.x;
	float bottom = flb.y;

	for (int i = 1; i < 8; i++)
	{
		if (left > arr[i].x)
		{
			left = arr[i].x;
		}

		if (bottom < arr[i].y)
		{
			bottom = arr[i].y;
		}

		if (right < arr[i].x)
		{
			right = arr[i].x;
		}

		if (top > arr[i].y)
		{
			top = arr[i].y;
		}
	}

	in.x = static_cast<int>(left);
	in.y = static_cast<int>(top);
	in.w = static_cast<int>(right) - static_cast<int>(left);
	in.h = static_cast<int>(bottom) - static_cast<int>(top);

	return true;
}

void features::visuals::player::run() 
{
	if (!c::visuals::enable || !menu::checkkey(c::visuals::key, c::visuals::key_s))
	{
		return;
	}

	if (!interfaces::engine->is_connected() || !interfaces::engine->is_in_game())
	{
		return;
	}

	for (int i = 1; i <= interfaces::globals->max_clients; i++) 
	{
		auto entity = reinterpret_cast<player_t*>(interfaces::ent_list->get_client_entity(i));

		if (!entity || !entity->is_player() || entity == g::local)
		{
			continue;
		}

		const int index = entity->index();

		if (c::visuals::players::fade::enable)
		{
			if (entity->dormant() || !entity->is_alive())
				player_alpha[index] = max(player_alpha[index] - interfaces::globals->frame_time * c::visuals::players::fade::time, 0.f);
			else
				player_alpha[index] = min(player_alpha[index] + interfaces::globals->frame_time * c::visuals::players::fade::time, 1.f);

			/*
			
			entity->dormant() || !entity->is_alive() ? player_alpha[index] -= interfaces::globals->frame_time * c::visuals::players::fade::time : player_alpha[index] += interfaces::globals->frame_time * c::visuals::players::fade::time;

			player_alpha[index] = std::clamp(player_alpha[index], 0.f, 1.f);

			if (player_alpha[index] < 0.1f)
			{
				continue;
			}

			*/
		}
		else
		{
			if (!entity->is_alive() || entity->dormant())
			{
				continue;
			}
		}

		bool check_on_see = g::local->can_see_player_pos(entity, entity->get_eye_pos());

		color_t visible = c::visuals::players::fade::enable ? color_t(c::visuals::players::colors::visible[0], c::visuals::players::colors::visible[1], c::visuals::players::colors::visible[2], float(player_alpha[index] * c::visuals::players::colors::visible[3])) : color_t(c::visuals::players::colors::visible[0], c::visuals::players::colors::visible[1], c::visuals::players::colors::visible[2], c::visuals::players::colors::visible[3]);
		color_t invisible = c::visuals::players::fade::enable ? color_t(c::visuals::players::colors::invisible[0], c::visuals::players::colors::invisible[1], c::visuals::players::colors::invisible[2], float(player_alpha[index] * c::visuals::players::colors::invisible[3])) : color_t(c::visuals::players::colors::invisible[0], c::visuals::players::colors::invisible[1], c::visuals::players::colors::invisible[2], c::visuals::players::colors::invisible[3]);
		color_t name = c::visuals::players::fade::enable ? color_t(c::visuals::players::colors::name[0], c::visuals::players::colors::name[1], c::visuals::players::colors::name[2], float(player_alpha[index] * c::visuals::players::colors::name[3])) : color_t(c::visuals::players::colors::name[0], c::visuals::players::colors::name[1], c::visuals::players::colors::name[2], c::visuals::players::colors::name[3]);
		color_t box = c::visuals::players::fade::enable ? color_t(c::visuals::players::colors::box[0], c::visuals::players::colors::box[1], c::visuals::players::colors::box[2], float(player_alpha[index] * c::visuals::players::colors::box[3])) : color_t(c::visuals::players::colors::box[0], c::visuals::players::colors::box[1], c::visuals::players::colors::box[2], c::visuals::players::colors::box[3]);
		color_t box_outline = c::visuals::players::fade::enable ? color_t(c::visuals::players::colors::box_outline[0], c::visuals::players::colors::box_outline[1], c::visuals::players::colors::box_outline[2], float(player_alpha[index] * c::visuals::players::colors::box_outline[3])) : color_t(c::visuals::players::colors::box_outline[0], c::visuals::players::colors::box_outline[1], c::visuals::players::colors::box_outline[2], c::visuals::players::colors::box_outline[3]);
		color_t health_bar = c::visuals::players::fade::enable ? color_t(c::visuals::players::colors::health_bar[0], c::visuals::players::colors::health_bar[1], c::visuals::players::colors::health_bar[2], float(player_alpha[index] * c::visuals::players::colors::health_bar[3])) : color_t(c::visuals::players::colors::health_bar[0], c::visuals::players::colors::health_bar[1], c::visuals::players::colors::health_bar[2], c::visuals::players::colors::health_bar[3]);
		color_t health_bar_upper = c::visuals::players::fade::enable ? color_t(c::visuals::players::colors::health_bar_upper[0], c::visuals::players::colors::health_bar_upper[1], c::visuals::players::colors::health_bar_upper[2], float(player_alpha[index] * c::visuals::players::colors::health_bar_upper[3])) : color_t(c::visuals::players::colors::health_bar_upper[0], c::visuals::players::colors::health_bar_upper[1], c::visuals::players::colors::health_bar_upper[2], c::visuals::players::colors::health_bar_upper[3]);
		color_t health_bar_lower = c::visuals::players::fade::enable ? color_t(c::visuals::players::colors::health_bar_lower[0], c::visuals::players::colors::health_bar_lower[1], c::visuals::players::colors::health_bar_lower[2], float(player_alpha[index] * c::visuals::players::colors::health_bar_lower[3])) : color_t(c::visuals::players::colors::health_bar_lower[0], c::visuals::players::colors::health_bar_lower[1], c::visuals::players::colors::health_bar_lower[2], c::visuals::players::colors::health_bar_lower[3]);
		color_t health_bar_outline = c::visuals::players::fade::enable ? color_t(c::visuals::players::colors::health_bar_outline[0], c::visuals::players::colors::health_bar_outline[1], c::visuals::players::colors::health_bar_outline[2], float(player_alpha[index] * c::visuals::players::colors::health_bar_outline[3])) : color_t(c::visuals::players::colors::health_bar_outline[0], c::visuals::players::colors::health_bar_outline[1], c::visuals::players::colors::health_bar_outline[2], c::visuals::players::colors::health_bar_outline[3]);
		color_t skeleton = c::visuals::players::fade::enable ? color_t(c::visuals::players::colors::skeleton[0], c::visuals::players::colors::skeleton[1], c::visuals::players::colors::skeleton[2], float(player_alpha[index] * c::visuals::players::colors::skeleton[3])) : color_t(c::visuals::players::colors::skeleton[0], c::visuals::players::colors::skeleton[1], c::visuals::players::colors::skeleton[2], c::visuals::players::colors::skeleton[3]);
		color_t backtrack_skeleton = c::visuals::players::fade::enable ? color_t(c::visuals::players::colors::backtrack_skeleton[0], c::visuals::players::colors::backtrack_skeleton[1], c::visuals::players::colors::backtrack_skeleton[2], float(player_alpha[index] * c::visuals::players::colors::backtrack_skeleton[3])) : color_t(c::visuals::players::colors::backtrack_skeleton[0], c::visuals::players::colors::backtrack_skeleton[1], c::visuals::players::colors::backtrack_skeleton[2], c::visuals::players::colors::backtrack_skeleton[3]);
		color_t backtrack_dot = c::visuals::players::fade::enable ? color_t(c::visuals::players::colors::backtrack_dot[0], c::visuals::players::colors::backtrack_dot[1], c::visuals::players::colors::backtrack_dot[2], float(player_alpha[index] * c::visuals::players::colors::backtrack_dot[3])) : color_t(c::visuals::players::colors::backtrack_dot[0], c::visuals::players::colors::backtrack_dot[1], c::visuals::players::colors::backtrack_dot[2], c::visuals::players::colors::backtrack_dot[3]);
		color_t view_angle = c::visuals::players::fade::enable ? color_t(c::visuals::players::colors::view_angle[0], c::visuals::players::colors::view_angle[1], c::visuals::players::colors::view_angle[2], float(player_alpha[index] * c::visuals::players::colors::view_angle[3])) : color_t(c::visuals::players::colors::view_angle[0], c::visuals::players::colors::view_angle[1], c::visuals::players::colors::view_angle[2], c::visuals::players::colors::view_angle[3]);
		color_t snap_line = c::visuals::players::fade::enable ? color_t(c::visuals::players::colors::snapline[0], c::visuals::players::colors::snapline[1], c::visuals::players::colors::snapline[2], float(player_alpha[index] * c::visuals::players::colors::snapline[3])) : color_t(c::visuals::players::colors::snapline[0], c::visuals::players::colors::snapline[1], c::visuals::players::colors::snapline[2], c::visuals::players::colors::snapline[3]);
		color_t ammo_bar = c::visuals::players::fade::enable ? color_t(c::visuals::players::colors::ammo_bar[0], c::visuals::players::colors::ammo_bar[1], c::visuals::players::colors::ammo_bar[2], float(player_alpha[index] * c::visuals::players::colors::ammo_bar[3])) : color_t(c::visuals::players::colors::ammo_bar[0], c::visuals::players::colors::ammo_bar[1], c::visuals::players::colors::ammo_bar[2], c::visuals::players::colors::ammo_bar[3]);
		color_t ammo_bar_outline = c::visuals::players::fade::enable ? color_t(c::visuals::players::colors::ammo_bar_outline[0], c::visuals::players::colors::ammo_bar_outline[1], c::visuals::players::colors::ammo_bar_outline[2], float(player_alpha[index] * c::visuals::players::colors::ammo_bar_outline[3])) : color_t(c::visuals::players::colors::ammo_bar_outline[0], c::visuals::players::colors::ammo_bar_outline[1], c::visuals::players::colors::ammo_bar_outline[2], c::visuals::players::colors::ammo_bar_outline[3]);
		color_t weapon_text = c::visuals::players::fade::enable ? color_t(c::visuals::players::colors::weapon_text[0], c::visuals::players::colors::weapon_text[1], c::visuals::players::colors::weapon_text[2], float(player_alpha[index] * c::visuals::players::colors::weapon_text[3])) : color_t(c::visuals::players::colors::weapon_text[0], c::visuals::players::colors::weapon_text[1], c::visuals::players::colors::weapon_text[2], c::visuals::players::colors::weapon_text[3]);
		color_t weapon_icon = c::visuals::players::fade::enable ? color_t(c::visuals::players::colors::weapon_icon[0], c::visuals::players::colors::weapon_icon[1], c::visuals::players::colors::weapon_icon[2], float(player_alpha[index] * c::visuals::players::colors::weapon_icon[3])) : color_t(c::visuals::players::colors::weapon_icon[0], c::visuals::players::colors::weapon_icon[1], c::visuals::players::colors::weapon_icon[2], c::visuals::players::colors::weapon_icon[3]);
		color_t distance = c::visuals::players::fade::enable ? color_t(c::visuals::players::colors::distance[0], c::visuals::players::colors::distance[1], c::visuals::players::colors::distance[2], float(player_alpha[index] * c::visuals::players::colors::distance[3])) : color_t(c::visuals::players::colors::distance[0], c::visuals::players::colors::distance[1], c::visuals::players::colors::distance[2], c::visuals::players::colors::distance[3]);
		color_t health_text = c::visuals::players::fade::enable ? color_t(c::visuals::players::colors::health_text[0], c::visuals::players::colors::health_text[1], c::visuals::players::colors::health_text[2], float(player_alpha[index] * c::visuals::players::colors::health_text[3])) : color_t(c::visuals::players::colors::health_text[0], c::visuals::players::colors::health_text[1], c::visuals::players::colors::health_text[2], c::visuals::players::colors::health_text[3]);
		color_t ammo_text = c::visuals::players::fade::enable ? color_t(c::visuals::players::colors::ammo_text[0], c::visuals::players::colors::ammo_text[1], c::visuals::players::colors::ammo_text[2], float(player_alpha[index] * c::visuals::players::colors::ammo_text[3])) : color_t(c::visuals::players::colors::ammo_text[0], c::visuals::players::colors::ammo_text[1], c::visuals::players::colors::ammo_text[2], c::visuals::players::colors::ammo_text[3]);
		color_t out_of_view = c::visuals::players::fade::enable ? color_t(c::visuals::players::colors::out_of_view[0], c::visuals::players::colors::out_of_view[1], c::visuals::players::colors::out_of_view[2], float(player_alpha[index] * c::visuals::players::colors::out_of_view[3])) : color_t(c::visuals::players::colors::out_of_view[0], c::visuals::players::colors::out_of_view[1], c::visuals::players::colors::out_of_view[2], c::visuals::players::colors::out_of_view[3]);

		if (entity->team() == g::local->team())
		{
			continue;
		}

		if (c::visuals::players::engine_radar)
		{
			entity->spotted() = true;
		}

		if (c::visuals::players::out_of_view::enable)
		{
			draw_outoffov(entity, c::visuals::players::colors::custom ? out_of_view : check_on_see ? visible : invisible);
		}

		if (c::visuals::players::emitted_sound::enable)
		{
			draw_sounds();
		}

		bbox_t _box;

		if (!get_player_box(entity, _box))
		{
			continue;
		}

		if (c::visuals::players::name::enable)
		{
			draw_name(entity, _box, c::visuals::players::colors::custom ? name : check_on_see ? visible : invisible);
		}

		if (c::visuals::players::box::enable)
		{
			draw_box(entity, _box, c::visuals::players::colors::custom ? box : check_on_see ? visible : invisible, box_outline);
		}

		if (c::visuals::players::health_bar::enable)
		{
			draw_health_bar(entity, _box, c::visuals::players::colors::custom ? health_bar : check_on_see ? visible : invisible, health_bar_upper, health_bar_lower, health_bar_outline);
		}

		if (c::visuals::players::skeleton::enable)
		{
			draw_skeleton(entity, c::visuals::players::colors::custom ? skeleton : check_on_see ? visible : invisible);
		}

		if (c::visuals::players::backtrack_skeleton::enable)
		{
			draw_backtrack_skeleton(entity, c::visuals::players::colors::custom ? backtrack_skeleton : check_on_see ? visible : invisible);
		}

		if (c::visuals::players::backtrack_dot::enable)
		{
			draw_backtrack_dot(entity, c::visuals::players::colors::custom ? backtrack_dot : check_on_see ? visible : invisible);
		}

		if (c::visuals::players::view_angle::enable)
		{
			draw_viewangle(entity, c::visuals::players::colors::custom ? view_angle : check_on_see ? visible : invisible);
		}

		if (c::visuals::players::snapline::enable)
		{
			draw_snapline(entity, _box, c::visuals::players::colors::custom ? snap_line : check_on_see ? visible : invisible);
		}

		if (c::visuals::players::flags::enable)
		{
			draw_flags(entity, _box, c::visuals::players::colors::custom ? snap_line : check_on_see ? visible : invisible);
		}

		draw_bottom_bar(entity, _box, c::visuals::players::colors::custom ? ammo_bar : check_on_see ? visible : invisible, c::visuals::players::colors::custom ? health_text : check_on_see ? visible : invisible, c::visuals::players::colors::custom ? weapon_icon : check_on_see ? visible : invisible, c::visuals::players::colors::custom ? weapon_text : check_on_see ? visible : invisible, c::visuals::players::colors::custom ? ammo_text : check_on_see ? visible : invisible, c::visuals::players::colors::custom ? distance : check_on_see ? visible : invisible, ammo_bar_outline);

		if (c::visuals::players::dlight::enable)
		{
			dlights(entity);
		}
	}

	for (int i = 1; i < interfaces::ent_list->get_highest_index(); i++)
	{
		auto entity = reinterpret_cast<entity_t*>(interfaces::ent_list->get_client_entity(i));

		if (!entity || entity->is_player() || entity->dormant() || entity == g::local)
		{
			continue;
		}

		if (c::visuals::players::thrown_grenade::text::enable || c::visuals::players::thrown_grenade::icon::enable)
		{
			throwed_grenade(entity);
		}

		if (c::visuals::players::dropped_weapon::box::enable ||
			c::visuals::players::dropped_weapon::ammo_bar::enable ||
			c::visuals::players::dropped_weapon::ammo_text::enable ||
			c::visuals::players::dropped_weapon::text::enable ||
			c::visuals::players::dropped_weapon::icon::enable)
		{

			bbox_t entity_box;

			if (!get_entity_box(entity, entity_box))
			{
				continue;
			}

			dropped_weapon(entity, entity_box);
		}
	}
}

void features::visuals::player::draw_name(player_t* entity, bbox_t bbox, color_t color)
{
	player_info_t info;
	interfaces::engine->get_player_info(entity->index(), &info);

	std::string print((c::visuals::players::name::bot_check && info.fakeplayer) ? std::string("[bot] ").append(info.name).c_str() : info.name);

	ImVec2 size = im_render.measure_text(print, fonts::esp_font, c::fonts::esp_size);

	im_render.text(bbox.x + (bbox.w / 2), bbox.y - static_cast<int>(size.y) - 2, c::fonts::esp_size, fonts::esp_font, print, true, color, c::fonts::esp_flag[9], c::fonts::esp_flag[10]);
}

void features::visuals::player::draw_box(player_t* entity, bbox_t bbox, color_t color, color_t color_second)
{
	switch (c::visuals::players::box::type)
	{
	case 0:
		if (c::visuals::players::box::outline[0])
		{
			im_render.drawrect(bbox.x - 1, bbox.y - 1, bbox.w + 2, bbox.h + 2, color_second);
		}

		if (c::visuals::players::box::outline[1])
		{
			im_render.drawrect(bbox.x + 1, bbox.y + 1, bbox.w - 2, bbox.h - 2, color_second);
		}

		im_render.drawrect(bbox.x, bbox.y, bbox.w, bbox.h, color);

		break;
	case 1:
		if (c::visuals::players::box::outline[0])
		{
			im_render.drawcornerbox(bbox.x - 1, bbox.y - 1, bbox.w + 2, bbox.h + 2, c::visuals::players::box::lenght, color_second);
		}

		if (c::visuals::players::box::outline[1])
		{
			im_render.drawcornerbox(bbox.x + 1, bbox.y + 1, bbox.w - 2, bbox.h - 2, c::visuals::players::box::lenght, color_second);
		}

		im_render.drawcornerbox(bbox.x, bbox.y, bbox.w, bbox.h, c::visuals::players::box::lenght, color);

	    break;
	}
}

constexpr float anim_time = 255 / 2.0f;

void features::visuals::player::draw_health_bar(player_t* entity, bbox_t bbox, color_t color, color_t upper, color_t lower, color_t outline)
{
	static float prev_player_hp[65];

	if (prev_player_hp[entity->index()] > entity->health())
	{
		prev_player_hp[entity->index()] -= anim_time * interfaces::globals->frame_time;
	}
	else
	{
		prev_player_hp[entity->index()] = entity->health();
	}

	int g = static_cast<int>(entity->health() * 2.55f);
	int r = 255 - g;

	color_t rgb_health_color = color_t(r, g, 0, 255);

	if (c::visuals::players::health_bar::background)
	{
		im_render.drawrectfilled(bbox.x - 2, bbox.y - 1, (-c::visuals::players::health_bar::size - 2), bbox.h + 2, outline);
	}
	else
	{
		im_render.drawrect(bbox.x - 3, bbox.y - 1, (-c::visuals::players::health_bar::size), bbox.h + 2, outline);
	}


	if (!c::visuals::players::health_bar::baseonhealth && !c::visuals::players::health_bar::gradient)
	{
		im_render.drawrectfilled(
			bbox.x - 3,
			bbox.y + (bbox.h - bbox.h * (prev_player_hp[entity->index()]) / 100.f),
			(-c::visuals::players::health_bar::size),
			(bbox.h * (prev_player_hp[entity->index()]) / 100.f),
			color);
	}

	if (c::visuals::players::health_bar::baseonhealth)
	{
		im_render.drawrectfilled(
			bbox.x - 3,
			bbox.y + (bbox.h - bbox.h * (prev_player_hp[entity->index()]) / 100.f),
			(-c::visuals::players::health_bar::size),
			(bbox.h * (prev_player_hp[entity->index()]) / 100.f),
			rgb_health_color);
	}

	if (c::visuals::players::health_bar::gradient)
	{
		im_render.drawrectfilledgradient(
			bbox.x - 3,
			bbox.y + (bbox.h - bbox.h * (prev_player_hp[entity->index()]) / 100.f),
			(-c::visuals::players::health_bar::size),
			(bbox.h * (prev_player_hp[entity->index()]) / 100.f),
			upper, upper, lower, lower);
	}

	if (c::visuals::players::health_bar::text && entity->health() < 99)
	{	
		ImVec2 size = im_render.measure_text(std::to_string(entity->health()), fonts::sub_esp_font, c::fonts::esp_sub_size);

		im_render.text(bbox.x - (size.x + c::visuals::players::health_bar::size), bbox.y, c::fonts::esp_sub_size, fonts::sub_esp_font, std::to_string(static_cast<int>(prev_player_hp[entity->index()])), true, color_t(1.0f, 1.0f, 1.0f, color[3]), c::fonts::esp_sub_flag[9], c::fonts::esp_sub_flag[10]);
	 }
}

void features::visuals::player::draw_backtrack_dot(player_t* entity, color_t color)
{
	if (backtrack.data[entity->index()].size() < 1)
	{
		return;
	}

	vec3_t previous_screenpos;

	for (int i = 1; i < backtrack.data[entity->index()].size(); i++)
	{
		vec3_t screen_pos;

		auto head = backtrack.data[entity->index()][i].m_headpos;

		if (!interfaces::debug_overlay->world_to_screen(head, screen_pos))
		{
			continue;
		}

		if (previous_screenpos.is_valid() && !previous_screenpos.is_zero())
		{

			im_render.drawrect(screen_pos.x, screen_pos.y, 2, 2, color, 0.1f);

		} 

		previous_screenpos = screen_pos;
	}
}

void features::visuals::player::draw_backtrack_skeleton(player_t* entity, color_t color)
{
	auto hdr = interfaces::model_info->get_studio_model(entity->model());

	if (!hdr)
	{
		return;
	}

	if (backtrack.data[entity->index()].size() < 1)
	{
		return;
	}

	auto record = &backtrack.data[entity->index()].back();

	vec3_t v_parent, v_child, s_parent, s_child;

	for (int j = 0; j < hdr->bones_count; j++) 
	{
		studio_bone_t* bone = hdr->bone(j);

		if (!bone)
		{
			continue;
		}

		if ((bone->flags & BONE_USED_BY_HITBOX) && (bone->parent != -1)) 
		{
			v_child = entity->get_bone_position(j);
			v_parent = entity->get_bone_position(bone->parent);

			v_child = vec3_t(record->m_matrix[j][0][3], record->m_matrix[j][1][3], record->m_matrix[j][2][3]);
			v_parent = vec3_t(record->m_matrix[bone->parent][0][3], record->m_matrix[bone->parent][1][3], record->m_matrix[bone->parent][2][3]);

			if (interfaces::debug_overlay->world_to_screen(v_parent, s_parent) && interfaces::debug_overlay->world_to_screen(v_child, s_child))
			{
				im_render.drawline(s_parent[0], s_parent[1], s_child[0], s_child[1], color, c::visuals::players::backtrack_skeleton::thickness);
			}
		}
	}
}

void features::visuals::player::draw_skeleton(player_t* entity, color_t color)
{
	auto p_studio_hdr = interfaces::model_info->get_studio_model(entity->model());

	if (!p_studio_hdr)
	{
		return;
	}

	vec3_t v_parent, v_child, s_parent, s_child;

	for (int i = 0; i < p_studio_hdr->bones_count; i++)
	{
		studio_bone_t* bone = p_studio_hdr->bone(i);

		if (!bone)
		{
			continue;
		}

		if (bone && (bone->flags & BONE_USED_BY_HITBOX) && (bone->parent != -1)) 
		{
			v_child = entity->get_bone_position(i);
			v_parent = entity->get_bone_position(bone->parent);

			if (interfaces::debug_overlay->world_to_screen(v_parent, s_parent) && interfaces::debug_overlay->world_to_screen(v_child, s_child))
			{
				im_render.drawline(s_parent[0], s_parent[1], s_child[0], s_child[1], color, c::visuals::players::skeleton::thickness);
			}
		}
	}
}

void features::visuals::player::draw_snapline(player_t* entity, bbox_t bbox, color_t color)
{
	if (c::visuals::players::snapline::type[0])
	{
		im_render.drawline(bbox.x + bbox.w / 2, bbox.y, g::width / 2.f, 0, color, c::visuals::players::snapline::thickness);
	}

	if (c::visuals::players::snapline::type[1])
	{
		im_render.drawline(bbox.x + bbox.w / 2, bbox.y + bbox.h / 2, g::width / 2.0f, g::height / 2.0f, color, c::visuals::players::snapline::thickness);
	}

	if (c::visuals::players::snapline::type[2])
	{
		im_render.drawline(bbox.x + bbox.w / 2, bbox.y + bbox.h, g::width / 2.0f, g::height, color, c::visuals::players::snapline::thickness);
	}
}

void features::visuals::player::draw_viewangle(player_t* entity, color_t color)
{
	vec3_t src;
	vec3_t pos = entity->get_bone_position(8);
	math::angle_vectors(entity->eye_angles(), src);
	src *= c::visuals::players::view_angle::distance;
	vec3_t dst = pos + src;
	vec3_t start, end;

	if (interfaces::debug_overlay->world_to_screen(pos, start) && interfaces::debug_overlay->world_to_screen(dst, end))
	{
		im_render.drawline(start.x, start.y, end.x, end.y, color, c::visuals::players::view_angle::thickness);
	}
}

void features::visuals::player::draw_flags(player_t* entity, bbox_t bbox, color_t color)
{
	std::vector<std::pair<std::string, color_t>> flags;

	if (c::visuals::players::flags::flags[0] && entity->armor() > 0)
	{
		flags.push_back({ entity->has_helmet() ? "hk" : "k", color });
	}

	if (c::visuals::players::flags::flags[1])
	{
		flags.push_back({ std::string(("$")).append(std::to_string(entity->money())), color });
	}

	if (c::visuals::players::flags::flags[2] && entity->is_flashed())
	{
		flags.push_back({ "flashed", color });
	}

	if (c::visuals::players::flags::flags[3] && entity->is_scoped())
	{
		flags.push_back({ "scoped", color });
	}

	if (c::visuals::players::flags::flags[4] && ((entity->get_sequence_activity(entity->get_animation_overlays_index(1)->m_nSequence) == 967) && (entity->get_animation_overlays_index(1)->m_flWeight != 0.f)))
	{
		flags.push_back({ "reloading...", color });
	}

	if (c::visuals::players::flags::flags[5] && entity->is_defusing())
	{
		flags.push_back({ "defusing...", color });
	}

	if (c::visuals::players::flags::flags[6] && entity->is_rescuing())
	{
		flags.push_back({ "rescuing...", color });
	}

	auto position = 0;

	for (auto text : flags) 
	{
		im_render.text(bbox.x + bbox.w + 3, bbox.y + position, c::fonts::esp_sub_size, fonts::sub_esp_font, text.first, false, text.second, c::fonts::esp_sub_flag[9], c::fonts::esp_sub_flag[10]);

		position += c::fonts::esp_sub_size;
	}
}

std::string u8toStr(const std::u8string& u8str)
{
	return std::string{ u8str.cbegin(), u8str.cend() };
}

void features::visuals::player::draw_bottom_bar(player_t* entity, bbox_t bbox, color_t ammo_bar, color_t health_text, color_t weapon_icon, color_t weapon_text, color_t ammo_text, color_t distance, color_t ammo_bar_outline)
{
	int offset_bottom = 0;

	auto weapon = entity->active_weapon();

	if (!weapon)
	{
		return;
	}


	int g = static_cast<int>(entity->health() * 2.55f);
	int r = 255 - g;

	color_t rgb_health_color = color_t(r, g, 0, 255);

	auto weapon_data = weapon->get_weapon_data();

	if (!weapon_data)
	{
		return;
	}

	if (c::visuals::players::ammo_bar::enable)
	{
		if (weapon->isgun())
		{
			float box_w = (float)fabs(bbox.w - bbox.x);

			int width = ((box_w * weapon->clip1_count()) / weapon_data->m_iMaxClip);

			if (c::visuals::players::ammo_bar::background)
			{
				im_render.drawrectfilled(bbox.x - 1, bbox.y + bbox.h + 2, bbox.w + 2, c::visuals::players::ammo_bar::size + 2, ammo_bar_outline);
			}
			else
			{
				im_render.drawrect(bbox.x - 1, bbox.y + bbox.h + 2, bbox.w + 2, c::visuals::players::ammo_bar::size + 2, ammo_bar_outline);
			}

			im_render.drawrectfilled(bbox.x, bbox.y + bbox.h + 3, bbox.w, c::visuals::players::ammo_bar::size, ammo_bar);

			offset_bottom += 3 + c::visuals::players::ammo_bar::size;
		}
	}

	if (c::visuals::players::health_text::enable)
	{
		if (c::visuals::players::health_text::hide_at_max)
		{
			if (entity->health() < 98)
			{
				if (c::visuals::players::health_text::baseonhealth)
				{
					im_render.text(bbox.x + (bbox.w / 2), bbox.y + bbox.h + 2 + offset_bottom, c::fonts::esp_sub_size, fonts::sub_esp_font, c::visuals::players::health_text::hide_suffix ? std::to_string(entity->health()) : std::to_string(entity->health()) + " hp", true, rgb_health_color, c::fonts::esp_sub_flag[9], c::fonts::esp_sub_flag[10]);
				}
				else
				{
					im_render.text(bbox.x + (bbox.w / 2), bbox.y + bbox.h + 2 + offset_bottom, c::fonts::esp_sub_size, fonts::sub_esp_font, c::visuals::players::health_text::hide_suffix ? std::to_string(entity->health()) : std::to_string(entity->health()) + " hp", true, health_text, c::fonts::esp_sub_flag[9], c::fonts::esp_sub_flag[10]);
				}

				offset_bottom += 4 + c::fonts::esp_sub_size;
			}
		}
		else
		{
			if (c::visuals::players::health_text::baseonhealth)
			{
				im_render.text(bbox.x + (bbox.w / 2), bbox.y + bbox.h + 2 + offset_bottom, c::fonts::esp_sub_size, fonts::sub_esp_font, c::visuals::players::health_text::hide_suffix ? std::to_string(entity->health()) : std::to_string(entity->health()) + " hp", true, rgb_health_color, c::fonts::esp_sub_flag[9], c::fonts::esp_sub_flag[10]);
			}
			else
			{
				im_render.text(bbox.x + (bbox.w / 2), bbox.y + bbox.h + 2 + offset_bottom, c::fonts::esp_sub_size, fonts::sub_esp_font, c::visuals::players::health_text::hide_suffix ? std::to_string(entity->health()) : std::to_string(entity->health()) + " hp", true, health_text, c::fonts::esp_sub_flag[9], c::fonts::esp_sub_flag[10]);
			}

			offset_bottom += 4 + c::fonts::esp_sub_size;
		}
	}

	if (c::visuals::players::weapon_icon::enable)
	{
		im_render.text(bbox.x + (bbox.w / 2), bbox.y + bbox.h + offset_bottom, 12.0f, fonts::icon_font, u8toStr(weapon->get_wpn_icon()), true, weapon_icon, c::fonts::esp_sub_flag[9], c::fonts::esp_sub_flag[10]);

		offset_bottom += 13;
	}

	if (c::visuals::players::weapon_text::enable)
	{
		im_render.text(bbox.x + (bbox.w / 2), bbox.y + bbox.h + offset_bottom, c::fonts::esp_sub_size, fonts::sub_esp_font, weapon->weapon_name_definition(), true, weapon_text, c::fonts::esp_sub_flag[9], c::fonts::esp_sub_flag[10]);

		offset_bottom += 2 + c::fonts::esp_sub_size;
	}

	if (c::visuals::players::ammo_text::enable)
	{
		if (weapon->isgun())
		{
			im_render.text(bbox.x + (bbox.w / 2), bbox.y + bbox.h + offset_bottom, c::fonts::esp_sub_size, fonts::sub_esp_font, "[" + std::to_string(weapon->clip1_count()) + "|" + std::to_string(weapon_data->m_iMaxClip) + "]", true, ammo_text, c::fonts::esp_sub_flag[9], c::fonts::esp_sub_flag[10]);

			offset_bottom += 2 + c::fonts::esp_sub_size;
		}
	}

	if (c::visuals::players::distance::enable)
	{
		im_render.text(bbox.x + (bbox.w / 2), bbox.y + bbox.h + offset_bottom, c::fonts::esp_sub_size, fonts::sub_esp_font, std::to_string((int)(g::local->origin().distance_to(entity->origin()) * 0.0254f)) + " m", true, distance, c::fonts::esp_sub_flag[9], c::fonts::esp_sub_flag[10]);
	}
}
	
void features::visuals::player::draw_outoffov(player_t* entity, color_t color)
{
	const vec2_t screen = vec2_t(g::width, g::height);
	const auto screen_center = vec2_t(screen.x / 2, screen.y / 2);

	vec3_t screen_point2;
	vec3_t player_pos = entity->origin();

	interfaces::debug_overlay->world_to_screen(player_pos, screen_point2);

	vec2_t screen_point = vec2_t(screen_point2.x, screen_point2.y);

	if (screen_point.x < 0 || screen_point.y < 0 || screen_point.x > screen.x || screen_point.y > screen.y)
	{
		const auto angle_yaw_rad = std::atan2f(screen_point.y - screen_center.y, screen_point.x - screen_center.x);
		auto angle = math::rad2deg(angle_yaw_rad);

		const auto new_point_x = screen_center.x + c::visuals::players::out_of_view::distance * cosf(angle_yaw_rad);
		const auto new_point_y = screen_center.y + c::visuals::players::out_of_view::distance * sinf(angle_yaw_rad);

		float size = c::visuals::players::out_of_view::size;

		std::array<vec2_t, 3> points
		{
		   vec2_t(new_point_x - size, new_point_y + size),
		   vec2_t(new_point_x, new_point_y - size),
		   vec2_t(new_point_x + size, new_point_y + size)
		};

		float kek = math::deg2rad(std::remainderf(angle + 90.0f, 360.0f));

		const auto c = cosf(kek);
		const auto s = sinf(kek);

		const auto points_center = (points.at(0) + points.at(1) + points.at(2)) / 3;
		for (auto& point : points)
		{
			point -= points_center;

			const auto x = point.x, y = point.y;

			point.x = x * c - y * s;
			point.y = x * s + y * c;

			point += points_center;
		}

		im_render.drawtrianglefilled((ImVec2&)points[0], (ImVec2&)points[1], (ImVec2&)points[2], color);
	}
}

void features::visuals::get_update_sounds() {
	static CUtlVector<SndInfo_t> sounds;

	interfaces::engine_sound->GetActiveSounds(sounds);

	if (!sounds.Count())
		return;

	for (int i = 0; i < sounds.Count(); ++i) {
		const auto& sound = sounds[i];

		if (!sound.m_nSoundSource)
			continue;

		if (sound.m_nChannel != 4)
			continue;

		if (!sound.m_bUpdatePositions)
			continue;

		player_t* player = player_t::get_player_by_index(sound.m_nSoundSource);

		if (!player)
			continue;

		if (player != g::local)
			continue;

		if (!player->is_alive() || !player->is_player() || player->dormant())
			continue;

		if (player->team() == g::local->team())
			continue;

		if (!sound.m_pOrigin)
			continue;

		auto& player_sound = m_sound_list[player->index()];
		if (player_sound.size() > 0) {
			bool should_break = false;
			for (const auto& snd : player_sound) {
				if (snd.guid == sound.m_nGuid) {
					should_break = true;
					break;
				}
			}

			if (should_break)
				continue;
		}

		auto& new_sound = player_sound.emplace_back();
		new_sound.guid = sound.m_nGuid;
		new_sound.soundPos = *sound.m_pOrigin;
		new_sound.soundTime = interfaces::globals->realtime;
		new_sound.alpha = 1.0f;
	}
}

void features::visuals::player::draw_sounds() {
	for (auto& [entIndex, sound] : m_sound_list) {

		if (sound.empty())
			continue;

		for (auto& info : sound) {
			if (info.soundTime + 0.30f < interfaces::globals->realtime)
				info.alpha -= interfaces::globals->frame_time * 1.0f;

			if (info.alpha <= 0.0f)
				continue;

			float delta_time = interfaces::globals->realtime - info.soundTime;

			auto factor = delta_time / 0.30f;

			if (factor > 1.0f)
				factor = 1.0f;

			float radius = 30.0f * factor;

			im_render.circle_filled_3d(info.soundPos, radius, color_t(c::visuals::players::colors::sounds[0], c::visuals::players::colors::sounds[1], c::visuals::players::colors::sounds[2], info.alpha));
		}

		while (!sound.empty())
		{
			auto& back = sound.back();

			if (back.alpha <= 0.0f)
				sound.pop_back();
			else 
				break;
		}
	}
}

void features::visuals::throwed_grenade(entity_t* entity)
{
	auto client_class = entity->client_class();

	if (!client_class)
	{
		return;
	}

	auto model = entity->model();

	if (!model)
	{
		return;
	}

	auto studio_model = interfaces::model_info->get_studio_model(model);

	if (!studio_model)
	{
		return;
	}

	auto name = (std::string)studio_model->name_char_array;

	if (strstr(client_class->network_name, "Projectile"))
	{
		if (!strstr(studio_model->name_char_array, "thrown") && !strstr(studio_model->name_char_array, "dropped"))
		{
			return;
		}

		vec3_t grenade_origin = entity->origin();
		vec3_t grenade_position;

		if (!interfaces::debug_overlay->world_to_screen(grenade_origin, grenade_position))
		{
			return;
		}

		im_render.text(grenade_position.x, grenade_position.y - 13, c::fonts::esp_sub_size, fonts::sub_esp_font, ("thrown"), true, color_t(255, 255, 255, 200), c::fonts::esp_sub_flag[9], c::fonts::esp_sub_flag[10]);

		std::string grenade_name;
		std::string grenade_icon;

		if (name.find("flashbang") != std::string::npos)
		{
			grenade_name = ("flashbang");
			grenade_icon = u8toStr(u8"\uE02B");
		}

		if (name.find("smokegrenade") != std::string::npos)
		{
			grenade_name = ("smoke");
			grenade_icon = u8toStr(u8"\uE02D");
		}

		if (name.find("incendiarygrenade") != std::string::npos)
		{
			grenade_name = ("incgrenade");
			grenade_icon = u8toStr(u8"\uE030");
		}

		if (name.find("molotov") != std::string::npos)
		{
			grenade_name = ("molotov");
			grenade_icon = u8toStr(u8"\uE02E");
		}

		if (name.find("fraggrenade") != std::string::npos)
		{
			grenade_name = ("he grenade");
			grenade_icon = u8toStr(u8"\uE02C");
		}

		if (name.find("decoy") != std::string::npos)
		{
			grenade_name = ("decoy");
			grenade_icon = u8toStr(u8"\uE02F");
		}

		auto offset = 0;


		if (c::visuals::players::thrown_grenade::icon::enable)
		{
			im_render.text(grenade_position.x, grenade_position.y, 12.0f, fonts::icon_font, grenade_icon, true, color_t(c::visuals::players::thrown_grenade::icon::color), c::fonts::esp_sub_flag[9], c::fonts::esp_sub_flag[10]);

			offset += 13;
		}

		if (c::visuals::players::thrown_grenade::text::enable)
		{
			im_render.text(grenade_position.x, grenade_position.y + offset, c::fonts::esp_sub_size, fonts::sub_esp_font, grenade_name, true, color_t(c::visuals::players::thrown_grenade::text::color), c::fonts::esp_sub_flag[9], c::fonts::esp_sub_flag[10]);
		}
	}
}

float calculate_distance(const vec3_t& pos1, const vec3_t& pos2) {
	float dx = pos1.x - pos2.x;
	float dy = pos1.y - pos2.y;
	float dz = pos1.z - pos2.z;
	return std::sqrt(dx * dx + dy * dy + dz * dz);
}

color_t interpolate_based_on_distance(const color_t& first, const color_t& second, const float distance) {
	return color_t(
		first.r() + distance * (second.r() - first.r()),
		first.g() + distance * (second.g() - first.g()),
		first.b() + distance * (second.b() - first.b()),
		first.a() + distance * (second.a() - first.a())
	);
}

void features::visuals::dropped_weapon(entity_t* entity, bbox_t bbox)
{
	auto weapon = (weapon_t*)entity;

	auto is_weapon = weapon->is_weapon();

	if (!is_weapon)
	{
		return;
	}

	auto weapon_data = weapon->get_weapon_data();

	if (!weapon_data)
	{
		return;
	}

	auto current = weapon->clip1_count();
	auto max = weapon_data->m_iMaxClip;

	vec3_t droppedweapon_origin = weapon->origin();
	vec3_t droppedweapon_position;

	if (droppedweapon_origin.x == 0.f && droppedweapon_origin.y == 0.f && droppedweapon_origin.z == 0.f)
	{
		return;
	}

	if (!interfaces::debug_overlay->world_to_screen(droppedweapon_origin, droppedweapon_position))
	{
		return;
	}

	if (droppedweapon_position.x == 0.f && droppedweapon_position.y == 0.f && droppedweapon_position.z == 0.f)
	{
		return;
	}

	float distance = calculate_distance(g::local->abs_origin(), droppedweapon_origin);
	float max_distance = c::visuals::players::dropped_weapon::distance;
	float interpolation_factor = std::clamp(distance / max_distance, 0.0f, 1.0f);

	std::string droppedweapon_name = weapon->weapon_name_definition();
	std::string droppedweapon_icon = u8toStr(weapon->get_wpn_icon());
	std::string currentammo_text = std::to_string(current);
	std::string maxammo_text = std::to_string(max);

	auto offset = 0;

	if (c::visuals::players::dropped_weapon::box::enable)
	{
		switch (c::visuals::players::dropped_weapon::box::type)
		{
		case 0:
			if (c::visuals::players::dropped_weapon::box::outline[0])
			{
				im_render.drawrect(bbox.x - 1, bbox.y - 1, bbox.w + 2, bbox.h + 2, interpolate_based_on_distance(color_t(0.0f, 0.0f, 0.0f, c::visuals::players::dropped_weapon::box::color[3]), color_t(0.0f, 0.0f, 0.0f, 0.0f), interpolation_factor));
			}

			if (c::visuals::players::dropped_weapon::box::outline[1])
			{
				im_render.drawrect(bbox.x + 1, bbox.y + 1, bbox.w - 2, bbox.h - 2, interpolate_based_on_distance(color_t(0.0f, 0.0f, 0.0f, c::visuals::players::dropped_weapon::box::color[3]), color_t(0.0f, 0.0f, 0.0f, 0.0f), interpolation_factor));
			}

			im_render.drawrect(bbox.x, bbox.y, bbox.w, bbox.h, interpolate_based_on_distance(color_t(c::visuals::players::dropped_weapon::box::color), color_t(c::visuals::players::dropped_weapon::box::color, 0.0f), interpolation_factor));

			break;
		case 1:
			if (c::visuals::players::dropped_weapon::box::outline[0])
			{
				im_render.drawcornerbox(bbox.x - 1, bbox.y - 1, bbox.w + 2, bbox.h + 2, c::visuals::players::dropped_weapon::box::lenght, interpolate_based_on_distance(color_t(0.0f, 0.0f, 0.0f, c::visuals::players::dropped_weapon::box::color[3]), color_t(0.0f, 0.0f, 0.0f, 0.0f), interpolation_factor));
			}

			if (c::visuals::players::dropped_weapon::box::outline[1])
			{
				im_render.drawcornerbox(bbox.x + 1, bbox.y + 1, bbox.w - 2, bbox.h - 2, c::visuals::players::dropped_weapon::box::lenght, interpolate_based_on_distance(color_t(0.0f, 0.0f, 0.0f, c::visuals::players::dropped_weapon::box::color[3]), color_t(0.0f, 0.0f, 0.0f, 0.0f), interpolation_factor));
			}

			im_render.drawcornerbox(bbox.x, bbox.y, bbox.w, bbox.h, c::visuals::players::dropped_weapon::box::lenght, interpolate_based_on_distance(color_t(c::visuals::players::dropped_weapon::box::color), color_t(c::visuals::players::dropped_weapon::box::color, 0.0f), interpolation_factor));

			break;
		}
	}

	if (c::visuals::players::dropped_weapon::ammo_bar::enable)
	{
		if (weapon->isgun())
		{
			float box_w = (float)fabs(bbox.w - bbox.x);

			int width = ((box_w * weapon->clip1_count()) / weapon_data->m_iMaxClip);

			im_render.drawrectfilled(bbox.x - 1, bbox.y + bbox.h + 2, bbox.w + 2, 3, interpolate_based_on_distance(color_t(0.0f, 0.0f, 0.0f, c::visuals::players::dropped_weapon::ammo_bar::color[3]), color_t(0.0f, 0.0f, 0.0f, 0.0f), interpolation_factor));
			im_render.drawrectfilled(bbox.x, bbox.y + bbox.h + 3, bbox.w, 1, interpolate_based_on_distance(color_t(c::visuals::players::dropped_weapon::ammo_bar::color), color_t(c::visuals::players::dropped_weapon::ammo_bar::color, 0.0f), interpolation_factor));

			offset += 4;
		}
	}

	if (c::visuals::players::dropped_weapon::icon::enable)
	{
		im_render.text(bbox.x + (bbox.w / 2), bbox.y + bbox.h + offset, 12.0f, fonts::icon_font, droppedweapon_icon, true, interpolate_based_on_distance(color_t(c::visuals::players::dropped_weapon::icon::color), color_t(c::visuals::players::dropped_weapon::icon::color, 0.0f), interpolation_factor), c::fonts::esp_sub_flag[9], c::fonts::esp_sub_flag[10]);

		offset += 13;
	}

	if (c::visuals::players::dropped_weapon::text::enable)
	{
		im_render.text(bbox.x + (bbox.w / 2), bbox.y + bbox.h + offset, c::fonts::esp_sub_size, fonts::sub_esp_font, droppedweapon_name, true, interpolate_based_on_distance(color_t(c::visuals::players::dropped_weapon::text::color), color_t(c::visuals::players::dropped_weapon::text::color, 0.0f), interpolation_factor), c::fonts::esp_sub_flag[9], c::fonts::esp_sub_flag[10]);

		offset += 13;
	}

	if (c::visuals::players::dropped_weapon::ammo_text::enable && weapon->isgun())
	{
		im_render.text(bbox.x + (bbox.w / 2), bbox.y + bbox.h + offset, c::fonts::esp_sub_size, fonts::sub_esp_font, ("[") + currentammo_text + (" / ") + maxammo_text + ("]"), true, interpolate_based_on_distance(color_t(c::visuals::players::dropped_weapon::ammo_text::color), color_t(c::visuals::players::dropped_weapon::ammo_text::color, 0.0f), interpolation_factor), c::fonts::esp_sub_flag[9], c::fonts::esp_sub_flag[10]);
	}
}