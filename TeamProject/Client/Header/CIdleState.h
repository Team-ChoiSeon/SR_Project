#pragma once
#include "IVellumState.h"

enum class VPattern { CHASE, DIVE, SPIN, FIRE, NONE };

class CIdleState : public IVellumState
{
public:
    virtual void Enter(CVellum* pVellum)override;
    virtual void Update(const _float fTimeDelta, CVellum* pVellum)override;
    virtual void Exit(CVellum* pVellum)override;

private:
    _float   m_fPatternTime = 0.f;
    _float   m_fSwitchTime = 3.f;
    VPattern m_ePattern = VPattern::NONE;
    IVellumState* m_pNext = nullptr;

    static int m_iPatternIdx;
};

