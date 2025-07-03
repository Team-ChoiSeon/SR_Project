#pragma once
#include "CComponent.h"

BEGIN(Engine)

class ENGINE_DLL CCollider : public CComponent
{
protected:
	explicit CCollider(AABB aabb);

public:
	virtual  ~CCollider();

public:
	static CCollider* Create(AABB aabb);
	const AABB& Get_AABB() { return m_tAABB; }

public:
	virtual void Update_Component(const _float& fTimeDelta) override;
	virtual void LateUpdate_Component() override;
	void Render(LPDIRECT3DDEVICE9 pDevice);
	// ex) change color, set flag
	void On_Collision(CCollider* pCollider) {};
	virtual void Free();

private:
	AABB m_tAABB;

	LPDIRECT3DVERTEXBUFFER9 m_pVB = nullptr;
	LPDIRECT3DINDEXBUFFER9  m_pIB = nullptr;
};

END

