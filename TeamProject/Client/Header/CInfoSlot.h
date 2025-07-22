#pragma once
#include "C:\Users\Á¤ºÎ±â\Desktop\SR_Project\TeamProject\Reference\Header\CGameObject.h"
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
private:
	void Free();
};

