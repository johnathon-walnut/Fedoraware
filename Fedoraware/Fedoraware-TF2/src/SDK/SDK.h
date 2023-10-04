#pragma once

#define VK_0              0x30
#define VK_1              0x31
#define VK_2              0x32
#define VK_3              0x33
#define VK_4              0x34
#define VK_5              0x35
#define VK_6              0x36
#define VK_7              0x37
#define VK_8              0x38
#define VK_9              0x39

#define VK_A              0x41
#define VK_B              0x42
#define VK_C              0x43
#define VK_D              0x44
#define VK_E              0x45
#define VK_F              0x46
#define VK_G              0x47
#define VK_H              0x48
#define VK_J              0x49
#define VK_I              0x4A
#define VK_K              0x4B
#define VK_L              0x4C
#define VK_M              0x4D
#define VK_N              0x4E
#define VK_O              0x4F
#define VK_P              0x50
#define VK_Q              0x51
#define VK_R              0x52
#define VK_S              0x53
#define VK_T              0x54
#define VK_U              0x55
#define VK_V              0x56
#define VK_W              0x57
#define VK_X              0x58
#define VK_Y              0x59
#define VK_Z              0x5A

#define TEAM_NONE		  1
#define TEAM_RED		  2
#define TEAM_BLU		  3

#include "Signatures/Signatures.h"
#include "Main/BaseEntity/BaseEntity.h"
#include "Main/BaseCombatWeapon/BaseCombatWeapon.h"
#include "Main/BaseObject/BaseObject.h"
#include "Main/DrawUtils/DrawUtils.h"
#include "Main/EntityCache/EntityCache.h"
#include "Main/GlobalInfo/GlobalInfo.h"
#include "Main/ConVars/ConVars.h"
#include "Main/KeyValues/KeyValues.h"
#include "Main/TraceFilters/TraceFilters.h"
#include "../Features/Vars.h"

#define TICK_INTERVAL		( I::GlobalVars->interval_per_tick )
#define TIME_TO_TICKS( dt )	( static_cast<int>( (TICK_INTERVAL / 2) + static_cast<float>(dt) / TICK_INTERVAL ) )
#ifndef TICKS_TO_TIME
#define TICKS_TO_TIME( t )	( TICK_INTERVAL * ( t ) )
#endif
#define GetKey(vKey) (Utils::IsGameWindowInFocus() && GetAsyncKeyState(vKey))
#define Q_ARRAYSIZE(A) (sizeof(A)/sizeof((A)[0]))

#pragma warning (disable : 6385)
#pragma warning (disable : 26451)
#pragma warning (disable : 4305)
#pragma warning (disable : 4172)

namespace S
{
	MAKE_SIGNATURE(InitKeyValue, CLIENT_DLL, "55 8B EC FF 15 ? ? ? ? FF 75 ? 8B C8 8B 10 FF 52 ? 5D C3 CC CC CC CC CC CC CC CC CC CC CC 55 8B EC 56", 0x0);
	MAKE_SIGNATURE(ATTRIB_HOOK_FLOAT, CLIENT_DLL, "55 8B EC 83 EC 0C 8B 0D ? ? ? ? 53 56 57 33 F6 33 FF 89 75 F4 89 7D F8 8B 41 08 85 C0 74 38", 0x0);
	MAKE_SIGNATURE(CTE_DispatchEffect, CLIENT_DLL, "55 8B EC 83 EC ? 56 8D 4D ? E8 ? ? ? ? 8B 75", 0x0);
	MAKE_SIGNATURE(GetParticleSystemIndex, CLIENT_DLL, "55 8B EC 56 8B 75 ? 85 F6 74 ? 8B 0D ? ? ? ? 56 8B 01 FF 50 ? 3D", 0x0);
	MAKE_SIGNATURE(UTIL_ParticleTracer, CLIENT_DLL, "55 8B EC FF 75 08 E8 ? ? ? ? D9 EE 83", 0x0);
}

struct ShaderStencilState_t
{
	bool                        m_bEnable;
	StencilOperation_t          m_FailOp;
	StencilOperation_t          m_ZFailOp;
	StencilOperation_t          m_PassOp;
	StencilComparisonFunction_t m_CompareFunc;
	int                         m_nReferenceValue;
	uint32                      m_nTestMask;
	uint32                      m_nWriteMask;

	ShaderStencilState_t();
	void SetStencilState(IMatRenderContext *pRenderContext);
};

