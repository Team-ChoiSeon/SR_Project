#pragma once
#include "CCube.h"
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

	//Getter, Setter Function
	void Set_StartPos(const _vec3& vStartPos) { m_vStartPos = vStartPos; }
	void Set_Direction(const _vec3& vDirection) { m_vDirection = vDirection; }
	void Set_MaxDistance(const _float& fMaxDistance) { m_fMaxDistance = fMaxDistance; }
	void Set_Speed(const _float& speed) { m_fSpeed = speed; }

private:
	//ComputeFunction
	void ComputeEndPos();
	void Move(const _float& fTimeDelta);
	void Sleep();

	//Variables
	_vec3 m_vStartPos;
	_vec3 m_vDirection;
	_vec3 m_vEndPos;
	_float m_fMaxDistance = 5.f;
	_float m_fSpeed = 10.f;

	bool m_bOn = false;	
	bool m_bGoBack = false;
	bool m_bBackward = false;
};

