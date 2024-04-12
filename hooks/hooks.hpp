#pragma once
#include <d3d9.h>

#include "../sdk/classes/c_view_setup.hpp"
#include "../sdk/classes/view_setup_t.hpp"
#include "../sdk/interfaces/isurface.hpp"
#include "../includes/minhook/minhook.h"
#include "../sdk/interfaces/ivmodelinfo.hpp"
#include "../features/aimbot/aimbot.hpp"
#include "../sdk/structs/materials.hpp"
#include "../sdk/structs/usercmd.hpp"
#include "../sdk/math/vec3.hpp"
#include "../utils/vfunc.hpp"
#include "../sdk/sdk.hpp"

#define registers void* ecx, void* edx


template<class T> struct CUtlReference {
	CUtlReference* m_pNext;
	CUtlReference* m_pPrev;
	T* m_pObject;
};

template<class T> struct CUtlIntrusiveList {
	T* m_pHead;
};

template<class T> struct CUtlIntrusiveDList : public CUtlIntrusiveList<T> { };
template<class T> struct CUtlReferenceList : public CUtlIntrusiveDList< CUtlReference<T> > { };

enum EAttributeDataType {
	ATTRDATATYPE_NONE = -1,
	ATTRDATATYPE_FLOAT = 0,
	ATTRDATATYPE_4V,
	ATTRDATATYPE_INT,
	ATTRDATATYPE_POINTER,

	ATTRDATATYPE_COUNT,
};

#define MAX_PARTICLE_ATTRIBUTES 24

#define DEFPARTICLE_ATTRIBUTE( name, bit, datatype )			\
	const int PARTICLE_ATTRIBUTE_##name##_MASK = (1 << bit);	\
	const int PARTICLE_ATTRIBUTE_##name = bit;					\
	const EAttributeDataType PARTICLE_ATTRIBUTE_##name##_DATATYPE = datatype;

DEFPARTICLE_ATTRIBUTE(TINT_RGB, 6, ATTRDATATYPE_4V);

DEFPARTICLE_ATTRIBUTE(ALPHA, 7, ATTRDATATYPE_FLOAT);

struct CUtlString_simple {
	char* buffer;
	int capacity;
	int grow_size;
	int length;
};

class CParticleSystemDefinition {
	BYTE pad_0[308];
public:
	CUtlString_simple m_Name;
};

struct CParticleAttributeAddressTable {
	float* m_pAttributes[MAX_PARTICLE_ATTRIBUTES];
	size_t m_nFloatStrides[MAX_PARTICLE_ATTRIBUTES];

	FORCEINLINE float* FloatAttributePtr(int nAttribute, int nParticleNumber) const {
		int block_ofs = nParticleNumber / 4;
		return m_pAttributes[nAttribute] +
			m_nFloatStrides[nAttribute] * block_ofs +
			(nParticleNumber & 3);
	}

};

class CParticleCollection {
	BYTE pad_0[48];//0
public:
	int m_nActiveParticles;//48
private:
	BYTE pad_1[12];//52
public:
	CUtlReference<CParticleSystemDefinition> m_pDef;//64
private:
	BYTE pad_2[60];//80
public:
	CParticleCollection* m_pParent;//136
private:
	BYTE pad_3[84];//140
public:
	CParticleAttributeAddressTable m_ParticleAttributes;//224
};

template <typename T>
static auto Rel2Abs(const T& address) noexcept {
	return address + 4 + *reinterpret_cast<int*>(address);
}

namespace sdk {
	namespace hooks {

		inline unsigned int get_virtual(void* _class, unsigned int index) { return static_cast<unsigned int>((*reinterpret_cast<int**>(_class))[index]); }

		namespace create_move {
			using fn = bool(__stdcall* )(float, c_usercmd* );
			inline fn ofunc;
			bool __stdcall create_move(float sample_time, c_usercmd* cmd );
		}

		namespace get_color_modulation {
			using fn = void(__fastcall*)(i_material*, void*, float*, float*, float* );
			inline fn ofunc;
			void __fastcall get_color_modulation(i_material* material, void* edx, float* r, float* g, float* b);
		}

		namespace get_alpha_modulation {
			using fn = float(__fastcall*)(i_material*, void*);
			inline fn ofunc;
			float __fastcall get_alpha_modulation(i_material* material, void* edx);
		}

		namespace present {
			using fn = long( __stdcall* )( IDirect3DDevice9*, RECT*, RECT*, HWND, RGNDATA* );
			inline fn ofunc;
			long __stdcall present( IDirect3DDevice9* device, RECT* source_rect, RECT* dest_rect, HWND dest_window_override, RGNDATA* dirty_region );
		}

		namespace override_mouse_input {
			using fn = void( __fastcall* )( void*, int, float*, float* );
			inline fn ofunc;
			void __fastcall override_mouse_input( void* this_, int edx, float* x, float* y );
		}

		namespace push_notice {
			using fn = void(__fastcall*)(int, int, const char*, int, const char*);
			inline fn ofunc;
			void __fastcall push_notice(int ecx, int edx, const char* text, int str_len, const char* null);
		}

