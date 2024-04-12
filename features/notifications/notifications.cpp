#include "notifications.hpp"

void features::notification::run(std::string text, std::string chat_text, bool console, bool chat, bool screen)
{
	color _color = color(menu::menu_accent[0] * 255, menu::menu_accent[1] * 255, menu::menu_accent[2] * 255);

	if (console)
	{
		log_in_console(text, _color);
	}

	if (chat && interfaces::engine->is_in_game() && interfaces::engine->is_connected())
	{
		interfaces::chat_element->chatprintf(chat_text.c_str());
	}

	if (screen)
	{
		log_on_scene(text, _color);
	}
}

void features::notification::log_in_console(std::string text, color _color)
{
	if (text.empty())
	{
		return;
	}

	interfaces::console->console_color_printf({ (unsigned char)_color.r, (unsigned char)_color.g, (unsigned char)_color.b, (unsigned char)_color.a }, "[bhop cheat] ");
	interfaces::console->console_printf(text.append("\n").c_str());
}

void features::notification::log_on_scene(std::string text, color _color)
{
	if (text.empty())
	{
		return;
	}

	features::notification::notify_list.push_back(notify_t(text.c_str(), color(_color.r, _color.g, _color.b, _color.a)));
}

void features::notification::draw()
{
	const auto stay_time = 4.0f;
	const auto fade_time = 0.6f;

	for (offset_message = 0; offset_message < features::notification::notify_list.size(); offset_message++)
	{
		if (features::notification::notify_list.empty())
		{
			continue;
		}

		notify_t notify = features::notification::notify_list[offset_message];

		float elapsed_time = interfaces::globals->cur_time - (notify.time + stay_time);
		int fade_alpha = static_cast<int>(255.f - (elapsed_time / fade_time) * 255.f);
		fade_alpha = std::clamp(fade_alpha, 0, 255);

		if (fade_alpha <= 0)
		{
			features::notification::notify_list.erase(features::notification::notify_list.begin() + offset_message);
			offset_message--;
			continue;
		}

		ImVec2 size = im_render.measure_text("[bhop cheat] ", fonts::scene_font, c::fonts::scene_size);

		im_render.text(5, 5 + (offset_message * 12.0f), c::fonts::scene_size, fonts::scene_font, ("[bhop cheat] "), false, color_t(notify._color.r, notify._color.g, notify._color.b, fade_alpha), c::fonts::scene_flag[9], c::fonts::scene_flag[10]);
		im_render.text(5 + size.x, 5 + (offset_message * 12.0f), c::fonts::scene_size, fonts::scene_font, notify.text, false, color_t(255, 255, 255, fade_alpha), c::fonts::scene_flag[9], c::fonts::scene_flag[10]);
	}
}