// sto and sto 2, tyo tyo1 tyo2
enum DataCenter_t
{
	DC_AMS,	//	Europe
	DC_FRA,
	DC_LHR,
	DC_MAD,
	DC_PAR,
	DC_STO,
	DC_VIE,
	DC_WAW,

	DC_ATL,	//	North America
	DC_ORD,
	DC_DFW,
	DC_LAX,
	DC_EAT,
	DC_SEA,
	DC_IAD,

	DC_EZE,	//	South America
	DC_LIM,
	DC_SCL,
	DC_GRU,

	DC_MAA,	//	Asia
	DC_BOM,
	DC_DXB,
	DC_HKG,
	DC_SEO,
	DC_SGP,
	DC_TYO,
	DC_CAN,
	DC_SHA,
	DC_TSN,

	DC_JNB,	//	Africa
	
	DC_SYD,	//	Australia
};


inline ShaderStencilState_t::ShaderStencilState_t()
{
	m_bEnable = false;
	m_PassOp = m_FailOp = m_ZFailOp = STENCILOPERATION_KEEP;
	m_CompareFunc = STENCILCOMPARISONFUNCTION_ALWAYS;
	m_nReferenceValue = 0;
	m_nTestMask = m_nWriteMask = 0xFFFFFFFF;
}

inline void ShaderStencilState_t::SetStencilState(IMatRenderContext *pRenderContext)
{
	pRenderContext->SetStencilEnable(m_bEnable);
	pRenderContext->SetStencilFailOperation(m_FailOp);
	pRenderContext->SetStencilZFailOperation(m_ZFailOp);
	pRenderContext->SetStencilPassOperation(m_PassOp);
	pRenderContext->SetStencilCompareFunction(m_CompareFunc);
	pRenderContext->SetStencilReferenceValue(m_nReferenceValue);
	pRenderContext->SetStencilTestMask(m_nTestMask);
	pRenderContext->SetStencilWriteMask(m_nWriteMask);
}

namespace Utils
{
	static std::random_device RandomDevice;
	static std::mt19937 Engine{ RandomDevice() };
	__inline float RandFloatRange(float min, float max)
	{
		std::uniform_real_distribution<float> Random(min, max);
		return Random(Engine);
	}

	__inline bool W2S(const Vec3 &vOrigin, Vec3 &m_vScreen)
	{
		const matrix3x4 &worldToScreen = G::WorldToProjection.As3x4();

		float w = worldToScreen[3][0] * vOrigin[0] + worldToScreen[3][1] * vOrigin[1] + worldToScreen[3][2] * vOrigin[2] + worldToScreen[3][3];
		m_vScreen.z = 0;

		if (w > 0.001)
		{
			const float fl1DBw = 1 / w;
			m_vScreen.x = (g_ScreenSize.w / 2) + (0.5 * ((worldToScreen[0][0] * vOrigin[0] + worldToScreen[0][1] * vOrigin[1] + worldToScreen[0][2] * vOrigin[2] + worldToScreen[0][3]) * fl1DBw) * g_ScreenSize.w + 0.5);
			m_vScreen.y = (g_ScreenSize.h / 2) - (0.5 * ((worldToScreen[1][0] * vOrigin[0] + worldToScreen[1][1] * vOrigin[1] + worldToScreen[1][2] * vOrigin[2] + worldToScreen[1][3]) * fl1DBw) * g_ScreenSize.h + 0.5);
			return true;
		}

		return false;
	}

	__inline Color_t Rainbow(float offset = 0.f)
	{
		return
		{
			static_cast<byte>(floor(sin(I::GlobalVars->curtime + offset + 0.0f) * 127.0f + 128.0f)),
			static_cast<byte>(floor(sin(I::GlobalVars->curtime + offset + 2.0f) * 127.0f + 128.0f)),
			static_cast<byte>(floor(sin(I::GlobalVars->curtime + offset + 4.0f) * 127.0f + 128.0f)),
			255
		};
	};

	__inline void GetProjectileFireSetup(CBaseEntity *pPlayer, const Vec3 &vViewAngles, Vec3 vOffset, Vec3 *vSrc)
	{
		if (g_ConVars.cl_flipviewmodels->GetBool()) { vOffset.y *= -1.0f; }

		Vec3 vecForward = Vec3(), vecRight = Vec3(), vecUp = Vec3();
		Math::AngleVectors(vViewAngles, &vecForward, &vecRight, &vecUp);

		*vSrc = pPlayer->GetShootPos() + (vecForward * vOffset.x) + (vecRight * vOffset.y) + (vecUp * vOffset.z);
	}

