#include "CCollisionMgr.h"
#include "CGameObject.h"

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
	Clear();
}



void CCollisionMgr::Add_Collider(CCollider* collider)
{
	// ▼▼▼▼▼▼▼▼▼▼ 이 디버깅 코드를 추가해주세요 ▼▼▼▼▼▼▼▼▼▼
	if (collider)
	{
		CGameObject* pOwner = collider->m_pOwner; // CCollider에 Get_Owner()가 있어야 합니다.
		if (pOwner)
		{
			char szBuffer[256] = "";
			sprintf_s(szBuffer, "Collider Added: Addr=0x%p, OwnerType=%s, OwnerAddr=0x%p\n",
				collider,
				pOwner->Get_Type(), // 1단계에서 추가한 함수 사용
				pOwner);
			OutputDebugStringA(szBuffer); // Visual Studio의 '출력' 창에서 확인
		}
		else
		{
			// 소유자가 없는 콜라이더가 추가되는 경우
			char szBuffer[128] = "";
			sprintf_s(szBuffer, "Collider Added: Addr=0x%p, Owner is NULL!\n", collider);
			OutputDebugStringA(szBuffer);
		}
	}
	// ▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲
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