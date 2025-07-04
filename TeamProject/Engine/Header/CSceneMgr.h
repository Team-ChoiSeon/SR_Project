#pragma once

#include "CBase.h"
#include "CScene.h"

BEGIN(Engine)

class ENGINE_DLL CSceneMgr : public CBase
{
	DECLARE_SINGLETON(CSceneMgr)

private:
	explicit	CSceneMgr();
	virtual		~CSceneMgr();

public:
	HRESULT			Set_Scene(CScene* pScene);
	_int			Update_Scene(const _float& fTimeDelta);
	void			LateUpdate_Scene(const _float& fTimeDelta);

private:
	CScene* m_pScene;

public:
	virtual void			Free();
};

END