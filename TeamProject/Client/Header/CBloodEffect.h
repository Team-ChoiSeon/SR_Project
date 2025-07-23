#pragma once
#include "CGameObject.h"
#include "UiPanel.h"

namespace Engine {
	class CTransform;
	class CUiQuad;
	class CCamera;
}

class CBloodEffect :
    public Engine::CGameObject
{
private:
	explicit CBloodEffect(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CBloodEffect();
public:
	static CBloodEffect* Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual			HRESULT		Ready_GameObject();
	virtual			_int		Update_GameObject(const _float& fTimeDelta);
	virtual			void		LateUpdate_GameObject(const _float& fTimeDelta);

public:
	void Active_Blood(float duration);
	void SetRand_Transform();

private:
	_bool m_bActive = false;
	_float m_fElapsedTime = 0.f;
	_float m_fDurateTime = 0.f;
	CTransform* m_pTransform = nullptr;
	CUiQuad* m_pQuad = nullptr;
	UIPanel m_tPanel;

private:
	void Free();
};