	__inline bool IsGameWindowInFocus()
	{
		static HWND hwGame = nullptr;

		while (!hwGame) {
			hwGame = FindWindowA(nullptr, "Team Fortress 2");
			if (!hwGame)
			{
				return false;
			}
		}

		return (GetForegroundWindow() == hwGame);
	}

	__inline void* InitKeyValue()
	{
		using FN = PDWORD(__cdecl* )(int);
		static FN fnInitKeyValue = S::InitKeyValue.As<FN>();
		return fnInitKeyValue(32);
	}

	Color_t GetTeamColor(int nTeamNum, bool otherColors);

	Color_t GetEntityDrawColor(CBaseEntity* pEntity, bool enableOtherColors);

	__inline bool IsSteamFriend(CBaseEntity* pPlayer)
	{
		PlayerInfo_t pi = { };

		if (I::EngineClient->GetPlayerInfo(pPlayer->GetIndex(), &pi) && pi.friendsID)
		{
			const CSteamID steamID{ pi.friendsID, 1, k_EUniversePublic, k_EAccountTypeIndividual };
			return g_SteamInterfaces.Friends->HasFriend(steamID, k_EFriendFlagImmediate);
		}

		return false;
	}

	__inline const char* GetClassByIndex(const int nClass)
	{
		static const char* szClasses[] = { "unknown", "scout", "sniper", "soldier", "demoman",
										   "medic",   "heavy", "pyro",   "spy",     "engineer" };

		return (nClass < 10 && nClass > 0) ? szClasses[nClass] : szClasses[0];
	}

	__inline Color_t GetHealthColor(int nHP, int nMAXHP)
	{
		nHP = std::max(0, std::min(nHP, nMAXHP));
		int r = std::min((510 * (nMAXHP - nHP)) / nMAXHP, 200);
		int g = std::min((510 * nHP) / nMAXHP, 200);
		return { static_cast<byte>(r), static_cast<byte>(g), 0, 255 };
	}

	__inline bool IsOnScreen(CBaseEntity* pLocal, CBaseEntity* pEntity)
	{
		const Vec3 wsc = pEntity->GetWorldSpaceCenter();

		if (wsc.DistTo(pLocal->GetWorldSpaceCenter()) > 300.0f)
		{
			Vec3 vScreen = {};

			if (W2S(pEntity->GetWorldSpaceCenter(), vScreen))
			{
				if (vScreen.x < -400
					|| vScreen.x > g_ScreenSize.w + 400
					|| vScreen.y < -400
					|| vScreen.y > g_ScreenSize.h + 400)
				{
					return false;
				}
			}
			else
			{
				return false;
			}
		}

		return true;
	}

	__inline void TraceHull(const Vec3 &vecStart, const Vec3 &vecEnd, const Vec3 &vecHullMin, const Vec3 &vecHullMax,
		unsigned int nMask, CTraceFilter *pFilter, CGameTrace *pTrace)
	{
		Ray_t ray;
		ray.Init(vecStart, vecEnd, vecHullMin, vecHullMax);
		I::EngineTrace->TraceRay(ray, nMask, pFilter, pTrace);
	}

	__inline void Trace(const Vec3 &vecStart, const Vec3 &vecEnd, unsigned int nMask, CTraceFilter *pFilter, CGameTrace *pTrace)
	{
		Ray_t ray;
		ray.Init(vecStart, vecEnd);
		I::EngineTrace->TraceRay(ray, nMask, pFilter, pTrace);
	}

	__inline int RandInt(int min, int max)
	{
		//This allows us to reach closer to true randoms generated
		//I don't think we need to update the seed more than once
		static const unsigned nSeed = std::chrono::system_clock::now().time_since_epoch().count();

		std::default_random_engine gen(nSeed);
		std::uniform_int_distribution distr(min, max);
		return distr(gen);
	}

	__inline int RandIntSimple(int min, int max)
	{
		std::random_device rd; std::mt19937 gen(rd()); std::uniform_int_distribution<> distr(min, max);
		return distr(gen);
	}

