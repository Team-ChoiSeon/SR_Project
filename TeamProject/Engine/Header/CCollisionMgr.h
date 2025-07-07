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

	using ColliderPair = pair<CCollider*, CCollider*>;
	struct PairLess {
		bool operator()(const ColliderPair& a, const ColliderPair& b) const
		{
			// (1,3) == (3,1) : µø¿œ«— Ω÷
			return tie(min(a.first, a.second), max(a.first, a.second)) <
				tie(min(b.first, b.second), max(b.first, b.second));
		}
	};
	set<ColliderPair, PairLess> m_setPrevCollisions;

private:
	virtual void Free() override;


};

END
