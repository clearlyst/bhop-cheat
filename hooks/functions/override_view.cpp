#include "../hooks.hpp"
#include "../../features/visuals/visuals.hpp"
#include "../../features/misc/misc.hpp"
#include "../../menu/config/config.hpp"

void __fastcall sdk::hooks::override_view::override_view( void* _this, void* _edx, view_setup_t* setup ) 
{
	features::misc::viewfov();
	features::misc::viewmodel();
	features::misc::swayscale();
	features::visuals::freecam(setup);

	ofunc( interfaces::client_mode, _this, setup );
}