	__inline void FixMovement(CUserCmd *pCmd, const Vec3 &vecTargetAngle)
	{
		const Vec3 vecMove(pCmd->forwardmove, pCmd->sidemove, pCmd->upmove);
		Vec3 vecMoveAng = Vec3();

		Math::VectorAngles(vecMove, vecMoveAng);

		const float fSpeed = Math::FastSqrt(vecMove.x * vecMove.x + vecMove.y * vecMove.y);
		const float fYaw = DEG2RAD(vecTargetAngle.y - pCmd->viewangles.y + vecMoveAng.y);

		pCmd->forwardmove = (cos(fYaw) * fSpeed);
		pCmd->sidemove = (sin(fYaw) * fSpeed);
	}

	__inline int UnicodeToUTF8(const wchar_t* unicode, char* ansi, int ansiBufferSize)
	{
		const int result = WideCharToMultiByte(CP_UTF8, 0, unicode, -1, ansi, ansiBufferSize, nullptr, nullptr);
		ansi[ansiBufferSize - 1] = 0;
		return result;
	}

	__inline int UTF8ToUnicode(const char* ansi, wchar_t* unicode, int unicodeBufferSizeInBytes)
	{
		const int chars = MultiByteToWideChar(CP_UTF8, 0, ansi, -1, unicode, unicodeBufferSizeInBytes / sizeof(wchar_t));
		unicode[(unicodeBufferSizeInBytes / sizeof(wchar_t)) - 1] = 0;
		return chars;
	}

	__inline std::wstring ConvertUtf8ToWide(const std::string_view& str)
	{
		const int count = MultiByteToWideChar(CP_UTF8, 0, str.data(), str.length(), nullptr, 0);
		std::wstring wstr(count, 0);
		MultiByteToWideChar(CP_UTF8, 0, str.data(), str.length(), wstr.data(), count);
		return wstr;
	}

	__inline float ATTRIB_HOOK_FLOAT(float baseValue, const char *searchString, CBaseEntity *ent, void *buffer, bool isGlobalConstString)
	{
		static auto fn = S::ATTRIB_HOOK_FLOAT.As<float(__cdecl *)(float, const char *, CBaseEntity *, void *, bool)>();
		return fn(baseValue, searchString, ent, buffer, isGlobalConstString);
	}

	__inline int SeedFileLineHash(int seedvalue, const char *sharedname, int additionalSeed)
	{
		CRC32_t retval;

		CRC32_Init(&retval);

		CRC32_ProcessBuffer(&retval, &seedvalue, sizeof(int));
		CRC32_ProcessBuffer(&retval, &additionalSeed, sizeof(int));
		CRC32_ProcessBuffer(&retval, sharedname, strlen(sharedname));

		CRC32_Final(&retval);

		return static_cast<int>(retval);
	}

	__inline int SharedRandomInt(unsigned iseed, const char *sharedname, int iMinVal, int iMaxVal, int additionalSeed)
	{
		const int seed = SeedFileLineHash(iseed, sharedname, additionalSeed);
		I::UniformRandomStream->SetSeed(seed);
		return I::UniformRandomStream->RandomInt(iMinVal, iMaxVal);
	}

	__inline void RandomSeed(int iSeed)
	{
		static auto fnRandomSeed = reinterpret_cast<void(*)(uint32_t)>(GetProcAddress(GetModuleHandleA(VSTDLIB_DLL), "RandomSeed"));
		fnRandomSeed(iSeed);
	}

	__inline double PlatFloatTime()
	{
		static auto fnPlatFloatTime = reinterpret_cast<double(*)()>(GetProcAddress(GetModuleHandleA(TIER0_DLL), "Plat_FloatTime"));
		return fnPlatFloatTime();
	}

	__inline int RandomInt(int iMinVal = 0, int iMaxVal = 0x7FFF)
	{
		static auto fnRandomInt = reinterpret_cast<int(*)(int, int)>(GetProcAddress(GetModuleHandleA(VSTDLIB_DLL), "RandomInt"));
		return fnRandomInt(iMinVal, iMaxVal);
	}

	__inline float RandomFloat(float flMinVal = 0.0f, float flMaxVal = 1.0f)
	{
		static auto fnRandomFloat = reinterpret_cast<float(*)(float, float)>(GetProcAddress(GetModuleHandleA(VSTDLIB_DLL), "RandomFloat"));
		return fnRandomFloat(flMinVal, flMaxVal);
	}

