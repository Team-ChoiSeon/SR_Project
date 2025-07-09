#pragma once
#include "CGameObject.h"
#include "CTransform.h"
#include "CRigidBody.h"

namespace Engine {
	class CModel;
	class CCollider;
}
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
	_vec3 Get_DragDistance() { return m_vDragDistance; }
	CGameObject* Get_PickObj() { return m_pPickObj; }

	void Set_GroundCheck();

protected:
	//Utility Function
	void KeyInput(const _float& fTimeDelta);
	void Update_State(const _float& fTimeDelta);
	void CursorRotate();

private:

	CTransform* m_pTransform;
	CModel* m_pModel;
	CCollider* m_pCollider;
	CRigidBody* m_pRigid;
	CGameObject* m_pPickObj;

	float m_fMoveSpeed;
	float m_fJumpPower = 20.f;

	float m_fWidth;
	float m_fDepth;

	bool m_bCursorMove;
	bool m_bObjHold = false;

	_vec3 m_vLastPt;
	_vec3 m_vPlanePt;
	_vec3 m_vPlaneNorm;
	_vec3 m_vDragDistance;

	PLAYER_STATE m_eCurState = PLAYER_STATE::PLAYER_IDLE;
	PLAYER_STATE m_ePrevState = PLAYER_STATE::PLAYER_IDLE;
};