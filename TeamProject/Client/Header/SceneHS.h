#pragma once
#include "CScene.h"
class SceneHS :
    public CScene
{
private:
	explicit SceneHS(LPDIRECT3DDEVICE9 pGraphicDev);
public:
	virtual ~SceneHS();

public:
	static SceneHS* Create(LPDIRECT3DDEVICE9 pGraphicDev);

public:
	virtual			HRESULT		Ready_Scene()override;
	virtual			_int		Update_Scene(const _float& fTimeDelta)override;
	virtual			void		LateUpdate_Scene(const _float& fTimeDelta)override;
	virtual			void		Render_Scene()override;

public:
	virtual void Free();
};

