#pragma once
#include "CBase.h"

#include "CCollider.h"

BEGIN(Engine)

class ENGINE_DLL CCollisionMgr : public CBase
{
	DECLARE_SINGLETON(CCollisionMgr)

private:
	explicit	  CCollisionMgr();
	virtual		 ~CCollisionMgr();

public:
	void Add_Collider(CCollider* collider);
	void Remove_Collider(CCollider* collider);
	void Update_Collision();
	bool Is_Colliding(const AABB& a, const AABB& b);
	void Clear();

private:
	vector<CCollider*> m_vCol;

private:
	virtual void Free() override;


};

END
