#include "CCollider.h"
#include "CRenderMgr.h"
#include "CTransform.h"
#include "CCollisionMgr.h"
#include "CRigidBody.h"
#include "CGameObject.h"
#include "CCameraMgr.h"


class CGameObject;

CCollider::CCollider(LPDIRECT3DDEVICE9 pGraphicDev)
	: CComponent(pGraphicDev)
{
	m_tAABB = { {-1.f, -1.f, -1.f}, {1.f, 1.f, 1.f} };
	m_tAABBOff = { {0.f, 0.f, 0.f}, {0.f, 0.f, 0.f} };
	m_tAABBWorld = { {0.f, 0.f, 0.f}, {0.f, 0.f, 0.f} };

	m_tBound.eType = BoundingType::AABB;
	m_tBound.vMin = _vec3(0.f, 0.f, 0.f);
	m_tBound.vMax = _vec3(0.f, 0.f, 0.f);
	m_tBound.vCenter = _vec3(0.f, 0.f, 0.f);
	m_tBound.vAxisX = _vec3(1.f, 0.f, 0.f);
	m_tBound.vAxisY = _vec3(0.f, 1.f, 0.f);
	m_tBound.vAxisZ = _vec3(0.f, 0.f, 1.f);
	m_tBound.vHalf = _vec3(1.f, 1.f, 1.f);
	m_tBound.vCorners.clear();
	m_tBound.vCorners.resize(8, _vec3(0.f, 0.f, 0.f));
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
	if (m_eState == ColliderState::ENTER) m_eState = ColliderState::STAY;
	else if (m_eState == ColliderState::EXIT) m_eState = ColliderState::NONE;

	CTransform* pTransform = m_pOwner->Get_Component<CTransform>();
	if (!pTransform) return;
	const _matrix* pWorld = pTransform->Get_WorldMatrix();

	AABB aabb;
	aabb.vMin = m_tAABB.vMin + m_tAABBOff.vMin;
	aabb.vMax = m_tAABB.vMax + m_tAABBOff.vMax;

	// 8개의 로컬 꼭짓점 → 월드 변환
	_vec3 corners[8] = {
		{aabb.vMin.x, aabb.vMin.y, aabb.vMin.z}, {aabb.vMax.x, aabb.vMin.y, aabb.vMin.z},
		{aabb.vMax.x, aabb.vMax.y, aabb.vMin.z}, {aabb.vMin.x, aabb.vMax.y, aabb.vMin.z},
		{aabb.vMin.x, aabb.vMin.y, aabb.vMax.z}, {aabb.vMax.x, aabb.vMin.y, aabb.vMax.z},
		{aabb.vMax.x, aabb.vMax.y, aabb.vMax.z}, {aabb.vMin.x, aabb.vMax.y, aabb.vMax.z}
	};

	for (int i = 0; i < 8; ++i)
		D3DXVec3TransformCoord(&corners[i], &corners[i], pWorld);

	// AABB 월드 좌표 재계산
	m_tAABBWorld.vMin = m_tAABBWorld.vMax = corners[0];
	for (int i = 1; i < 8; ++i)
	{
		m_tAABBWorld.vMin.x = min(m_tAABBWorld.vMin.x, corners[i].x);
		m_tAABBWorld.vMin.y = min(m_tAABBWorld.vMin.y, corners[i].y);
		m_tAABBWorld.vMin.z = min(m_tAABBWorld.vMin.z, corners[i].z);
		m_tAABBWorld.vMax.x = max(m_tAABBWorld.vMax.x, corners[i].x);
		m_tAABBWorld.vMax.y = max(m_tAABBWorld.vMax.y, corners[i].y);
		m_tAABBWorld.vMax.z = max(m_tAABBWorld.vMax.z, corners[i].z);
	}

	// Bounding 정보 계산
	if (m_tBound.eType == BoundingType::OBB)
	{
		// OBB 계산 (축 방향 반지름 계산 및 변환)
		_vec3 vHalf = (aabb.vMax - aabb.vMin) * 0.5f;
		_vec3 vScale = {
			D3DXVec3Length((_vec3*)&pWorld->_11),
			D3DXVec3Length((_vec3*)&pWorld->_21),
			D3DXVec3Length((_vec3*)&pWorld->_31)
		};
		m_tBound.vHalf = _vec3(vHalf.x * vScale.x, vHalf.y * vScale.y, vHalf.z * vScale.z);
		m_tBound.Calc_Transform(*pWorld);
	}
	else
	{
		// AABB이지만 OBB 계산기(Calc_Push_OBB)를 위해 꼭짓점 저장
		m_tBound.vCorners.clear();
		m_tBound.vCorners.reserve(8);
		for (int i = 0; i < 8; ++i)
			m_tBound.vCorners.push_back(corners[i]);

		// 중심 위치 갱신
		m_tBound.vCenter = (m_tAABBWorld.vMin + m_tAABBWorld.vMax) * 0.5f;

		// 단위 벡터 설정 (축 고정)
		m_tBound.vAxisX = _vec3(1.f, 0.f, 0.f);
		m_tBound.vAxisY = _vec3(0.f, 1.f, 0.f);
		m_tBound.vAxisZ = _vec3(0.f, 0.f, 1.f);

		// 반지름 길이 갱신
		m_tBound.vHalf = (m_tAABBWorld.vMax - m_tAABBWorld.vMin) * 0.5f;
	}

	// 충돌 매니저 등록
	CCollisionMgr::Get_Instance()->Add_Collider(this);
}

