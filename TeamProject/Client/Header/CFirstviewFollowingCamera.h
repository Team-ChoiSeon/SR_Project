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
	explicit CFirstviewFollowingCamera(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CFirstviewFollowingCamera(const CFirstviewFollowingCamera& rhs);
	virtual ~CFirstviewFollowingCamera();

public:
	//Basic Function
	HRESULT		Ready_GameObject() override;
	int			Update_GameObject(const _float& fTimeDelta) override;
	void		LateUpdate_GameObject(const _float& fTimeDelta) override;
	void		Render_GameObject() override;

	//Create, Release Function
	static		CFirstviewFollowingCamera* Create(LPDIRECT3DDEVICE9 pGraphicDev);
	void		Free();

	//Getter, Setter Function
	void		Set_Target(CGameObject* target) { 
		m_pCamera->Set_Target(target);
		m_pTargetTransform = m_pCamera->Get_Target()->Get_Component<CTransform>();
	}
	virtual _matrix	Get_ViewMatrix() { return *m_pCamera->Get_ViewMatrix(); }
	virtual _matrix	Get_ProjectionMatrix() { return *m_pCamera->Get_ProjectionMatrix(); }

	void Start_Shake(const _float fDuration, const _float fMagnitude);

protected:

private:
	CTransform*				m_pTransform;
	CTransform*				m_pTargetTransform;
	CCamera*				m_pCamera;

	bool					m_bCursorMove = false;

	// 카메라 흔들기
	_float m_fShakeDuration = 0.f;  
	_float m_fShakeMagnitude = 0.f;
	_float m_fShakeTime = 0.f;     
	
}FFCam;

