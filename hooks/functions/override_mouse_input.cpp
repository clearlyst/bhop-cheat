#include "../hooks.hpp"
#include "../../menu/config/config.hpp"
#include "../../features/movement/movement.hpp"

void __fastcall sdk::hooks::override_mouse_input::override_mouse_input( void* this_, int edx, float* x, float* y )
{
	features::movement::mouse_strafe_limiter(x, y);

	return ofunc( this_, edx, x, y );
}