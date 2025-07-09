#pragma once
#include "CCube.h"

namespace Engine
{
	class CCollider;
	class CRigidBody;
}
class CFloatingCube : public CCube
{
private:
	explicit CFloatingCube(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CFloatingCube(const CFloatingCube& rhs);
	virtual ~CFloatingCube();

public:
	//Basic Function
	HRESULT Ready_GameObject()override;
	_int Update_GameObject(const _float& fTimeDelta) override;
	void LateUpdate_GameObject(const _float& fTimeDelta) override;

	//Create, Release Function
	static CFloatingCube* Create(LPDIRECT3DDEVICE9 pGraphicDev);
	void Free() override;

	void SetTrigger(bool Trigger) { m_bOn = Trigger; }

	//Getter, Setter Function
	void Set_StartPos(const _vec3& vStartPos) { m_vStartPos = vStartPos; }
	void Set_EndPos(const _vec3& vEndPos) { m_vEndPos = vEndPos; }
	void Set_Direction(const _vec3& vDirection) { m_vDirection = vDirection; }
	void Set_MaxDistance(const _float& fMaxDistance) { m_fMaxDistance = fMaxDistance; }
	void Set_Speed(const _float& speed) { m_fSpeed = speed; }
	void Set_SleepTime(const _float& SleepTime) { m_fSleepTime = SleepTime; }
	void Set_Info(const _vec3& vStartPos, const _vec3& vDirection, const _float& fMax, const _float& fSpeed, const _float& SleepTime);
	void Set_Loop() { m_bBackward = true; }


private:
	//Compute Function
	void ComputeEndPos();
	void Move(const _float& fTimeDelta);
	void MoveBack(const _float& fTimeDelta);
	void Sleep(const _float fTimeDelta);

	//Variables
	CCollider* m_pCollider;
	CRigidBody* m_pRigid;

	_vec3 m_vStartPos;
	_vec3 m_vDirection;
	_vec3 m_vEndPos;
	_vec3 m_vConstVelPos;
	_vec3 m_vDescVelPos;
	_float m_fMaxDistance = 5.f;
	_float m_fSpeed = 10.f;
	_float m_fSleepTime;
	_float m_fTime = 0.f;

	bool m_bOn = false;	
	bool m_bGoBack = false;
	bool m_bBackward = false;
};

