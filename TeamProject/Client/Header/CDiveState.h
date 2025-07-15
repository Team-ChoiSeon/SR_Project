#pragma once
#include "IVellumState.h"

enum class DivePhase { Ready, In, Wait, Out, End };

class CDiveState : public IVellumState
{
public:
    virtual void Enter(CVellum* pVellum)override;
    virtual void Update(const _float fTimeDelta, CVellum* pVellum)override;
    virtual void Exit(CVellum* pVellum)override;

private:
    DivePhase m_eDPhase = DivePhase::Ready;
    _float m_fSearch = 0.f;
};

