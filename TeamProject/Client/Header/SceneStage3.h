#pragma once
#include "CScene.h"
class SceneStage3 :
	public CScene
{
private:
	explicit SceneStage3(LPDIRECT3DDEVICE9 pGraphicDev);
public:
	virtual ~SceneStage3();

public:
	static SceneStage3* Create(LPDIRECT3DDEVICE9 pGraphicDev);

public:
	virtual			HRESULT		Ready_Scene()override;
	virtual			_int		Update_Scene(const _float& fTimeDelta)override;
	virtual			void		LateUpdate_Scene(const _float& fTimeDelta)override;

private:
	_bool m_bClear;
public:
	virtual void Free();
	virtual const wstring Get_ScenePath() { return L"../../Scene/Scene3.json"; };

private:
	void FloatingSet();
	void DirectionSet();
	void SlotSet();
	void StairSet();

	void Set_Triggers();

};

