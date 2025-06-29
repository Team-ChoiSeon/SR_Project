#pragma once
#include "CCameraObject.h"

class TestFollowingCam : public CCameraObject
{
public:
	explicit TestFollowingCam(LPDIRECT3DDEVICE9 pGraphicDev, CGameObject* target, _vec3 distance);
	explicit TestFollowingCam(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit TestFollowingCam(const TestFollowingCam& rhs);
	virtual ~TestFollowingCam();
	
	HRESULT		Ready_Fcam();
	void		Update_FCam(const _float& fTimeDelta);
	void		LateUpdate_FCam();
	void		Render_FCam();
	void		ComputeCamera();


	virtual TestFollowingCam* Clone() override;
	static TestFollowingCam* Create(LPDIRECT3DDEVICE9 pGraphicDev,CGameObject* target,_vec3 distance);
	static TestFollowingCam* Create(LPDIRECT3DDEVICE9 pGraphicDev);
protected:

private:

	CGameObject*			m_pFollowingTarget;
	_vec3					m_vTargetDistance;
};