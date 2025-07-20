#pragma once
#include "CGameObject.h"

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

private:
	CGameObject* BackGround;

public: 
	virtual			void Free();
};

