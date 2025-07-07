#pragma once
#include "CComponent.h"

BEGIN(Engine)

enum class ColliderType {PASSIVE, ACTIVE, TRIGGER};
enum class ColliderTag { NONE, GROUND, WALL, PLATFORM };

class ENGINE_DLL CCollider : public CComponent
{
protected:
	explicit CCollider(LPDIRECT3DDEVICE9 pGraphicDev);

public:
	virtual  ~CCollider();

public:
	static CCollider* Create(LPDIRECT3DDEVICE9 pGraphicDev);

	void Set_ColType(ColliderType eType) { m_eType = eType; }
	void Set_ColTag(ColliderTag eTag)    { m_eTag = eTag; }

	ColliderType Get_ColType() { return m_eType; }
	ColliderTag  Get_ColTag()  { return m_eTag; }
	const AABB&  Get_AABBW()   { return m_tAABBWorld; }

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

	ColliderType m_eType = ColliderType::ACTIVE;
	ColliderTag m_eTag	 = ColliderTag::NONE;
};

END

