#pragma once
#include "CScene.h"

class BossScene :
	public CScene
{
private:
	explicit BossScene(LPDIRECT3DDEVICE9 pGraphicDev);
public:
	virtual ~BossScene();

public:
	static BossScene* Create(LPDIRECT3DDEVICE9 pGraphicDev);

public:
	virtual			HRESULT		Ready_Scene()override;
	virtual			_int		Update_Scene(const _float& fTimeDelta)override;
	virtual			void		LateUpdate_Scene(const _float& fTimeDelta)override;

	virtual const wstring Get_ScenePath() { return L"../../Scene/SceneSB.json"; };

public:
	virtual void Free();
};