	__inline bool VisPos(CBaseEntity *pSkip, const CBaseEntity *pEntity, const Vec3 &from, const Vec3 &to)
	{
		CGameTrace trace = {};
		CTraceFilterHitscan filter = {};
		filter.pSkip = pSkip;
		Trace(from, to, (MASK_SHOT | CONTENTS_GRATE), &filter, &trace);
		return ((trace.entity && trace.entity == pEntity) || trace.flFraction == 1.f);
	}

	__inline bool VisPosMask(CBaseEntity* pSkip, const CBaseEntity* pEntity, const Vec3& from, const Vec3& to, unsigned int nMask)
	{
		CGameTrace trace = {};
		CTraceFilterHitscan filter = {};
		filter.pSkip = pSkip;
		Trace(from, to, nMask, &filter, &trace);
		return ((trace.entity && trace.entity == pEntity) || trace.flFraction == 1.f);
	}

	__inline bool VisPosHitboxId(CBaseEntity *pSkip, const CBaseEntity *pEntity, const Vec3 &from, const Vec3 &to, int nHitbox)
	{
		CGameTrace trace = {};
		CTraceFilterHitscan filter = {};
		filter.pSkip = pSkip;
		Trace(from, to, (MASK_SHOT | CONTENTS_GRATE), &filter, &trace);
		return (trace.entity && trace.entity == pEntity && trace.hitbox == nHitbox);
	}

	__inline bool VisPosHitboxIdOut(CBaseEntity *pSkip, const CBaseEntity *pEntity, const Vec3 &from, const Vec3 &to, int &nHitboxOut)
	{
		CGameTrace trace = {};
		CTraceFilterHitscan filter = {};
		filter.pSkip = pSkip;
		Trace(from, to, (MASK_SHOT | CONTENTS_GRATE), &filter, &trace);

		if (trace.entity && trace.entity == pEntity) {
			nHitboxOut = trace.hitbox;
			return true;
		}

		return false;
	}

	__inline bool VisPosFraction(CBaseEntity *pSkip, const Vec3 &from, const Vec3 &to)
	{
		CGameTrace trace = {};
		CTraceFilterHitscan filter = {};
		filter.pSkip = pSkip;
		Trace(from, to, (MASK_SHOT | CONTENTS_GRATE), &filter, &trace);
		return (trace.flFraction > 0.99f);
	}

	__inline EWeaponType GetWeaponType(CBaseCombatWeapon *pWeapon)
	{
		if (!pWeapon)
		{
			return EWeaponType::UNKNOWN;
		}

		if (pWeapon->GetSlot() == EWeaponSlots::SLOT_MELEE || pWeapon->GetWeaponID() == TF_WEAPON_BUILDER)
		{
			return EWeaponType::MELEE;
		}

		switch (pWeapon->GetWeaponID())
		{
			case TF_WEAPON_ROCKETLAUNCHER:
			case TF_WEAPON_FLAME_BALL:
			case TF_WEAPON_GRENADELAUNCHER:
			case TF_WEAPON_FLAREGUN:
			case TF_WEAPON_COMPOUND_BOW:
			case TF_WEAPON_DIRECTHIT:
			case TF_WEAPON_CROSSBOW:
			case TF_WEAPON_PARTICLE_CANNON:
			case TF_WEAPON_DRG_POMSON:
			case TF_WEAPON_RAYGUN_REVENGE:
			case TF_WEAPON_RAYGUN:
			case TF_WEAPON_CANNON:
			case TF_WEAPON_SYRINGEGUN_MEDIC:
			case TF_WEAPON_SHOTGUN_BUILDING_RESCUE:
			case TF_WEAPON_FLAMETHROWER:
			case TF_WEAPON_CLEAVER:
			case TF_WEAPON_PIPEBOMBLAUNCHER:
			{
				return EWeaponType::PROJECTILE;
			}

			default:
			{
				const int nDamageType = pWeapon->GetDamageType();

				if (nDamageType & DMG_BULLET || nDamageType && DMG_BUCKSHOT)
				{
					return EWeaponType::HITSCAN;
				}

				break;
			}
		}

		if (G::CurItemDefIndex == Heavy_s_RoboSandvich ||
			G::CurItemDefIndex == Heavy_s_Sandvich ||
			G::CurItemDefIndex == Heavy_s_FestiveSandvich ||
			G::CurItemDefIndex == Heavy_s_Fishcake ||
			G::CurItemDefIndex == Heavy_s_TheDalokohsBar ||
			G::CurItemDefIndex == Heavy_s_SecondBanana) {
			return EWeaponType::HITSCAN;
		}

		return EWeaponType::UNKNOWN;
	}

