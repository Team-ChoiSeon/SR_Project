#pragma once
#include "CCameraObject.h"

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

	void Start_Cinematic(const _vec3& startPos, const _vec3& endPos, const _vec3& endLook, _float fov, _float duration);
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

	_vec3 m_StartPos;
	_vec3 m_EndPos;

	_vec3 m_StartLookDir;
	_vec3 m_EndLookDir;

	_float m_fCinematicFov = D3DX_PI * 0.15f;
	_float m_fDuration = 0.f;
	_float m_fTimer = 0.f;
	bool   m_bCinematic = false;

};
