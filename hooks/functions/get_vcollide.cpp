#include "../../sdk/sdk.hpp"
#include "../hooks.hpp"
#include "../../features/visuals/visuals.hpp"

vcollide_t* __fastcall sdk::hooks::get_vcollide::get_vcollide(void* edx, void* ecx, int vcollide_index)
{
	if (!features::visuals::weather::m_precip_list.empty())
	{
		for (auto& it : features::visuals::weather::m_precip_list)
		{
			if (it.m_collide_init && it.m_model_index == vcollide_index)
			{
				return &it.m_collide;
			}
		}
	}

	return ofunc(edx, ecx, vcollide_index);
}