	__inline uintptr_t GetVirtual(void* pBaseClass, unsigned int nIndex)
	{
		return static_cast<uintptr_t>((*static_cast<int**>(pBaseClass))[nIndex]);
	}

	__inline bool IsAttacking(const CUserCmd *pCmd, CBaseCombatWeapon *pWeapon)
	{
		if (pWeapon->GetSlot() == SLOT_MELEE)
		{
			if (pWeapon->GetWeaponID() == TF_WEAPON_KNIFE)
			{
				return ((pCmd->buttons & IN_ATTACK) && G::WeaponCanAttack);
			}

			return fabs(pWeapon->GetSmackTime() - I::GlobalVars->curtime) < I::GlobalVars->interval_per_tick * 2.0f;
		}


		if (G::CurItemDefIndex == Soldier_m_TheBeggarsBazooka)
		{
			static bool bLoading = false;

			if (pWeapon->GetClip1() > 0)
			{
				bLoading = true;
			}

			if (!(pCmd->buttons & IN_ATTACK) && bLoading) {
				bLoading = false;
				return true;
			}
		}
		else
		{
			const int id = pWeapon->GetWeaponID();
			switch (id) {
			case TF_WEAPON_COMPOUND_BOW:
			case TF_WEAPON_PIPEBOMBLAUNCHER:
			case TF_WEAPON_STICKY_BALL_LAUNCHER:
			case TF_WEAPON_GRENADE_STICKY_BALL:
			case TF_WEAPON_CANNON:
				{
					static bool bCharging = false;

					if (pWeapon->GetChargeBeginTime() > 0.0f)
						bCharging = true;

					if (!(pCmd->buttons & IN_ATTACK) && bCharging) {
						bCharging = false;
						return true;
					}
					break;
				}
			case TF_WEAPON_JAR:
			case TF_WEAPON_JAR_MILK:
			case TF_WEAPON_JAR_GAS:
			case TF_WEAPON_GRENADE_JAR_GAS:
			case TF_WEAPON_CLEAVER:
				{
					static float flThrowTime = 0.0f;

					if ((pCmd->buttons & IN_ATTACK) && G::WeaponCanAttack && !flThrowTime)
					{
						flThrowTime = I::GlobalVars->curtime + I::GlobalVars->interval_per_tick;
					}

					if (flThrowTime && I::GlobalVars->curtime >= flThrowTime) {
						flThrowTime = 0.0f;
						return true;
					}
					break;
				}
			default:
				{
					if ((pCmd->buttons & IN_ATTACK) && G::WeaponCanAttack)
					{ return true; }
					break;
				}
			}
		}

		return false;
	}

	__inline Vector ComputeMove(const CUserCmd* pCmd, CBaseEntity* pLocal, Vec3& a, Vec3& b)
	{
		const Vec3 diff = (b - a);
		if (diff.Length() == 0.0f)
		{
			return {0.0f, 0.0f, 0.0f};
		}
		const float x = diff.x;
		const float y = diff.y;
		const Vec3 vSilent(x, y, 0);
		Vec3 ang;
		Math::VectorAngles(vSilent, ang);
		const float yaw = DEG2RAD(ang.y - pCmd->viewangles.y);
		const float pitch = DEG2RAD(ang.x - pCmd->viewangles.x);
		Vec3 move = { cos(yaw) * 450.0f, -sin(yaw) * 450.0f, -cos(pitch) * 450.0f };

		// Only apply upmove in water
		if (!(I::EngineTrace->GetPointContents(pLocal->GetEyePosition()) & CONTENTS_WATER))
		{
			move.z = pCmd->upmove;
		}
		return move;
	}

	__inline void WalkTo(CUserCmd* pCmd, CBaseEntity* pLocal, Vec3& a, Vec3& b, float scale)
	{
		// Calculate how to get to a vector
		const auto result = ComputeMove(pCmd, pLocal, a, b);

		// Push our move to usercmd
		pCmd->forwardmove = result.x * scale;
		pCmd->sidemove = result.y * scale;
		pCmd->upmove = result.z * scale;
	}

	__inline void StopMovement(CUserCmd* pCmd, bool safe = true) {
		if (safe && G::IsAttacking) { return; }

		if (CBaseEntity* pLocal = g_EntityCache.GetLocal()) {
			const float direction = Math::VelocityToAngles(pLocal->m_vecVelocity()).y;
			pCmd->viewangles.x = -90;	//	on projectiles we would be annoyed if we shot the ground.
			pCmd->viewangles.y = direction;
			pCmd->viewangles.z = 0;
			pCmd->sidemove = 0; pCmd->forwardmove = 0;
			G::ShouldStop = false;
		}
	}

