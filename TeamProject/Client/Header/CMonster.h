#pragma once
#include "CGameObject.h"

#include "CTransform.h"
#include "CModel.h"
#include "CCollider.h"
#include "CRigidBody.h"


class CMonster : public Engine::CGameObject
{
private:
	CMonster(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CMonster();

public:
	//Basic Function
	HRESULT Ready_GameObject()override;
	int Update_GameObject(const _float& fTimeDelta) override;
	void LateUpdate_GameObject(const _float& fTimeDelta) override;

	//Create, Release Function
	static CMonster* Create(LPDIRECT3DDEVICE9 pGraphicDev);
	void Free();

	//Gettter, Setter Function
	_vec3 GetPos() { return Get_Component<CTransform>()->Get_Pos(); }


protected:
	//Utility Function
	void KeyInput(const _float& fTimeDelta);

private:
	CTransform* m_pTransform;
	CModel* m_pModel;
	CCollider* m_pCollider;
	CRigidBody* m_pRigid;


	const _matrix* m_mWorld;


	float m_fWidth;
	float m_fDepth;

	bool m_bCursorMove;
};