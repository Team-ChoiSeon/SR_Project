#include "CCollider.h"
#include "CRenderMgr.h"
#include "CTransform.h"
#include "CCollisionMgr.h"
#include "CRigidBody.h"
#include "CGameObject.h"


class CGameObject;

CCollider::CCollider(LPDIRECT3DDEVICE9 pGraphicDev)
	: CComponent(pGraphicDev)
{
}

CCollider::~CCollider()
{
}

CCollider* CCollider::Create(LPDIRECT3DDEVICE9 pGraphicDev, CRigidBody* pRigid)
{
	CCollider* pCollider = new CCollider(pGraphicDev);
	pCollider->m_pRigid = pRigid;
	if (pCollider == nullptr || pRigid == nullptr)
	{
		MSG_BOX("CCollider Create Failed");
		return nullptr;
	}
	return pCollider;
}
void CCollider::Update_Component(const _float& fTimeDelta)
{
	switch (m_eState)
	{
	case ColliderState::ENTER:
		m_eState = ColliderState::STAY;
		break;
	case ColliderState::EXIT:
		m_eState = ColliderState::NONE;
		break;
	}

	CTransform* pTransform = m_pOwner->Get_Component<CTransform>();
	if (!pTransform) return;

	
	D3DXVec3TransformCoord(&m_tAABBWorld.vMin, &m_tAABB.vMin, pTransform->Get_WorldMatrix());
	D3DXVec3TransformCoord(&m_tAABBWorld.vMax, &m_tAABB.vMax, pTransform->Get_WorldMatrix());
	CCollisionMgr::Get_Instance()->Add_Collider(this);
}

void CCollider::LateUpdate_Component()
{
	CRenderMgr::Get_Instance()->Add_Collider(this);
}

void CCollider::Render(LPDIRECT3DDEVICE9 pDevice)
{
	CTransform* pTransform = m_pOwner->Get_Component<CTransform>();
	if (pTransform == nullptr)
	{
		MSG_BOX("CModel::Render : pTransform is nullptr");
		return;
	}

	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VTXLINE) * 8, 0, FVF_LINE,
		D3DPOOL_MANAGED, &m_pVB, nullptr)))
		return;

	// 인덱스 버퍼 (12개 선 = 24 index)
	if (FAILED(pDevice->CreateIndexBuffer(sizeof(WORD) * 24, 0, D3DFMT_INDEX16,
		D3DPOOL_MANAGED, &m_pIB, nullptr)))
		return;

	// 인덱스 채우기
	WORD* pIndices = nullptr;
	m_pIB->Lock(0, 0, (void**)&pIndices, 0);

	WORD indices[24] = {
		0,1, 1,2, 2,3, 3,0, // 앞면
		4,5, 5,6, 6,7, 7,4, // 뒷면
		0,4, 1,5, 2,6, 3,7  // 측면
	};
	memcpy(pIndices, indices, sizeof(indices));
	m_pIB->Unlock();

	if (!m_pVB || !m_pIB) return;

	VTXLINE* pVertices = nullptr;
	m_pVB->Lock(0, 0, (void**)&pVertices, 0);

	D3DCOLOR color = D3DCOLOR_ARGB(255, 0, 255, 0); // 연두색

	const _vec3& vMin = m_tAABB.vMin;
	const _vec3& vMax = m_tAABB.vMax;

	pVertices[0] = { {vMin.x, vMin.y, vMin.z}, color };
	pVertices[1] = { {vMax.x, vMin.y, vMin.z}, color };
	pVertices[2] = { {vMax.x, vMax.y, vMin.z}, color };
	pVertices[3] = { {vMin.x, vMax.y, vMin.z}, color };
	pVertices[4] = { {vMin.x, vMin.y, vMax.z}, color };
	pVertices[5] = { {vMax.x, vMin.y, vMax.z}, color };
	pVertices[6] = { {vMax.x, vMax.y, vMax.z}, color };
	pVertices[7] = { {vMin.x, vMax.y, vMax.z}, color };

	m_pVB->Unlock();

	pDevice->SetFVF(FVF_LINE);
	pDevice->SetStreamSource(0, m_pVB, 0, sizeof(VTXLINE));
	pDevice->SetIndices(m_pIB);

	pDevice->DrawIndexedPrimitive(D3DPT_LINELIST, 0, 0, 8, 0, 12);
}

