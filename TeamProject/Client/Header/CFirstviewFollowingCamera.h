#pragma once
#include "CCameraObject.h"
#include "CGameObject.h"

namespace Engine {
	class CTransform;
	class CCamera;
}
typedef class CFirstviewFollowingCamera : public CCameraObject
{
private:
	explicit CFirstviewFollowingCamera(LPDIRECT3DDEVICE9 pGraphicDev, CGameObject* target);
	explicit CFirstviewFollowingCamera(const CFirstviewFollowingCamera& rhs);
	virtual ~CFirstviewFollowingCamera();

public:
	//Basic Function
	HRESULT		Ready_GameObject() override;
	int			Update_GameObject(const _float& fTimeDelta) override;
	void		LateUpdate_GameObject(const _float& fTimeDelta) override;
	void		Render_GameObject() override;

	//Create, Release Function
	static		CFirstviewFollowingCamera* Create(LPDIRECT3DDEVICE9 pGraphicDev, CGameObject* target);
	void		Free();

	//Optional Function
	void		CursorRotate();
	void		AngleClamping();

protected:

private:
	CGameObject*			m_pFollowingTarget;
	CTransform*				m_pTransform;
	CCamera*				m_pCamera;

	bool					m_bCursorMove = false;
	
}FFCam;

