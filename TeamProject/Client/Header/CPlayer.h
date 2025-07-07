#pragma once
#include "CGameObject.h"

#include "CTransform.h"
#include "CModel.h"
#include "CCollider.h"
#include "CRigidbody.h"


class CPlayer : public Engine::CGameObject
{
private:
	CPlayer(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CPlayer();

public:
	//Basic Function
	HRESULT Ready_GameObject();
	int Update_GameObject(const _float& fTimeDelta) override;
	void LateUpdate_GameObject(const _float& fTimeDelta) override;

	//Create, Release Function
	static CPlayer* Create(LPDIRECT3DDEVICE9 pGraphicDev);
	void Free();

	//Gettter, Setter Function
	_vec3 GetPos() { return Get_Component<CTransform>()->Get_Pos(); }


protected:
	//Utility Function
	void KeyInput(const _float& fTimeDelta);
	void CursorRotate();

private:
	CTransform* m_pTransform;
	CModel* m_pModel;
	CCollider* m_pCollider;
	CRigidbody* m_pRigid;

	const _matrix* m_mWorld;

	float m_fSpeed;

	float m_fWidth;
	float m_fDepth;

	bool m_bCursorMove;
};