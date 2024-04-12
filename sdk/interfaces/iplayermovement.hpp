#pragma once
#include "../math/vec3.hpp"
#include "../structs/usercmd.hpp"
#include "../../utils/vfunc.hpp"
#include "../../utils/utlvector.hpp"

class i_physics_surface_props;
enum ESoundLevel;
class player_t;

class IMoveHelper
{
public:
    virtual	const char* GetName(void* hEntity) const = 0;
    virtual void				SetHost(player_t* pHost) = 0;
    virtual void				ResetTouchList() = 0;
    virtual bool                pad() = 0;
    virtual void				ProcessImpacts() = 0;
    virtual void				Con_NPrintf(int nIndex, char const* fmt, ...) = 0;
    virtual void				StartSound(const vec3_t& vecOrigin, int iChannel, char const* szSample, float flVolume, ESoundLevel soundlevel, int fFlags, int iPitch) = 0;
    virtual void				StartSound(const vec3_t& vecOrigin, const char* soundname) = 0;
    virtual void				PlaybackEventFull(int fFlags, int nClientIndex, unsigned short uEventIndex, float flDelay, vec3_t& vecOrigin, vec3_t& vecAngles, float flParam1, float flParam2, int iParam1, int iParam2, int bParam1, int bParam2) = 0;
    virtual bool				PlayerFallingDamage() = 0;
    virtual void				PlayerSetAnimation(int playerAnimation) = 0;
    virtual i_physics_surface_props* GetSurfaceProps() = 0;
    virtual bool				IsWorldEntity(const unsigned long& hEntity) = 0;
};

class CMoveData
{
public:
    bool    m_bFirstRunOfFunctions : 1;
    bool    m_bGameCodeMovedPlayer : 1;
    int     m_nPlayerHandle;        // edict index on server, client entity handle on client=
    int     m_nImpulseCommand;      // Impulse command issued.
    vec3_t  m_vecViewAngles;        // Command view angles (local space)
    vec3_t  m_vecAbsViewAngles;     // Command view angles (world space)
    int     m_nButtons;             // Attack buttons.
    int     m_nOldButtons;          // From host_client->oldbuttons;
    float   m_flForwardMove;
    float   m_flSideMove;
    float   m_flUpMove;
    float   m_flMaxSpeed;
    float   m_flClientMaxSpeed;
    vec3_t  m_vecVelocity;          // edict::velocity        // Current movement direction.
    vec3_t  m_vecAngles;            // edict::angles
    vec3_t  m_vecOldAngles;
    float   m_outStepHeight;        // how much you climbed this move
    vec3_t  m_outWishVel;           // This is where you tried 
    vec3_t  m_outJumpVel;           // This is your jump velocity
    vec3_t  m_vecConstraintCenter;
    float   m_flConstraintRadius;
    float   m_flConstraintWidth;
    float   m_flConstraintSpeedFactor;
    float   m_flUnknown[5];
    vec3_t  m_vecAbsOrigin;
};

class IGameMovement
{
public:
    virtual			~IGameMovement(void) {}

    virtual void	          ProcessMovement(player_t* pPlayer, CMoveData* pMove) = 0;
    virtual void	          Reset(void) = 0;
    virtual void	          StartTrackPredictionErrors(player_t* pPlayer) = 0;
    virtual void	          FinishTrackPredictionErrors(player_t* pPlayer) = 0;
    virtual void	          DiffPrint(char const* fmt, ...) = 0;
    virtual vec3_t const& GetPlayerMins(bool ducked) const = 0;
    virtual vec3_t const& GetPlayerMaxs(bool ducked) const = 0;
    virtual vec3_t const& GetPlayerViewOffset(bool ducked) const = 0;
    virtual bool		        IsMovingPlayerStuck(void) const = 0;
    virtual player_t* GetMovingPlayer(void) const = 0;
    virtual void		        UnblockPusher(player_t* pPlayer, player_t* pPusher) = 0;
    virtual void            SetupMovementBounds(CMoveData* pMove) = 0;
};

class CGameMovement : public IGameMovement
{
public:
    virtual ~CGameMovement(void) {}
};

class IPrediction
{
public:
    bool& m_bInPrediction()
    {
        return *(bool*)((uintptr_t)(this) + 0x8);
    }

    bool& m_bEnginePaused()
    {
        return *(bool*)((uintptr_t)(this) + 0xA);
    }

    bool& m_bIsFirstTimePredicted()
    {
        return *(bool*)((uintptr_t)(this) + 0x18);
    }

    int& m_nCommandsPredicted()
    {
        return *(int*)((uintptr_t)(this) + 0x1C);
    }

    void Update(int start_frame, bool valid_frame, int inc_ack, int out_cmd)
    {
        typedef void(__thiscall* oUpdate)(void*, int, bool, int, int);
        return CallVFunction<oUpdate>(this, 3)(this, start_frame, valid_frame, inc_ack, out_cmd);
    }

    void GetLocalViewAngle(vec3_t& view)
    {
        typedef void(__thiscall* oGetLocalViewAngle)(void*, vec3_t&);
        return CallVFunction<oGetLocalViewAngle>(this, 12)(this, view);
    }

    void SetLocalViewAngle(vec3_t& view)
    {
        typedef void(__thiscall* oSetLocalViewAngle)(void*, vec3_t&);
        return CallVFunction<oSetLocalViewAngle>(this, 13)(this, view);
    }

    void CheckMovingGround(player_t* player, double frametime)
    {
        typedef void(__thiscall* oCheckMovingGround)(void*, player_t*, double);
        return CallVFunction<oCheckMovingGround>(this, 18)(this, player, frametime);
    }

    void RunCommand(player_t* player, c_usercmd* ucmd, IMoveHelper* moveHelper)
    {
        typedef void(__thiscall* oRunCommand)(void*, player_t*, c_usercmd*, IMoveHelper*);
        return CallVFunction<oRunCommand>(this, 19)(this, player, ucmd, moveHelper);
    }

    void SetupMove(player_t* player, c_usercmd* ucmd, IMoveHelper* moveHelper, void* pMoveData)
    {
        typedef void(__thiscall* oSetupMove)(void*, player_t*, c_usercmd*, IMoveHelper*, void*);
        return CallVFunction<oSetupMove>(this, 20)(this, player, ucmd, moveHelper, pMoveData);
    }

    void FinishMove(player_t* player, c_usercmd* ucmd, void* pMoveData)
    {
        typedef void(__thiscall* oFinishMove)(void*, player_t*, c_usercmd*, void*);
        return CallVFunction<oFinishMove>(this, 21)(this, player, ucmd, pMoveData);
    }
};