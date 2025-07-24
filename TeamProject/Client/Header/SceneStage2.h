#pragma once
#include "CScene.h"
class SceneStage2 :
	public CScene
{
private:
	explicit SceneStage2(LPDIRECT3DDEVICE9 pGraphicDev);
public:
	virtual ~SceneStage2();

public:
	static SceneStage2* Create(LPDIRECT3DDEVICE9 pGraphicDev);

public:
	virtual			HRESULT		Ready_Scene()override;
	virtual			_int		Update_Scene(const _float& fTimeDelta)override;
	virtual			void		LateUpdate_Scene(const _float& fTimeDelta)override;

private:
	_bool m_bClear;
public:
	virtual void Free();
	virtual const wstring Get_ScenePath() { return L"../../Scene/Scene2.json"; };

private:
	void FloatingSet();
	void DirectionSet();
	void SlotSet();
	void StairSet();
	
	void Set_Triggers();

	vector<int> AnswerSwitch3_1 = { 1, 2, 3, 4 };
	vector<int> InputSwitch3_1;

	float m_fMuteTimer = 0.f;
	bool  m_bSfxMuted = false;


	vector<bool> m_vbPanels;
	_bool m_bSceneFirst = true;

};

