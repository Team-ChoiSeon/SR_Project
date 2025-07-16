#pragma once
#include "IVellumState.h"
class CSpinState : public IVellumState
{
public:
    virtual void Enter(CVellum* pVellum)override;
    virtual void Update(const _float fTimeDelta, CVellum* pVellum)override;
    virtual void Exit(CVellum* pVellum)override;

private:
    _float m_fSpeed = 0.f;
    _float   m_fPatternTime = 0.f;
    _float   m_fSwitchTime = 3.f;
};

