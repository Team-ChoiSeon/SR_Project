#pragma once
#include "CGameObject.h"
#include "UIPanel.h"

namespace Engine {
	class CTransform;
	class CUiQuad;
	class CCamera;
}

class CInfoSlot :
    public CGameObject
{
private:
	explicit CInfoSlot(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CInfoSlot();
public:
	static CInfoSlot* Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual			HRESULT		Ready_GameObject();
	virtual			_int		Update_GameObject(const _float& fTimeDelta);
	virtual			void		LateUpdate_GameObject(const _float& fTimeDelta);

public:
	void Set_Pos(_vec2 pos);
	UIPanel& Get_Panel() { return m_tPanel; };
private:
	_bool m_bOpen;
	CTransform* m_pTransform = nullptr;
	CUiQuad* m_pQuad = nullptr;
	UIPanel m_tPanel;
private:
	void Free();
};

