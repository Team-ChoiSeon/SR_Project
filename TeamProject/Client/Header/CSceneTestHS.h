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
	virtual const wstring Get_ScenePath() { return L"../../Scene/Ending.json"; };


private:
	_float fElapsedTime = 0.f;
	void StartEnding(const _float& fTimeDelta);

	//bool bLaunched = false;
	float fElapsed = 0.f;
	bool bReadyToLaunch = false;

	bool bLaunched = false;
	bool bShakeStarted = false;
	_float fShakeTime = 0.f;
	const _float fShakeDuration = 3.f;
	const _float fShakeMagnitude = 0.15f;
};

