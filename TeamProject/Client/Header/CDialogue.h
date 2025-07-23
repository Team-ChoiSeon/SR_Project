#pragma once
#include "CGameObject.h"
#include "UIPanel.h"

namespace Engine {
	class CTransform;
	class CUiQuad;
	class CCamera;
}

class CDialogue :
    public Engine::CGameObject
{
private:
	explicit CDialogue(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CDialogue();
public:
	static CDialogue* Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual			HRESULT		Ready_GameObject();
	virtual			_int		Update_GameObject(const _float& fTimeDelta);
	virtual			void		LateUpdate_GameObject(const _float& fTimeDelta);
public:
	void Set_Pos(_vec2 pos);
	void Set_Open(_bool open) { m_bOpen = open; };
	UIPanel& Get_Panel() { return m_tPanel; };

private:
	_bool m_bOpen;
	ID3DXFont* m_pFont = nullptr;
	CTransform* m_pTransform = nullptr;
	CUiQuad* m_pQuad = nullptr;
	LPDIRECT3DTEXTURE9 m_pDialTex = nullptr;
	wstring m_sDialogue;
	UIPanel m_tPanel;

public: 
	virtual			void Free();
};