void CCollider::LateUpdate_Component()
{
	CRenderMgr::Get_Instance()->Add_Collider(this);
}

void CCollider::Render(LPDIRECT3DDEVICE9 pDevice)
{
	const _matrix* pView = CCameraMgr::Get_Instance()->Get_MainViewMatrix();
	const _matrix* pProj = CCameraMgr::Get_Instance()->Get_MainProjectionMatrix();

	pDevice->SetTransform(D3DTS_VIEW, pView);
	pDevice->SetTransform(D3DTS_PROJECTION, pProj);

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

	bool bHasOffset = (m_tAABBOff.vMin != _vec3(0.f, 0.f, 0.f) || m_tAABBOff.vMax != _vec3(0.f, 0.f, 0.f));
	D3DCOLOR color = D3DCOLOR_ARGB(255, 255, 255, 255); // 기본 흰색
	if (m_tBound.eType == BoundingType::AABB)
	{
		color = bHasOffset ? D3DCOLOR_ARGB(255, 0, 255, 0)     // AABB + Offset : 연두
			: D3DCOLOR_ARGB(255, 0, 128, 0);     // AABB + No Offset : 짙은 초록
	}
	else if (m_tBound.eType == BoundingType::OBB)
	{
		color = bHasOffset ? D3DCOLOR_ARGB(255, 255, 0, 0)     // OBB + Offset : 빨강
			: D3DCOLOR_ARGB(255, 128, 0, 0);     // OBB + No Offset : 짙은 빨강
	}

	if (m_tBound.eType == BoundingType::OBB)
	{
		for (int i = 0; i < 8; ++i)
			pVertices[i] = { m_tBound.vCorners[i], color };
	}
	else // AABB용
	{
		_vec3 min = m_tAABBWorld.vMin;
		_vec3 max = m_tAABBWorld.vMax;

		pVertices[0] = { {min.x, min.y, min.z}, color };
		pVertices[1] = { {max.x, min.y, min.z}, color };
		pVertices[2] = { {max.x, max.y, min.z}, color };
		pVertices[3] = { {min.x, max.y, min.z}, color };
		pVertices[4] = { {min.x, min.y, max.z}, color };
		pVertices[5] = { {max.x, min.y, max.z}, color };
		pVertices[6] = { {max.x, max.y, max.z}, color };
		pVertices[7] = { {min.x, max.y, max.z}, color };
	}

	m_pVB->Unlock();

	_matrix matIdentity;
	D3DXMatrixIdentity(&matIdentity);
	pDevice->SetTransform(D3DTS_WORLD, &matIdentity);
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
		if (m_tBound.eType == BoundingType::AABB && pOther->Get_Bound().eType == BoundingType::AABB)
		{
			if (!Calc_Push_AABB(Get_AABBW(), pOther->Get_AABBW(), push))
				return;
		}
		else
		{
			if (!Calc_Push_OBB(Get_Bound(), pOther->Get_Bound(), push))
				return;
		}

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
			if (push.y > abs(push.x) && push.y > abs(push.z))
				m_pRigid->Set_OnGround(true);
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
		if (!Calc_Push_AABB(myAABB, otherAABB, push))	return;
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
		if (m_tBound.eType == BoundingType::AABB && pOther->Get_Bound().eType == BoundingType::AABB)
		{
			if (!Calc_Push_AABB(Get_AABBW(), pOther->Get_AABBW(), push))
				return;
		}
		else
		{
			if (!Calc_Push_OBB(Get_Bound(), pOther->Get_Bound(), push))
				return;
		}

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
		if (!Calc_Push_AABB(myAABB, otherAABB, push))	return;

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

bool CCollider::Calc_Push_AABB(const AABB& a, const AABB& b, _vec3& push)
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

bool CCollider::Calc_Push_OBB(const BoundInfo& a, const BoundInfo& b, _vec3& push)
{
	vector<_vec3> vAxis;
	
	vAxis.push_back(a.vAxisX);
	vAxis.push_back(a.vAxisY);
	vAxis.push_back(a.vAxisZ);

	vAxis.push_back(b.vAxisX);
	vAxis.push_back(b.vAxisY);
	vAxis.push_back(b.vAxisZ);

	_vec3 cross;
	for (int i = 0; i < 3; ++i)
	{
		_vec3 aAxis = (&a.vAxisX)[i];
		for (int j = 0; j < 3; ++j)
		{
			_vec3 bAxis = (&b.vAxisX)[j];
			D3DXVec3Cross(&cross, &aAxis, &bAxis);
			if (D3DXVec3LengthSq(&cross) > 0.0001f) // 유효한 축만
			{
				D3DXVec3Normalize(&cross, &cross);
				vAxis.push_back(cross);
			}
		}
	}

	float minOverlap = FLT_MAX;
	_vec3 mtvAxis = { 0.f, 0.f, 0.f };

	for (const _vec3& axis : vAxis)
	{
		float minA = 0.f, maxA = 0.f;
		float minB = 0.f, maxB = 0.f;

		// A 투영
		{
			float dot = D3DXVec3Dot(&a.vCorners[0], &axis);
			minA = maxA = dot;
			for (int i = 1; i < 8; ++i)
			{
				dot = D3DXVec3Dot(&a.vCorners[i], &axis);
				if (dot < minA) minA = dot;
				if (dot > maxA) maxA = dot;
			}
		}

		// B 투영
		{
			float dot = D3DXVec3Dot(&b.vCorners[0], &axis);
			minB = maxB = dot;
			for (int i = 1; i < 8; ++i)
			{
				dot = D3DXVec3Dot(&b.vCorners[i], &axis);
				if (dot < minB) minB = dot;
				if (dot > maxB) maxB = dot;
			}
		}

		float overlap = min(maxA, maxB) - max(minA, minB);
		if (overlap <= 0.f)
			return false; // 분리 축 발견 → 충돌 아님

		if (overlap < minOverlap)
		{
			minOverlap = overlap;
			mtvAxis = axis;

			// 중심 기준 방향 보정
			_vec3 dir = a.vCenter - b.vCenter;
			if (D3DXVec3Dot(&dir, &axis) < 0.f)
				mtvAxis = -mtvAxis;
		}
	}

	push = mtvAxis * minOverlap;
	return true;
}


void CCollider::Free()
{
	Safe_Release(m_pVB);
	Safe_Release(m_pIB);
	Safe_Release(m_pGraphicDev);
	//Safe_Release(m_pOwner);
	m_pRigid = nullptr;
}
