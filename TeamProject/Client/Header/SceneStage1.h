#pragma once
#include "CScene.h"

class CMainPlayer;
class CFirstviewFollowingCamera;

class SceneStage1 : public CScene
{
private:
	explicit SceneStage1(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~SceneStage1();

	HRESULT Ready_Scene();

public:
	//Basic Function
	int		Update_Scene(const _float& fTimeDelta);
	void	LateUpdate_Scene(const _float& fTimeDelta);

	//Create, Release Function
	static SceneStage1* Create(LPDIRECT3DDEVICE9 pGraphicDev);
	void	Free();

	virtual const wstring Get_ScenePath() { return L"../../Scene/Stage_1.json"; }
private:
	//Setting
	void FloatingSet();
	void DirectionSet();
	void SlotSet();
	void MagnetSet();
	void SensorSet();
	void SoundSet();

	//Update
	void SoundUpdate(const _float& fTimeDelta);
	void Room1Update(const _float& fTimeDelta);
	void Path1Update(const _float& fTimeDelta);
	void Room2Update(const _float& fTimeDelta);
	void Room3Update(const _float& fTimeDelta);
	void Path2Update(const _float& fTimeDelta);
	void Room4Update(const _float& fTimeDelta);
	void Room5Update(const _float& fTimeDelta);

	CMainPlayer* m_pPlayer;
	CFirstviewFollowingCamera* m_pFFCam;

	float m_fMuteTimer = 2.0f;  // 2√ ∞£ mute
	bool  m_bSfxMuted = false;

};

