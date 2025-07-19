#pragma once

#include "CBase.h"
#include "CScene.h"

BEGIN(Engine)

class CGameObject;

class ENGINE_DLL CSceneMgr : public CBase
{
	DECLARE_SINGLETON(CSceneMgr)

private:
	explicit	CSceneMgr();
	virtual		~CSceneMgr();

public:
	HRESULT Ready_SceneManager(CScene* pScene);

	_int			Update_Scene(const _float& fTimeDelta);
	void			LateUpdate_Scene(const _float& fTimeDelta);

public:
	HRESULT			Set_Scene(CScene* pScene);
	void			Set_LoadingScene(CScene* pScene);
	HRESULT			Set_Player(CGameObject* pPlayer);

	CScene* Get_Scene() { return m_pCurScene; }
	CGameObject* Get_Player() { return m_pPlayer; }
	void			Set_CurrentScene(CScene* pScene);

private:
	CScene* m_pCurScene = nullptr;
	CScene* m_pLoading = nullptr;
	CGameObject* m_pPlayer = nullptr;

public:
	virtual void			Free();
};

END