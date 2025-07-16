#pragma once
#include "IVellumState.h"

enum class FirePhase { Prepare, Charge, Fire, Cooldown };
class CFireState : public IVellumState
{
public:
	virtual void Enter(CVellum* pVellum)override;
	virtual void Update(const _float fTimeDelta, CVellum* pVellum)override;
	virtual void Exit(CVellum* pVellum)override;

private:
	FirePhase m_ePhase = FirePhase::Prepare;

    _float      m_fPhaseTime = 0.f;    // 현재 단계 시간
    _float      m_fChargeTime = 3.f;  // 충전 시간
    _float      m_fFireTime = 1.5f;     // 발사 지속 시간
    _float      m_fCoolTime = 1.f;     // 후딜레이 시간

    // 발사에 필요한 정보
    _vec3       m_vPos;                // 발사 시작 위치
    _vec3       m_vBasePos;
    _vec3       m_vDir;                // 발사 방향
};

