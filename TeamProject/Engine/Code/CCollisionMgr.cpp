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

	for (auto iterA = m_ColList.begin(); iterA != m_ColList.end(); ++iterA)
	{
		auto iterB = iterA;
		++iterB; // iterB는 항상 iterA의 다음부터 시작

		for (; iterB != m_ColList.end(); ++iterB)
		{
			CCollider* pA = *iterA;
			CCollider* pB = *iterB;

			// 오브젝트가 이미 소멸되어 포인터가 nullptr이 된 경우를 대비
			if (pA == nullptr || pB == nullptr)
				continue;

			if (!pA->Broad_Phase(pB))
				continue;

			_vec3 push{};
			bool bCollided = pA->Narrow_Phase(pB, push);

			if (bCollided)
			{
				ColliderPair pair = make_pair(pA, pB);
				setCurrCollisions.insert(pair);

				if (m_setPrevCollisions.find(pair) != m_setPrevCollisions.end())
				{
					pA->On_Collision_Stay(pB);
					pB->On_Collision_Stay(pA);
				}
				else
				{
					pA->On_Collision_Enter(pB);
					pB->On_Collision_Enter(pA);
				}
			}
		}
	}

	// Exit 처리
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

	m_setPrevCollisions = move(setCurrCollisions);
}

void CCollisionMgr::Add_Collider(CCollider* collider)
{
	auto iter = find_if(m_ColList.begin(), m_ColList.end(),
		[&collider](CCollider* data)->bool {
			return data == collider;
		});

	if (iter == m_ColList.end())
		m_ColList.push_back(collider);
}

void CCollisionMgr::Remove_Collider(CCollider* collider)
{
	m_ColList.remove(collider);
}

void CCollisionMgr::Clear()
{
	m_ColList.clear();
}

void CCollisionMgr::Free()
{

}