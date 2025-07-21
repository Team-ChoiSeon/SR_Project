#pragma once
#include "CGameObject.h"
class CHealthBar;
class CMainPlayer;

namespace Engine {
	class CTransform;
	class CUiQuad;
	class CCamera;
}

class CHealthPanel :
	public Engine::CGameObject
{
private:
	explicit CHealthPanel(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CHealthPanel();
public:
	static CHealthPanel* Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual			HRESULT		Ready_GameObject();
	virtual			_int		Update_GameObject(const _float& fTimeDelta);
	virtual			void		LateUpdate_GameObject(const _float& fTimeDelta);

private:
	void Compute_ScreenToWorld();

private://component Caching
	CTransform* m_pTransform;
	CUiQuad* m_pQuad;
	CMainPlayer* m_pPlayer = nullptr;
	CCamera* m_pMainCam= nullptr;

	_vec3 m_vScreen;
	_vec3 m_vWorldPos;
	_vec2 m_vTest;
private:
	CHealthBar* m_pHealthBar;
private:
	void Free();
};

