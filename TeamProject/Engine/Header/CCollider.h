#pragma once
#include "CComponent.h"

BEGIN(Engine)

enum class ColliderType {COL_PASSIVE, COL_ACTIVE, COL_TRIGGER};

class ENGINE_DLL CCollider : public CComponent
{
protected:
	explicit CCollider(LPDIRECT3DDEVICE9 pGraphicDev);

public:
	virtual  ~CCollider();

public:
	static CCollider* Create(LPDIRECT3DDEVICE9 pGraphicDev);
	void Set_ColType(ColliderType eType) { m_eType = eType; }
	ColliderType Get_ColType() { return m_eType; }
	const AABB& Get_AABBW() { return m_tAABBWorld; }

public:
	virtual void Update_Component(const _float& fTimeDelta) override;
	virtual void LateUpdate_Component() override;
	void Render(LPDIRECT3DDEVICE9 pDevice);
	// pCollider : other
	// ex) change color, set flag
	void On_Collision_Enter(CCollider* pCollider);
	void On_Collision_Stay(CCollider* pCollider);
	void On_Collision_Exit(CCollider* pCollider);
	virtual void Free();

private:
	AABB m_tAABB = { {-1,-1,-1},{1,1,1} };
	AABB m_tAABBWorld;

	LPDIRECT3DVERTEXBUFFER9 m_pVB = nullptr;
	LPDIRECT3DINDEXBUFFER9  m_pIB = nullptr;

	ColliderType m_eType = ColliderType::COL_ACTIVE;
};

END