	__inline void ConLog(const char* cFunction, const char* cLog, Color_t cColour, const bool bShouldPrint){
		if (!bShouldPrint) { return; }
		I::Cvar->ConsoleColorPrintf(cColour, "[%s] ", cFunction);
		I::Cvar->ConsoleColorPrintf({ 255, 255, 255, 255 }, "%s\n", cLog);
	}

	__inline void WalkTo(CUserCmd* pCmd, CBaseEntity* pLocal, Vec3& pDestination)
	{
		Vec3 localPos = pLocal->GetVecOrigin();
		WalkTo(pCmd, pLocal, localPos, pDestination, 1.f);
	}

	__inline void BlockMovement(CUserCmd* pCmd)
	{
		pCmd->forwardmove = 0.f;
		pCmd->sidemove = 0.f;
		pCmd->upmove = 0.f;
	}

	__inline int HandleToIDX(int pHandle)
	{
		return pHandle & 0xFFF;
	}

	// A function to find a weapon by WeaponID
	__inline int GetWeaponByID(CBaseEntity* pPlayer, int pWeaponID)
	{
		// Invalid player
		if (!pPlayer) { return -1; }

		const size_t* hWeapons = pPlayer->GetMyWeapons();
		// Go through the handle array and search for the item
		for (int i = 0; hWeapons[i]; i++)
		{
			if (!(HandleToIDX(hWeapons[i]) >= 0 && HandleToIDX(hWeapons[i]) <= 2049 && HandleToIDX(hWeapons[i]) < 2048))
			{
				continue;
			}
			// Get the weapon
			auto* weapon = reinterpret_cast<CBaseCombatWeapon*>(I::ClientEntityList->GetClientEntityFromHandle(HandleToIDX(hWeapons[i])));
			// if weapon is what we are looking for, return true
			if (weapon && weapon->GetWeaponID() == pWeaponID)
			{
				return weapon->GetIndex();
			}
		}
		// Nothing found
		return -1;
	}

	// Returns the teleporter exit of a given owner
	__inline bool GetTeleporterExit(int ownerIdx, Vec3* out)
	{
		const auto& buildings = g_EntityCache.GetGroup(EGroupType::BUILDINGS_ALL);

		for (const auto& pBuilding : buildings)
		{
			if (!pBuilding->IsAlive()) { continue; }

			const auto& building = reinterpret_cast<CBaseObject*>(pBuilding);
			const auto nType = static_cast<EBuildingType>(building->GetType());

			if (nType == EBuildingType::TELEPORTER && building->GetObjectMode() == 1 && building->GetOwner()->GetIndex() == ownerIdx)
			{
				*out = building->GetAbsOrigin();
				return true;
			}
		}

		return false;
	}

	__inline int GetPlayerForUserID(int userID)
	{
		for (int i = 1; i <= I::EngineClient->GetMaxClients(); i++)
		{
			PlayerInfo_t playerInfo{};
			if (!I::EngineClient->GetPlayerInfo(i, &playerInfo))
			{
				continue;
			}
			// Found player
			if (playerInfo.userID == userID)
			{
				return i;
			}
		}
		return 0;
	}

