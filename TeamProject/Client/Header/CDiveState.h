#pragma once
#include "IVellumState.h"

enum class DivePhase { Ready, In, Wait, Out, End };
class CTestTile;

class CDiveState : public IVellumState
{
public:
    virtual void Enter(CVellum* pVellum)override;
    virtual void Update(const _float fTimeDelta, CVellum* pVellum)override;
    virtual void Exit(CVellum* pVellum)override;

    CTestTile* Calc_Tile(const _vec3& pos, CVellum* pVellum);

private:
    DivePhase m_eDPhase = DivePhase::Ready;
    _float m_fSearch = 0.f;
    int m_iCnt = 0;

    _float m_fSpeed = 0.f;
    _float m_fPartRatio = 0.f;
};

