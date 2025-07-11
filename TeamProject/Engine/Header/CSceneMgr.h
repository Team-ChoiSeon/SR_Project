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
	HRESULT			Set_Scene(CScene* pScene);
	HRESULT			Set_Player(CGameObject* pPlayer);

	CScene*			Get_Scene() { return m_pScene; }
	CGameObject*	Get_Player() { return m_pPlayer; }

	_int			Update_Scene(const _float& fTimeDelta);
	void			LateUpdate_Scene(const _float& fTimeDelta);

private:
	CScene* m_pScene = nullptr;
	CGameObject* m_pPlayer = nullptr;

public:
	virtual void			Free();
};

END