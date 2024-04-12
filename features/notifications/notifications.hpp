#pragma once
#include "../../sdk/sdk.hpp"

namespace features::notification
{
	struct notify_t
	{
		std::string text;
		float time;
		color _color;

		notify_t(std::string _text, color __color)
		{
			text = _text;
			_color = __color;
			time = interfaces::globals->cur_time;
		}
	};

	inline std::vector<notify_t> notify_list;

	inline int offset_message = 0;

	void run(std::string text, std::string chat_text, bool console, bool chat, bool screen);
	void log_in_console(std::string text, color _color);
	void log_on_scene(std::string text, color _color);
	void draw();
}