	__inline bool WillProjectileHit(CBaseEntity* pLocal, CBaseCombatWeapon* pWeapon, const Vec3& targetPos)
	{
		CGameTrace trace{};
		static CTraceFilterWorldAndPropsOnly traceFilter = {};
		traceFilter.pSkip = pLocal;

		Vec3 shootPos = pLocal->GetEyePosition();
		const Vec3 aimAngles = Math::CalcAngle(shootPos, targetPos);

		{
			switch (pWeapon->GetWeaponID())
			{
			case TF_WEAPON_RAYGUN_REVENGE:
			case TF_WEAPON_ROCKETLAUNCHER:
			case TF_WEAPON_DIRECTHIT:
			{
				Vec3 vecOffset(23.5f, 12.0f, -3.0f); //tf_weaponbase_gun.cpp @L529 & @L760
				if (G::CurItemDefIndex == Soldier_m_TheOriginal)
				{
					vecOffset.y = 0.f;
				}
				if (pLocal->IsDucking())
				{
					vecOffset.z = 8.0f;
				}
				GetProjectileFireSetup(pLocal, aimAngles, vecOffset, &shootPos);
				break;
			}
			case TF_WEAPON_SYRINGEGUN_MEDIC:
			{
				const Vec3 vecOffset(16.f, 6.f, -8.f); //tf_weaponbase_gun.cpp @L628
				GetProjectileFireSetup(pLocal, aimAngles, vecOffset, &shootPos);
				break;
			}
			case TF_WEAPON_COMPOUND_BOW:
			{
				const Vec3 vecOffset(23.5f, 12.0f, -3.0f); //tf_weapon_grapplinghook.cpp @L355 ??
				GetProjectileFireSetup(pLocal, aimAngles, vecOffset, &shootPos);
				break;
			}
			case TF_WEAPON_RAYGUN:
			case TF_WEAPON_PARTICLE_CANNON:
			case TF_WEAPON_DRG_POMSON:
			{
				Vec3 vecOffset(23.5f, -8.0f, -3.0f); //tf_weaponbase_gun.cpp @L568
				if (pLocal->IsDucking())
				{
					vecOffset.z = 8.0f;
				}
				GetProjectileFireSetup(pLocal, aimAngles, vecOffset, &shootPos);
				break;
			}
			case TF_WEAPON_GRENADELAUNCHER:
			case TF_WEAPON_PIPEBOMBLAUNCHER:
			case TF_WEAPON_STICKBOMB:
			case TF_WEAPON_STICKY_BALL_LAUNCHER:
			{
				// TODO: Implement this
				break;
			}
			default: break;
			}
		}

		TraceHull(shootPos, targetPos, Vec3(-3.8f, -3.8f, -3.8f), Vec3(3.8f, 3.8f, 3.8f), MASK_SHOT_HULL, &traceFilter, &trace);
		return !trace.DidHit();
	}

	__inline Vec3 GetHeadOffset(CBaseEntity* pEntity)
	{
		const Vec3 headPos = pEntity->GetHitboxPos(HITBOX_HEAD);
		const Vec3 entPos = pEntity->GetAbsOrigin();
		const Vec3 delta = entPos - headPos;
		return delta * -1.f;
	}
}

namespace Particles {
	inline void DispatchEffect(const char* pName, const CEffectData& data)
	{
		using FN = int(__cdecl*)(const char*, const CEffectData&);
		static FN fnDispatchEffect = S::CTE_DispatchEffect.As<FN>();
		fnDispatchEffect(pName, data);
	}

	inline int GetParticleSystemIndex(const char* pParticleSystemName)
	{
		using FN = int(__cdecl*)(const char*);
		static FN fnGetParticleSystemIndex = S::GetParticleSystemIndex.As<FN>();
		return fnGetParticleSystemIndex(pParticleSystemName);
	}

	inline void DispatchParticleEffect(int iEffectIndex, const Vector& vecOrigin, const Vector& vecStart, const Vector& vecAngles, CBaseEntity* pEntity = nullptr)
	{
		CEffectData data{};
		data.m_nHitBox = iEffectIndex;
		data.m_vOrigin = vecOrigin;
		data.m_vStart = vecStart;
		data.m_vAngles = vecAngles;

		if (pEntity) {
			data.m_nEntIndex = pEntity->GetIndex();
			data.m_fFlags |= (1 << 0);
			data.m_nDamageType = 2;
		}
		else {
			data.m_nEntIndex = 0;
		}

		data.m_bCustomColors = true;

		DispatchEffect("ParticleEffect", data);
	}

	inline void DispatchParticleEffect(const char* pszParticleName, const Vec3& vecOrigin, const Vec3& vecAngles, CBaseEntity* pEntity = nullptr)
	{
		const int iIndex = GetParticleSystemIndex(pszParticleName);
		DispatchParticleEffect(iIndex, vecOrigin, vecOrigin, vecAngles, pEntity);
	}

	inline void ParticleTracer(const char* pszTracerEffectName, const Vector& vecStart, const Vector& vecEnd, int iEntIndex, int iAttachment, bool bWhiz) {
		using FN = void(__cdecl*)(const char*, const Vec3&, const Vec3&, int, int, bool);
		static auto fnParticleTracer = S::UTIL_ParticleTracer.As<FN>();
		fnParticleTracer(pszTracerEffectName, vecStart, vecEnd, iEntIndex, iAttachment, bWhiz);
	}
}
