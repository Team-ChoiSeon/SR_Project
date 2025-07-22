#pragma once
#include "CGameObject.h"
#include "UIPanel.h"

class CMainPlayer;

namespace Engine {
	class CTransform;
	class CUiQuad;
	class CCamera;
}

class CObjectInfo :
    public CGameObject
{
private:
	explicit CObjectInfo(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CObjectInfo();
public:
	static CObjectInfo* Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual			HRESULT		Ready_GameObject();
	virtual			_int		Update_GameObject(const _float& fTimeDelta);
	virtual			void		LateUpdate_GameObject(const _float& fTimeDelta);

private://component Caching
	CTransform* m_pTransform = nullptr;
	CUiQuad* m_pQuad = nullptr;

	CMainPlayer* m_pPlayer = nullptr;
	UIPanel m_tPanel;
private:
	void Free();
};

