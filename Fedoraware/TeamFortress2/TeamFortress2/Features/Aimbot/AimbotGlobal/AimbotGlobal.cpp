#include "AimbotGlobal.h"

#include "../../Vars.h"

namespace SandvichAimbot
{
	bool bIsSandvich;

	bool IsSandvich()
	{
		return (G::CurItemDefIndex == Heavy_s_RoboSandvich ||
			G::CurItemDefIndex == Heavy_s_Sandvich ||
			G::CurItemDefIndex == Heavy_s_FestiveSandvich ||
			G::CurItemDefIndex == Heavy_s_Fishcake ||
			G::CurItemDefIndex == Heavy_s_TheDalokohsBar ||
			G::CurItemDefIndex == Heavy_s_SecondBanana);
	}

	void RunSandvichAimbot(CBaseEntity* pLocal, CBaseCombatWeapon* pWeapon, CUserCmd* pCmd, CBaseEntity* pTarget)
	{
		const int nWeaponID = pWeapon->GetWeaponID();
		const bool bShouldAim = (Vars::Aimbot::Global::AimKey.Value == VK_LBUTTON
			                         ? (pCmd->buttons & IN_ATTACK)
			                         : F::AimbotGlobal.IsKeyDown());

		if (bShouldAim)
		{
			Vec3 tr = pTarget->GetAbsOrigin() - pLocal->GetEyePosition();
			Vec3 angle;
			Math::VectorAngles(tr, angle);
			Math::ClampAngles(angle);
			pCmd->viewangles = angle;
			pCmd->buttons |= IN_ATTACK2;
			G::HitscanSilentActive = true;
		}
	}
}

bool CAimbotGlobal::IsKeyDown()
{
	static KeyHelper aimKey{ &Vars::Aimbot::Global::AimKey.Value };
	return !Vars::Aimbot::Global::AimKey.Value ? true : aimKey.Down();
}

void CAimbotGlobal::SortTargets(const ESortMethod& Method)
{
	// Sort by preference
	std::sort(m_vecTargets.begin(), m_vecTargets.end(), [&](const Target_t& a, const Target_t& b) -> bool {
		switch (Method)
		{
		case ESortMethod::FOV: return (a.m_flFOVTo < b.m_flFOVTo);
		case ESortMethod::DISTANCE: return (a.m_flDistTo < b.m_flDistTo);
		default: return false;
		}
	});

	// Sort by priority
	std::sort(m_vecTargets.begin(), m_vecTargets.end(), [&](const Target_t& a, const Target_t& b) -> bool {
		return (a.n_Priority.Mode > b.n_Priority.Mode);
	});
}

const Target_t& CAimbotGlobal::GetBestTarget(const ESortMethod& Method)
{
	return *std::min_element(m_vecTargets.begin(), m_vecTargets.end(), [&](const Target_t& a, const Target_t& b) -> bool {
		if (a.n_Priority.Mode < b.n_Priority.Mode) { return false; }

		switch (Method)
		{
		case ESortMethod::FOV: return (a.m_flFOVTo < b.m_flFOVTo);
		case ESortMethod::DISTANCE: return (a.m_flDistTo < b.m_flDistTo);
		default: return false;
		}
	});
}

bool CAimbotGlobal::ShouldIgnore(CBaseEntity* pTarget, bool hasMedigun)
{
	CBaseEntity* pLocal = g_EntityCache.GetLocal();
	CBaseCombatWeapon* pWeapon = g_EntityCache.GetWeapon();

	PlayerInfo_t pInfo{};
	if (!pTarget) { return true; }
	if (!I::EngineClient->GetPlayerInfo(pTarget->GetIndex(), &pInfo)) { return true; }
	if (pTarget->GetDormant()) { return true; }
	if (Vars::Aimbot::Global::IgnoreOptions.Value & (INVUL) && !pTarget->IsVulnerable()) { return true; }
	if (Vars::Aimbot::Global::IgnoreOptions.Value & (CLOAKED) && pTarget->IsVisible()) { return true; }
	if (Vars::Aimbot::Global::IgnoreOptions.Value & (DEADRINGER) && pTarget->GetFeignDeathReady()) { return true; }
	if (Vars::Aimbot::Global::IgnoreOptions.Value & (TAUNTING) && pTarget->IsTaunting()) { return true; }

	// Special conditions for mediguns //
	if (!hasMedigun || (pLocal && pLocal->GetTeamNum() != pTarget->GetTeamNum()))
	{
		if (G::IsIgnored(pInfo.friendsID)) { return true; }
		if (Vars::Aimbot::Global::IgnoreOptions.Value & (FRIENDS) && g_EntityCache.IsFriend(pTarget->GetIndex())) { return true; }
	}

	if (Vars::Aimbot::Global::IgnoreOptions.Value & (VACCINATOR))
	{
		switch (G::CurWeaponType)
		{
		case EWeaponType::HITSCAN:
		{
			if (G::CurItemDefIndex != Spy_m_TheEnforcer && pTarget->IsBulletResist())
				return true;

			break;
		}
		case EWeaponType::PROJECTILE:
		{
			if (pWeapon->GetWeaponID() == TF_WEAPON_FLAMETHROWER || pWeapon->GetWeaponID() == TF_WEAPON_FLAREGUN)
			{
				if (pTarget->IsFireResist())
					return true;
			}
			else if (pWeapon->GetWeaponID() == TF_WEAPON_COMPOUND_BOW)//Right?
			{
				if (pTarget->IsBulletResist())
					return true;
			}
			else
			{
				if (pTarget->IsBlastResist())
					return true;
			}

			break;
		}
		default: break;
		}
	}

	return false;
}
