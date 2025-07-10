#pragma once
#include "CScene.h"
class CSceneTestHS :
	public CScene
{
private:
	explicit CSceneTestHS(LPDIRECT3DDEVICE9 pGraphicDev);
public:
	virtual ~CSceneTestHS();

public:
	static CSceneTestHS* Create(LPDIRECT3DDEVICE9 pGraphicDev);

public:
	virtual			HRESULT		Ready_Scene()override;
	virtual			_int		Update_Scene(const _float& fTimeDelta)override;
	virtual			void		LateUpdate_Scene(const _float& fTimeDelta)override;

public:
	virtual void Free();
};

