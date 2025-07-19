#pragma once
#include "IVellumState.h"
enum class IntroPhase {Orbit, Charge, Roar};
class CIntroState : public IVellumState
{
public:
    virtual void Enter(CVellum* pVellum)override;
    virtual void Update(const _float fTimeDelta, CVellum* pVellum)override;
    virtual void Exit(CVellum* pVellum)override;

private:
    IntroPhase m_ePhase = IntroPhase::Orbit;
    _float m_fRad = 0.f;
    _float m_fASpeed = 0.f;
    _float m_fFreq = 0.f;
    _float m_fAmp = 0.f;
    _float m_fAngle = 0.f;

    _float m_fChargeTime = 5.f;
    _float m_fRoarTime = 0.f;
    _float m_fPhaseTime = 0.f;

    _vec3       m_vBase;
    _vec3       m_vDir;                // 발사 방향
};