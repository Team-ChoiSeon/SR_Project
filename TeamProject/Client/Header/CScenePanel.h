#pragma once
#include "CGameObject.h"
#include "CImageUIObject.h"

class CScenePanel :
    public Engine::CGameObject
{
private:
    explicit CScenePanel(LPDIRECT3DDEVICE9 pGraphicDev);
    virtual ~CScenePanel();
public:
	static CScenePanel* Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual			HRESULT		Ready_GameObject();
	virtual			_int		Update_GameObject(const _float& fTimeDelta);
	virtual			void		LateUpdate_GameObject(const _float& fTimeDelta);
	virtual			void		Render_GameObject() {};

public: 
	void Set_ObjectInfo(_bool boolean) { m_bInfo = boolean; }
private:
	_bool m_bInfo = false;
	unordered_map<wstring,CGameObject*> m_umUI;
public:
	virtual void Free();
};

