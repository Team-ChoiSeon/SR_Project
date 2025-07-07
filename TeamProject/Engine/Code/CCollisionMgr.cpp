#include "CCollisionMgr.h"

IMPLEMENT_SINGLETON(CCollisionMgr)

CCollisionMgr::CCollisionMgr()
{
}

CCollisionMgr::~CCollisionMgr()
{
}

void CCollisionMgr::Update_Collision()
{
	set<ColliderPair, PairLess> setCurrCollisions;
	for (size_t i = 0; i < m_vCol.size(); ++i)
	{
		for (size_t j = i + 1; j < m_vCol.size(); ++j)
		{
			CCollider* pA = m_vCol[i];
			CCollider* pB = m_vCol[j];

			if (Is_Colliding(pA->Get_AABBW(), pB->Get_AABBW()))
			{
				ColliderPair pair = make_pair(pA, pB);
				setCurrCollisions.insert(pair);
				if (m_setPrevCollisions.find(pair) != m_setPrevCollisions.end())
				{
					// 충돌 중 (Stay)
					pA->On_Collision_Stay(pB);
					pB->On_Collision_Stay(pA);
				}
				else
				{
					// 충돌 진입 (Enter)
					pA->On_Collision_Enter(pB);
					pB->On_Collision_Enter(pA);
				}
			}
		}
	}

	// 충돌 종료 (Exit)
	for (const auto& prev : m_setPrevCollisions)
	{
		if (setCurrCollisions.find(prev) == setCurrCollisions.end())
		{
			CCollider* pA = prev.first;
			CCollider* pB = prev.second;
			pA->On_Collision_Exit(pB);
			pB->On_Collision_Exit(pA);
		}
	}

	// 갱신
	m_setPrevCollisions = move(setCurrCollisions);
}

bool CCollisionMgr::Is_Colliding(const AABB& a, const AABB& b)
{
	return !(a.vMax.x < b.vMin.x || a.vMin.x > b.vMax.x ||
		a.vMax.y < b.vMin.y || a.vMin.y > b.vMax.y ||
		a.vMax.z < b.vMin.z || a.vMin.z > b.vMax.z);
}

void CCollisionMgr::Add_Collider(CCollider* collider)
{
	auto iter = find_if(m_vCol.begin(), m_vCol.end(),
		[&collider](CCollider* data)->bool {
			return data == collider;
		});

	if (iter == m_vCol.end())
		m_vCol.push_back(collider);
}

void CCollisionMgr::Remove_Collider(CCollider* collider)
{
	auto iter = remove_if(m_vCol.begin(), m_vCol.end(),
		[&collider](CCollider* data)->bool {
			return data == collider;
		});

	if (iter != m_vCol.end()) {
		m_vCol.erase(iter, m_vCol.end());
	}
}

void CCollisionMgr::Clear()
{
	m_vCol.clear();
}

void CCollisionMgr::Free()
{

}