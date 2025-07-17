#pragma once
#include "CGameObject.h"

#include "CModel.h"


BEGIN(Engine)

class CTransform;
class CRigidBody;
class CCollider;

enum class PartState { NORMAL, BROKEN };

class ENGINE_DLL CMonsterPart : public CGameObject
{
private:
	explicit CMonsterPart(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CMonsterPart(const CMonsterPart& rhs);
	virtual ~CMonsterPart();

public:
	static CMonsterPart* Create(LPDIRECT3DDEVICE9 pGraphicDev);

	virtual	 HRESULT	Ready_GameObject()override;
	virtual	 _int		Update_GameObject(const _float& fTimeDelta)override;
	virtual	 void		LateUpdate_GameObject(const _float& fTimeDelta)override;

public:
	void Follow_Target(_float fDeltaTime);
	void Set_Target(CGameObject* pTarget) { m_pTarget = pTarget; }
	void Set_Index(int idx, int max) { m_iIdx = idx, m_iMax = max; }
	void Set_State(PartState eState) { m_eState = eState; }

	CGameObject* Get_Owner();
	CGameObject* Get_Target() { return m_pTarget; }
	PartState Get_State() const { return m_eState; }

	void Reserve_Delete() { m_bDelete = true; }

public:
	virtual void Free();

private:
	CModel* m_pModel = nullptr;
	CTransform* m_pTransform = nullptr;
	CRigidBody* m_pRigid = nullptr;
	CCollider* m_pCol = nullptr;

	CGameObject* m_pTarget = nullptr;
	_float m_fSpeed = 10.f;

	bool m_bDelete = false;
	PartState m_eState = PartState::NORMAL;

	int m_iIdx = 0;
	int m_iMax = 0;

};

END