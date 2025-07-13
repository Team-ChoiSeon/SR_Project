#pragma once
#include "CScene.h"

class CMainPlayer;

class SceneBG :
    public CScene
{
private:
	explicit SceneBG(LPDIRECT3DDEVICE9 pGraphicDev);
public:
	virtual ~SceneBG();

public:
	static SceneBG* Create(LPDIRECT3DDEVICE9 pGraphicDev);

public:
	virtual			HRESULT		Ready_Scene()override;
	virtual			_int		Update_Scene(const _float& fTimeDelta)override;
	virtual			void		LateUpdate_Scene(const _float& fTimeDelta)override;
	void Test_Panel();

private:
	void TileLayer_Set();
	void Step_StoneSet();
	void Moving_StoneSet();
	void Stair_Set();
private:
	CMainPlayer* m_pPlayer;
public:
	virtual void Free();
};

