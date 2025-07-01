#pragma once
#include "CCameraObject.h"
#include "CGameObject.h"

namespace Engine {
	class CTransform;
	class CCamera;
}
typedef class CFirstviewFollowingCamera : public CCameraObject
{
public:
	explicit CFirstviewFollowingCamera(LPDIRECT3DDEVICE9 pGraphicDev, CGameObject* target);
	explicit CFirstviewFollowingCamera(const CFirstviewFollowingCamera& rhs);
	virtual ~CFirstviewFollowingCamera();

	HRESULT		Ready_FFCam();
	void		Update_FFCam(const float& fTimeDelta);
	void		LateUpdate_FFCam();
	void		Render_FFCam();
	void		CursorRotate();
	void		AngleClamping();
	static CFirstviewFollowingCamera* Create(LPDIRECT3DDEVICE9 pGraphicDev, CGameObject* target);
	void		Free();

protected:

private:
	CGameObject*			m_pFollowingTarget;
	CTransform*				m_pTransform;
	CCamera*				m_pCamera;

	bool					m_bCursorMove = false;
	
}FFCam;

