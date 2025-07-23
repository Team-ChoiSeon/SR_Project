#pragma once
#include "CGameObject.h"
#include "UIPanel.h"

class CMainPlayer;
class CInfoSlot;
class CObjCapture;
class CDialogue;


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

private:
	void OpenPanel(const _float& fTimeDelta);
private:
	_bool m_bOpen;
	CTransform* m_pTransform = nullptr;
	CUiQuad* m_pQuad = nullptr;

	CMainPlayer* m_pPlayer = nullptr;
	UIPanel m_tPanel;

private:
	//childeren ui
	CInfoSlot* m_pSlot = nullptr;
	CObjCapture* m_pCapture = nullptr;
	CDialogue* m_pDialogue = nullptr;
	CGameObject* m_pPicked = nullptr;

private:
	void Free();
};

