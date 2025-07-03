#include "CCollider.h"
#include "CRenderMgr.h"
#include "CTransform.h"
#include "CCollisionMgr.h"

CCollider::CCollider(AABB aabb)
	: m_tAABB(aabb)
{
}

CCollider::~CCollider()
{
}

CCollider* CCollider::Create(AABB aabb)
{
	CCollider* pCollider = new CCollider(aabb);
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

	// �ε��� ���� (12�� �� = 24 index)
	if (FAILED(pDevice->CreateIndexBuffer(sizeof(WORD) * 24, 0, D3DFMT_INDEX16,
		D3DPOOL_MANAGED, &m_pIB, nullptr)))
		return;

	// �ε��� ä���
	WORD* pIndices = nullptr;
	m_pIB->Lock(0, 0, (void**)&pIndices, 0);

	WORD indices[24] = {
		0,1, 1,2, 2,3, 3,0, // �ո�
		4,5, 5,6, 6,7, 7,4, // �޸�
		0,4, 1,5, 2,6, 3,7  // ����
	};
	memcpy(pIndices, indices, sizeof(indices));
	m_pIB->Unlock();

	if (!m_pVB || !m_pIB) return;

	VTXLINE* pVertices = nullptr;
	m_pVB->Lock(0, 0, (void**)&pVertices, 0);

	D3DCOLOR color = D3DCOLOR_ARGB(255, 0, 255, 0); // ���λ�

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

void CCollider::Free()
{
	Safe_Release(m_pVB);
	Safe_Release(m_pIB);
	Safe_Release(m_pGraphicDev);
}
