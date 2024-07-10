#pragma once
#include "../math/vec3.hpp"
#include "../utils/vfunc.hpp"
#include "../structs/color.hpp"

class iv_text_overlay;

class iv_debug_overlay {
public:
	virtual void AddEntityTextOverlay(int ent_index, int line_offset, float duration, int r, int g, int b, int a, const char* format, ...) = 0;
	virtual void add_box_overlay(const vec3_t& origin, const vec3_t& mins, const vec3_t& max, vec3_t const& orientation, int r, int g, int b, int a, float duration) = 0;
	virtual void AddSphereOverlay(const vec3_t& vOrigin, float flRadius, int nTheta, int nPhi, int r, int g, int b, int a, float flDuration) = 0;
	virtual void AddTriangleOverlay(const vec3_t& p1, const vec3_t& p2, const vec3_t& p3, int r, int g, int b, int a, bool noDepthTest, float duration) = 0;
	virtual void AddLineOverlay(const vec3_t& origin, const vec3_t& dest, int r, int g, int b, bool noDepthTest, float duration) = 0;
	virtual void AddTextOverlay(const vec3_t& origin, float duration, const char* format, ...) = 0;
	virtual void AddTextOverlay(const vec3_t& origin, int line_offset, float duration, const char* format, ...) = 0;
	virtual void AddScreenTextOverlay(float flXPos, float flYPos, float flDuration, int r, int g, int b, int a, const char* text) = 0;
	virtual void AddSweptBoxOverlay(const vec3_t& start, const vec3_t& end, const vec3_t& mins, const vec3_t& max, const vec3_t& angles, int r, int g, int b, int a, float flDuration) = 0;
	virtual void AddGridOverlay(const vec3_t& origin) = 0;
	virtual void AddCoordFrameOverlay(const matrix3x4_t& frame, float flScale, int vColorTable[3][3] = NULL) = 0;
	virtual int ScreenPosition(const vec3_t& point, vec3_t& screen) = 0;
	virtual int ScreenPosition(float flXPos, float flYPos, vec3_t& screen) = 0;
	virtual iv_text_overlay* GetFirst(void) = 0;
	virtual iv_text_overlay* GetNext(iv_text_overlay* current) = 0;
	virtual void ClearDeadOverlays(void) = 0;
	virtual void ClearAllOverlays(void) = 0;
	virtual void AddTextOverlayRGB(const vec3_t& origin, int line_offset, float duration, float r, float g, float b, float alpha, const char* format, ...) = 0;
	virtual void AddTextOverlayRGB(const vec3_t& origin, int line_offset, float duration, int r, int g, int b, int a, const char* format, ...) = 0;
	virtual void AddLineOverlayAlpha(const vec3_t& origin, const vec3_t& dest, int r, int g, int b, int a, bool noDepthTest, float duration) = 0;
	virtual void AddBoxOverlay2(const vec3_t& origin, const vec3_t& mins, const vec3_t& max, vec3_t const& orientation,
		const uint8_t* faceColor, const uint8_t* edgeColor, float duration) = 0;
	virtual void AddLineOverlay(const vec3_t& origin, const vec3_t& dest, int r, int g, int b, int a, float, float) = 0;
	virtual void PurgeTextOverlays() = 0;

	bool world_to_screen(const vec3_t& in, vec3_t& out) {
		using original_fn = int(__thiscall*)(iv_debug_overlay*, const vec3_t&, vec3_t&);
		int return_value = (*(original_fn**)this)[13](this, in, out);
		return static_cast<bool>(return_value != 1);
	}

	void box_overlay(const vec3_t& origin, const vec3_t& mins, const vec3_t& maxs, const vec3_t& orientation, int r, int g, int b, int a, float duration)
	{
		using original_fn = void(__thiscall*)(void*, const vec3_t&, const vec3_t&, const vec3_t&, const vec3_t&, int, int, int, int, float);
		CallVFunction<original_fn>(this, 1)(this, origin, mins, maxs, orientation, r, g, b, a, duration);
	}

	void add_capsule_overlay(const vec3_t& mins, const vec3_t& maxs, float radius, int r, int g, int b, int a, float duration)
	{
		using original_fn = void(__thiscall*)(void*, const vec3_t&, const vec3_t&, float, int, int, int, int, float);
		CallVFunction<original_fn>(this, 24)(this, mins, maxs, radius, r, g, b, a, duration);
	}
};
