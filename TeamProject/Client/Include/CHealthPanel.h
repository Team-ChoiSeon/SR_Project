#pragma once
#include "CGameObject.h"
#include "UIPanel.h"
class CHealthBar;
class CHealthIcon;
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

private://component Caching
	CTransform* m_pTransform;
	CUiQuad* m_pQuad;

	CMainPlayer* m_pPlayer = nullptr;
	CCamera* m_pMainCam= nullptr;

	UIPanel m_tPanel;
private:
	CHealthBar* m_pHealthBar;
	CHealthIcon* m_pHealthIcon;
private:
	void Free();
};

