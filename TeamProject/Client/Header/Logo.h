#pragma once
#include "CScene.h"
class Logo : public CScene
{
private:
	explicit Logo(LPDIRECT3DDEVICE9 pGraphicDev);
public:
	virtual ~Logo();

public:
	static Logo* Create(LPDIRECT3DDEVICE9 pGraphicDev);

public:
	virtual			HRESULT		Ready_Scene()override;
	virtual			_int		Update_Scene(const _float& fTimeDelta)override;
	virtual			void		LateUpdate_Scene(const _float& fTimeDelta)override;

public:
	virtual void Free();
};

