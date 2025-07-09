#pragma once
#include "CCube.h"

namespace Engine {
	class CCollider;
	class CRigidBody;
	class CPickTarget;
}
class CDirectionalCube : public CCube
{
private:
	explicit CDirectionalCube(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CDirectionalCube(const CDirectionalCube& rhs);
	virtual ~CDirectionalCube();

public:
	//Basic Function
	HRESULT Ready_GameObject()override;
	_int Update_GameObject(const _float& fTimeDelta) override;
	void LateUpdate_GameObject(const _float& fTimeDelta) override;

	//Create, Release Function
	static CDirectionalCube* Create(LPDIRECT3DDEVICE9 pGraphicDev);
	void Free() override;

	//Getter, Setter Function
	void Set_Info(const _vec3& startpos,  const _vec3& axis, const _float& mindistance, const _float& maxdistance);
	void Set_Info(const _vec3& startpos, const _vec3& direction, const _float& maxdistance);
	
private:
	//Compute Function
	void ComputeMoveVecIntoAxisMoveVec();
	void ComputeMinMaxPos();
	void ComputeEndPos();
	void Move();

	//Variables
	CCollider*		m_pCollider;
	CPickTarget*	m_pPick;
	CRigidBody*		m_pRigid;

	_vec3			m_vStartPos;
	_vec3			m_vDefaultAxis;
	_vec3			m_vMoveDelta;
	_vec3			m_vMinPos;
	_vec3			m_vMaxPos;
	_float			m_fMaxDistance;
	_float			m_fMinDistance;

	_float			m_fCurDistance;

	_bool			m_bOneway;
	
};

