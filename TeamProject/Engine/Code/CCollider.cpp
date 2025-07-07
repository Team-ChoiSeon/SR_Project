#include "CCollider.h"
#include "CRenderMgr.h"
#include "CTransform.h"
#include "CCollisionMgr.h"
#include "CRigidbody.h"
#include "CGameObject.h"


class CGameObject;

CCollider::CCollider(LPDIRECT3DDEVICE9 pGraphicDev)
	: CComponent(pGraphicDev)
{
}

CCollider::~CCollider()
{
}

CCollider* CCollider::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CCollider* pCollider = new CCollider(pGraphicDev);
	
	if (pCollider == nullptr)
	{
		MSG_BOX("CCollider Create Failed");
		return nullptr;
	}
	return pCollider;
}
void CCollider::Update_Component(const _float& fTimeDelta)
{
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
		//MSG_BOX("Collision Detected!");
		const AABB& myAABB = Get_AABBW();
		const AABB& otherAABB = pOther->Get_AABBW();

		_vec3 vMyCenter = (myAABB.vMin + myAABB.vMax) * 0.5f;
		_vec3 vOtherCenter = (otherAABB.vMin + otherAABB.vMax) * 0.5f;

		_vec3 overlap;
		overlap.x = min(myAABB.vMax.x, otherAABB.vMax.x) - max(myAABB.vMin.x, otherAABB.vMin.x);
		overlap.y = min(myAABB.vMax.y, otherAABB.vMax.y) - max(myAABB.vMin.y, otherAABB.vMin.y);
		overlap.z = min(myAABB.vMax.z, otherAABB.vMax.z) - max(myAABB.vMin.z, otherAABB.vMin.z);

		// 가장 작은 축 방향으로 밀기
		_vec3 push(0.f, 0.f, 0.f);
		if (overlap.x <= overlap.y && overlap.x <= overlap.z)
			push.x = (vMyCenter.x > vOtherCenter.x) ? overlap.x : -overlap.x;
		else if (overlap.y <= overlap.z)
			push.y = (vMyCenter.y > vOtherCenter.y) ? overlap.y : -overlap.y;
		else
			push.z = (vMyCenter.z > vOtherCenter.z) ? overlap.z : -overlap.z;

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
			if (auto pRigid = m_pOwner->Get_Component<CRigidbody>())
			{
				pRigid->Set_OnGround(true);
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
	
	}
}

void CCollider::On_Collision_Stay(CCollider* pOther)
{
	ColliderType oType = pOther->Get_ColType();
	if (m_eType == ColliderType::ACTIVE && oType == ColliderType::PASSIVE)
	{
	
		const AABB& myAABB = Get_AABBW();
		const AABB& otherAABB = pOther->Get_AABBW();

		_vec3 vMyCenter = (myAABB.vMin + myAABB.vMax) * 0.5f;
		_vec3 vOtherCenter = (otherAABB.vMin + otherAABB.vMax) * 0.5f;

		_vec3 overlap;
		overlap.x = min(myAABB.vMax.x, otherAABB.vMax.x) - max(myAABB.vMin.x, otherAABB.vMin.x);
		overlap.y = min(myAABB.vMax.y, otherAABB.vMax.y) - max(myAABB.vMin.y, otherAABB.vMin.y);
		overlap.z = min(myAABB.vMax.z, otherAABB.vMax.z) - max(myAABB.vMin.z, otherAABB.vMin.z);

		// 가장 작은 축 방향으로 밀기
		_vec3 push(0.f, 0.f, 0.f);
		if (overlap.x <= overlap.y && overlap.x <= overlap.z)
			push.x = (vMyCenter.x > vOtherCenter.x) ? overlap.x : -overlap.x;
		else if (overlap.y <= overlap.z)
			push.y = (vMyCenter.y > vOtherCenter.y) ? overlap.y : -overlap.y;
		else
			push.z = (vMyCenter.z > vOtherCenter.z) ? overlap.z : -overlap.z;

		// Transform 얻기
		CTransform* pTransform = m_pOwner->Get_Component<CTransform>();
		if (pTransform)
		{
			_vec3 vPos = pTransform->Get_Pos();
			pTransform->Set_Pos(vPos + push);
		}



	}
}

void CCollider::On_Collision_Exit(CCollider* pOther)
{
	if (m_eType == ColliderType::ACTIVE && pOther->Get_ColTag() == ColliderTag::GROUND)
	{
		CRigidbody* pRigid = m_pOwner->Get_Component<CRigidbody>();
		if (pRigid)
			pRigid->Set_OnGround(false);
	}
}

void CCollider::Free()
{
	Safe_Release(m_pVB);
	Safe_Release(m_pIB);
	Safe_Release(m_pGraphicDev);
	//Safe_Release(m_pOwner);
}
