#pragma once
#include "CGameObject.h"
#include "UIPanel.h"

namespace Engine {
	class CTransform;
	class CUiQuad;
	class CCamera;
}

class CHints : public CGameObject
{
private:
	explicit CHints(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CHints();
public:
	static CHints* Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual			HRESULT		Ready_GameObject();
	virtual			_int		Update_GameObject(const _float& fTimeDelta);
	virtual			void		LateUpdate_GameObject(const _float& fTimeDelta);

public:
	void Set_Active(const wstring& Hint);
	_bool Get_Active() { return m_bActive; }
private:
	CTransform* m_pTransform = nullptr;
	CUiQuad* m_pQuad = nullptr;
	wstring m_sHint;
	UIPanel m_tPanel;

	_bool m_bActive = false;
	_float m_fLifeTime;
	_float m_fAge;
	_float m_fSpeed;

	ID3DXFont* m_pFont = nullptr;
	LPDIRECT3DTEXTURE9 m_pHintTex = nullptr;
	LPDIRECT3DSURFACE9 m_pNewSurf = nullptr;
	LPDIRECT3DSURFACE9 m_pOldSurf = nullptr;
private:
	void Free();
};

