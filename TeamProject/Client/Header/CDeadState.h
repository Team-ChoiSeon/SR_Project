#pragma once
#include "IVellumState.h"
class CDeadState : public IVellumState
{
public:
	virtual void Enter(CVellum* pVellum)override;
	virtual void Update(const _float fTimeDelta, CVellum* pVellum)override;
	virtual void Exit(CVellum* pVellum)override;

private:
	_float m_fDeadTime = 0.f; // 누적시간
	_float m_fDuration = 3.f; // 지속시간
	bool m_bFire = false;

};

