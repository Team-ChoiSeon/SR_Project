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

    _float      m_fPhaseTime = 0.f;    // ���� �ܰ� �ð�
    _float      m_fChargeTime = 3.f;  // ���� �ܰ� �ð�
    _float      m_fFireTime = 5.f;     // �߻� �ܰ� �ð�
    _float      m_fCoolTime = 1.f;     // �ĵ����� �ð�

    // �߻翡 �ʿ��� ����
    _vec3       m_vPos;                // �߻� ���� ��ġ
    _vec3       m_vBasePos;
    _vec3       m_vDir;                // �߻� ����
    bool        m_bFire = false;
    int m_iFireCnt = 0;
    float m_fFireDelay = 1.f;
};