void CCollider::On_Collision_Enter(CCollider* pOther)
{
	ColliderType oType = pOther->Get_ColType();
	if (m_eType == ColliderType::TRIGGER || oType == ColliderType::TRIGGER)
		return; // 트리거는 반응 없음

	if (m_eType == ColliderType::ACTIVE && oType == ColliderType::PASSIVE)
	{
		// 둘 다 밀림
		const AABB& myAABB = Get_AABBW();
		const AABB& otherAABB = pOther->Get_AABBW();
		// 가장 작은 축 방향으로 밀기 벡터 계산
		_vec3 push(0.f, 0.f, 0.f);
		if (!Calc_Push(myAABB, otherAABB, push))	return;

		// Transform 얻기
		CTransform* pTransform = m_pOwner->Get_Component<CTransform>();
		if (pTransform)
		{
			_vec3 vPos = pTransform->Get_Pos();
			pTransform->Set_Pos(vPos + push);
		}

		// 지면 충돌 : Ground와 위에서 닿은 경우만
		if (m_eType == ColliderType::ACTIVE &&
			pOther->Get_ColType() == ColliderType::PASSIVE &&
			pOther->Get_ColTag() == ColliderTag::GROUND &&
			push.y > 0.f)
		{
			if (m_pRigid)
			{
				m_pRigid->Set_OnGround(true);
			}
		}


	}
	else if (m_eType == ColliderType::PASSIVE && oType == ColliderType::ACTIVE)
	{
		// 상대가 밀려나는 쪽
	}
	else if (m_eType == ColliderType::ACTIVE && oType == ColliderType::ACTIVE)
	{
		// 둘 다 밀림
		const AABB& myAABB = Get_AABBW();
		const AABB& otherAABB = pOther->Get_AABBW();
		// 가장 작은 축 방향으로 밀기 벡터 계산
		_vec3 push(0.f, 0.f, 0.f);
		if (!Calc_Push(myAABB, otherAABB, push))	return;
		// Rigidbody 얻기

		if (!m_pRigid || !pOther->m_pRigid)
			return;

		// 질량 비율 계산
		float m1 = m_pRigid->Get_Mass();
		float m2 = pOther->m_pRigid->Get_Mass();
		float total = m1 + m2;

		float ratio1 = m2 / total;
		float ratio2 = m1 / total;

		// 외력으로 반작용 적용
		_vec3 force1 = push * ratio1 * 1000.f;
		_vec3 force2 = -push * ratio2 * 1000.f;

		CTransform* pTransform = m_pOwner->Get_Component<CTransform>();
		if (pTransform)
		{
			_vec3 vPos = pTransform->Get_Pos();
			pTransform->Set_Pos(vPos + push);
		}

		m_pRigid->Add_Force(force1);
		pOther->m_pRigid->Add_Force(force2);

	}

	if (m_eState == ColliderState::NONE || m_eState == ColliderState::EXIT)
		m_eState = ColliderState::ENTER;
}

void CCollider::On_Collision_Stay(CCollider* pOther)
{
	ColliderType oType = pOther->Get_ColType();
	if (m_eType == ColliderType::ACTIVE && oType == ColliderType::PASSIVE)
	{
	
		// 둘 다 밀림
		const AABB& myAABB = Get_AABBW();
		const AABB& otherAABB = pOther->Get_AABBW();
		// 가장 작은 축 방향으로 밀기 벡터 계산
		_vec3 push(0.f, 0.f, 0.f);
		if (!Calc_Push(myAABB, otherAABB, push))	return;

		// Transform 얻기
		CTransform* pTransform = m_pOwner->Get_Component<CTransform>();
		if (pTransform)
		{
			_vec3 vPos = pTransform->Get_Pos();
			pTransform->Set_Pos(vPos + push);
		}
	}

	else if (m_eType == ColliderType::ACTIVE && oType == ColliderType::ACTIVE)
	{
		// 둘 다 밀림
		const AABB& myAABB = Get_AABBW();
		const AABB& otherAABB = pOther->Get_AABBW();
		// 가장 작은 축 방향으로 밀기 벡터 계산
		_vec3 push(0.f, 0.f, 0.f);
		if (!Calc_Push(myAABB, otherAABB, push))	return;

		if (!m_pRigid || !pOther->m_pRigid)
			return;

		// 질량 비율 계산
		float m1 = m_pRigid->Get_Mass();
		float m2 = pOther->m_pRigid->Get_Mass();
		float total = m1 + m2;

		float ratio1 = m2 / total;
		float ratio2 = m1 / total;

		// 외력으로 반작용 적용
		_vec3 force1 = push * ratio1 * 1000.f;
		_vec3 force2 = -push * ratio2 * 1000.f;

		CTransform* pTransform = m_pOwner->Get_Component<CTransform>();
		if (pTransform)
		{
			_vec3 vPos = pTransform->Get_Pos();
			pTransform->Set_Pos(vPos + push);
		}

		m_pRigid->Add_Force(force1);
		pOther->m_pRigid->Add_Force(force2);
	}

	if (m_eState == ColliderState::NONE)
		m_eState = ColliderState::ENTER;
	else
		m_eState = ColliderState::STAY;
}

void CCollider::On_Collision_Exit(CCollider* pOther)
{
	if (m_eType == ColliderType::ACTIVE && pOther->Get_ColTag() == ColliderTag::GROUND)
	{
		if (m_pRigid)
			m_pRigid->Set_OnGround(false);
	}

	m_eState = ColliderState::EXIT;
}

bool CCollider::Calc_Push(const AABB& a, const AABB& b, _vec3& push)
{
	_vec3 vMyCenter = (a.vMin + a.vMax) * 0.5f;
	_vec3 vOtherCenter = (b.vMin + b.vMax) * 0.5f;

	_vec3 overlap;
	overlap.x = min(a.vMax.x, b.vMax.x) - max(a.vMin.x, b.vMin.x);
	overlap.y = min(a.vMax.y, b.vMax.y) - max(a.vMin.y, b.vMin.y);
	overlap.z = min(a.vMax.z, b.vMax.z) - max(a.vMin.z, b.vMin.z);


	if (overlap.x <= overlap.y && overlap.x <= overlap.z)
		push.x = (vMyCenter.x > vOtherCenter.x) ? overlap.x : -overlap.x;
	else if (overlap.y <= overlap.z)
		push.y = (vMyCenter.y > vOtherCenter.y) ? overlap.y : -overlap.y;
	else
		push.z = (vMyCenter.z > vOtherCenter.z) ? overlap.z : -overlap.z;

	return true;
}

void CCollider::Calc_Transform(const _matrix& matWorld)
{
}

void CCollider::Free()
{
	Safe_Release(m_pVB);
	Safe_Release(m_pIB);
	Safe_Release(m_pGraphicDev);
	//Safe_Release(m_pOwner);
	m_pRigid = nullptr;
}
