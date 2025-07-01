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
	HRESULT Ready_GameObject();
	int Update_GameObject(const _float& fTimeDelta) override;
	void LateUpdate_GameObject(const _float& fTimeDelta) override;
	void Render_GameObject() override;

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

