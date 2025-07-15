#pragma once
#include "CCameraObject.h"
#include "CImageUIObject.h"

namespace Engine {
	class CTransform;
	class CCamera;
}

class CCinematicCamera : public CCameraObject
{
private:
	explicit CCinematicCamera(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CCinematicCamera(const CCinematicCamera& rhs);
	virtual ~CCinematicCamera();

public:
	HRESULT Ready_GameObject() override;
	int Update_GameObject(const _float& fTimeDelta) override;
	void LateUpdate_GameObject(const _float& fTimeDelta) override;

	static CCinematicCamera* Create(LPDIRECT3DDEVICE9 pGraphicDev);
	void Free();

	void Start_Cinematic();
	void End_Cinematic();
	
	void Set_Target(CGameObject* target) {
		m_pCamera->Set_Target(target);
		m_pTargetTransform = m_pCamera->Get_Target()->Get_Component<CTransform>();
	}
	void Set_Pos(const _vec3& pos);
	void Set_Angle(const _vec3& angle);
	
	bool Get_CinematicEnd() { return m_bCinematic; }

private:
	CTransform* m_pTransform = nullptr;
	CTransform* m_pTargetTransform;
	CCamera* m_pCamera = nullptr;

	float m_fbarMove = 0.f;
	bool m_bCinematic = false;

	CImageUIObject* m_pBlackBarTop = nullptr;
	CImageUIObject* m_pBlackBarBottom = nullptr;
};
