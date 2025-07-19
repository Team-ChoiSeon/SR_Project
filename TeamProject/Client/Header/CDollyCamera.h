#pragma once
#include "CCameraObject.h"

namespace Engine {
	class CTransform;
	class CCamera;
}

class CDollyCamera : public CCameraObject
{
private:
	explicit CDollyCamera(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CDollyCamera(const CDollyCamera& rhs);
	virtual ~CDollyCamera();

public:
	HRESULT Ready_GameObject() override;
	int Update_GameObject(const _float& fTimeDelta) override;
	void LateUpdate_GameObject(const _float& fTimeDelta) override;

	static CDollyCamera* Create(LPDIRECT3DDEVICE9 pGraphicDev);
	void Free();

	void Start_Dolly(CGameObject* pTarget, const _vec3& moveOffset, _float fovDelta, _float duration);
	void End_Dolly();

	void Set_Pos(const _vec3& pos);
	void Set_Angle(const _vec3& angle);

	bool Get_DollyEnd() { return m_bDolly; }

private:
	CTransform* m_pTransform = nullptr;
	CTransform* m_pTargetTransform;
	CCamera* m_pCamera = nullptr;

	_vec3 m_vTargetPos;

	_vec3 m_StartPos;
	_vec3 m_MiddlePos;
	_vec3 m_EndPos;
	_vec3 m_MoveOffset;

	_float m_OriginFov = D3DX_PI * 0.25f;
	_float m_fDollyFovDelta = 0.f;

	_float m_fDuration = 0.f;
	_float m_fTimer = 0.f;

	int m_CurPhase = 0;
	bool m_bDolly = false;
};
