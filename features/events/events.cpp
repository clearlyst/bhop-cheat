#include "events.hpp"
#include "../misc/misc.hpp"
#include "../../features/movement/movement.hpp"

#pragma comment(lib, "Winmm.lib")

hooked_events events;

void hooked_events::fire_game_event(i_game_event* event) 
{
	auto event_name = event->get_name();

	if (!strcmp(event_name, "player_hurt")) 
	{
		features::misc::hitmarker::event(event);
		features::misc::hit_info(event);	
	}
}

int hooked_events::get_event_debug_id(void) {
	return EVENT_DEBUG_ID_INIT;
}

void hooked_events::setup() {
	debug_id = EVENT_DEBUG_ID_INIT;
	interfaces::event_manager->add_listener(this, "player_hurt", false);

	printf("ozungaware | events initialized!\n");
}

void hooked_events::unload() {
	interfaces::event_manager->remove_listener(this);
}
