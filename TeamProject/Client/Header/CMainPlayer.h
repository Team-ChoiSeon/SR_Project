#pragma once
#include "CGameObject.h"
#include "CTransform.h"
#include "CRigidBody.h"
#include "CCrosshairUIObject.h"

namespace Engine {
	class CModel;
	class CCollider;
}

class CCube;
class CSwitch;
class CMainPlayer : public CGameObject
{
public:
	enum class PLAYER_STATE { PLAYER_IDLE, PLAYER_MOVE, PLAYER_JUMP, PLAYER_FALL  };  // 필요시에 더 추가
private:
	CMainPlayer(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CMainPlayer();

public:
	//Basic Function
	HRESULT Ready_GameObject();
	int Update_GameObject(const _float& fTimeDelta) override;
	void LateUpdate_GameObject(const _float& fTimeDelta) override;

	void Change_State(PLAYER_STATE eNewState);

	//Create, Release Function
	static CMainPlayer* Create(LPDIRECT3DDEVICE9 pGraphicDev);
	void Free();

	//Gettter, Setter Function
	_vec3 GetPos() { return Get_Component<CTransform>()->Get_Pos(); }
	bool Get_Hold() { return m_bObjHold;  }
	bool Get_MouseTap() { return m_bMouseTap; }
	bool Get_MouseAway() { return m_bMouseAway; }
	_vec3 Get_DragDistance() { return m_vDragDistance; }
	CGameObject* Get_PickObj() { return m_pPickObj; }
	CGameObject* Get_PrevPickObj() { return m_pPrevPickObj; }

	void Set_Crosshair(CCrosshairUIObject* crosshair) { m_pCrosshair = crosshair; };

protected:
	//Utility Function
	void KeyInput(const _float& fTimeDelta);
	void Update_State(const _float& fTimeDelta);
	void CursorRotate(const _float& fTimeDelta);

private:
	//Compute Function
	void Picking_Init();
	void Tap_Picking();
	void Hold_Picking();
	void Away_Picking();


	CTransform* m_pTransform = nullptr;
	CModel* m_pModel = nullptr;
	CCollider* m_pCollider = nullptr;
	CRigidBody* m_pRigid = nullptr;
	CGameObject* m_pPickObj = nullptr;
	CGameObject* m_pPrevPickObj = nullptr;

	CCrosshairUIObject* m_pCrosshair = nullptr;

	float m_fMoveSpeed;
	float m_fJumpPower = 20.f;
	_float m_fJumpTime = 5.f;

	float m_fWidth;
	float m_fDepth;

	bool m_bCursorMove;
	bool m_bObjHold = false;
	bool m_bMouseTap = false;
	bool m_bMouseAway = false;

	_vec3 m_vLastPt;
	_vec3 m_vPlanePt;
	_vec3 m_vPlaneNorm;
	_vec3 m_vDragDistance;

	_vec3 m_vPickPoint;
	_vec3 m_vPickPointGap;
	_vec3 m_vPickObjPos;
	_vec3 m_vPickPointDist;
	_float m_fPickPointDist;
	_vec3 m_vPickObjDist;
	_float m_fPickObjDist;
	_vec3 m_vPrePickObjPos;
	_vec3 m_vPrePickPoint;
	_vec3 m_vCursorRotate;
	_float m_fMaxPickDist = 20.f;

	CCube* m_PickedCube;
	CSwitch* m_PickedSwitch;
	Ray* m_pRay;

	PLAYER_STATE m_eCurState = PLAYER_STATE::PLAYER_IDLE;
	PLAYER_STATE m_ePrevState = PLAYER_STATE::PLAYER_IDLE;
};