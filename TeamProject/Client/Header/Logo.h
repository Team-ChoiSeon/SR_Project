#pragma once
#include "CScene.h"
namespace  Engine{
	class CGameObject;
}
class CMainPlayer;

class Logo : public CScene
{
private:
	explicit Logo(LPDIRECT3DDEVICE9 pGraphicDev);
public:
	virtual ~Logo();

public:
	static Logo* Create(LPDIRECT3DDEVICE9 pGraphicDev);

public:
	virtual			HRESULT		Ready_Scene() override;
	virtual			_int		Update_Scene(const _float& fTimeDelta)override;
	virtual			void		LateUpdate_Scene(const _float& fTimeDelta)override;
	virtual const wstring Get_ScenePath() { return L"../../Scene/Merge_Scene.json"; };

private:
	CGameObject* m_pBackground;
	CMainPlayer* m_pPlayer;
public:
	virtual void Free();
};

