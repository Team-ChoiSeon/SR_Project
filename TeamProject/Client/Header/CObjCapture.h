#pragma once
#include "CGameObject.h"
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
private:
	void Free();
};