		namespace emit_sound {
			using fn = void(__fastcall*)(void*, void*, void*, int, int, const char*, unsigned int, const char*, float, float, int, int, int, const vec3_t*, const vec3_t*, vec3_t*, bool, float, int, void*);
			inline fn ofunc;
			void __fastcall emit_sound(registers, void* filter, int iEntIndex, int iChannel, const char* pSoundEntry, unsigned int nSoundEntryHash, const char* pSample, float flVolume, float flAttenuation, int nSeed, int iFlags, int iPitch, const vec3_t* pOrigin, const vec3_t* pDirection, vec3_t* pUtlVecOrigins, bool bUpdatePositions, float soundtime, int speakerentity, void* fds);
		}

		namespace frame_stage_notify {
			using fn = void( __thiscall* )( i_base_client_dll*, int );
			inline fn ofunc;
			void __stdcall frame_stage_notify( int frame_stage );
		}

		namespace reset {
			using fn = long( __stdcall* )( IDirect3DDevice9*, D3DPRESENT_PARAMETERS* );
			inline fn ofunc;
			long __stdcall reset( IDirect3DDevice9* device, D3DPRESENT_PARAMETERS* pp );
		}

		namespace lock_cursor {
			using fn = void( __fastcall* )( i_surface* );
			inline fn ofunc;
			void __fastcall lock_cursor( i_surface* _this );
		}

		namespace draw_model_execute {
			using fn = void(__fastcall*)(void*, int, i_mat_render_context*, const draw_model_state_t&, const model_render_info_t&, matrix_t*);
			inline fn draw_model_execute_original;
			void __fastcall draw_model_execute(void* _this, int edx, i_mat_render_context* ctx, const draw_model_state_t& state, const model_render_info_t& pInfo, matrix_t* pCustomBoneToWorld);
		}

		namespace override_view {
			using fn = void* ( __fastcall* )( i_client_mode*, void* _this, view_setup_t* setup );
			inline fn ofunc;
			void __fastcall override_view( void* _this, void* _edx, view_setup_t* setup );
		}

		namespace alloc_key_values_memory {
			using fn = void* (__thiscall*)(void*, const std::int32_t);
			inline fn ofunc;
			void* __stdcall alloc_key_values_memory(const std::int32_t size);
		}

		namespace list_leaves_in_box {
			using fn = int(__thiscall*)(void*, const vec3_t&, const vec3_t&, unsigned short*, int);
			inline fn ofunc;
			int __fastcall list_leaves_in_box(void* bsp, void* edx, const vec3_t& mins, const vec3_t& maxs, unsigned short* list, int list_max);
		}

		namespace set_visuals_data {
			using fn = void(__fastcall*)(void*, void*, const char*);
			inline fn ofunc;
			void __fastcall set_visuals_data(void* ecx, void* edx, const char* shader_name);
		}

		namespace modify_eye_position {
			using fn = void(__fastcall*)(void*, void*, vec3_t&);
			inline fn ofunc;
			void __fastcall modify_eye_position(void* thisPointer, void* edx, vec3_t& input_eye_pos);
		}

		namespace calcultate_view {
			using fn = void(__fastcall*)(player_t*, void*, vec3_t&, vec3_t&, float&, float&, float&);
			inline fn ofunc;
			void __fastcall calcultate_view(player_t* player, void* edx, vec3_t& eye_origin, vec3_t& eye_angles, float& z_near, float& z_far, float& fov);
		}

		namespace do_post_screen_effects {
			using fn = bool(__thiscall*)(void*, view_setup_t*);
			inline fn ofunc;
			bool __stdcall do_post_screen_effects(view_setup_t* setup);
		}

		namespace paint_traverse {
			using fn = void(__thiscall*)(i_panel*, unsigned int, bool, bool);
			inline fn ofunc;
			void __stdcall paint_traverse(unsigned int panel, bool force_repaint, bool allow_force);
		}

		namespace send_datagram {
			using fn = int(__fastcall*)(i_net_channel*, int, bf_write*);
			inline fn ofunc;
			int __fastcall send_datagram(i_net_channel* thisptr, int edx, bf_write* datagram);
		}

		namespace particle {
			using fn = void(__fastcall*)(CParticleCollection*, void*);
			inline fn ofunc;
			void __fastcall simulations(CParticleCollection* thisPtr, void* edx);
		}

		namespace fire_event {
			using fn = void(__fastcall*)(void*, void* edx, i_game_event* gameEvent, bool bServerOnly, bool bClientOnly);
			inline fn ofunc;
			void __fastcall fire_event(void* thisptr, void* edx, i_game_event* gameEvent, bool bServerOnly, bool bClientOnly);
		}

        #pragma region sv_pure
		namespace loose_files_allowed {
			using fn = void(__thiscall*)(void*);
			inline fn ofunc;
			bool __stdcall loose_files_allowed();
		}

		namespace check_for_pure_server_white_list {
			using fn = void(__thiscall*)(void*);
			inline fn ofunc;
			void __fastcall check_for_pure_server_white_list(void* edx, void* ecx);
		}
        #pragma endregion

		inline WNDPROC wndproc_original;
		inline HWND window;
		LRESULT __stdcall wndproc( HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam );
		void init( );
		void unload( );
	}
}