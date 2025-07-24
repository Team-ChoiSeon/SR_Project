#pragma once
#include "CGameObject.h"
#include "UIPanel.h"

namespace Engine {
	class CTransform;
	class CUiQuad;
	class CCamera;
}

class CObjCapture :
    public CGameObject
{
private:
	explicit CObjCapture(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CObjCapture();
public:
	static CObjCapture* Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual			HRESULT		Ready_GameObject();
	virtual			_int		Update_GameObject(const _float& fTimeDelta);
	virtual			void		LateUpdate_GameObject(const _float& fTimeDelta);
	void RenderTarget();
public:
	void Set_Pos(_vec2 pos);
	void Set_Open(_bool open) { m_bOpen = open; };
	void Set_Object(CGameObject* obj) {  m_pRenderTarget= obj ; };
	UIPanel& Get_Panel() { return m_tPanel; };
	void RenderModel(CModel* model);

private:
	_bool m_bOpen = false;
	CTransform* m_pTransform = nullptr;
	CUiQuad* m_pQuad = nullptr;
	UIPanel m_tPanel;
	CGameObject* m_pRenderTarget = nullptr;
	LPDIRECT3DTEXTURE9 m_pTargetTex = nullptr;
	_vec2 vRatio;
private:
	void Free();
};
