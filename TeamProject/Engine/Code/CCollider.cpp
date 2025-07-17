#include "CCollider.h"
#include "CRenderMgr.h"
#include "CTransform.h"
#include "CCollisionMgr.h"
#include "CRigidBody.h"
#include "CGameObject.h"
#include "CCameraMgr.h"


class CGameObject;
const _vec3 up(0.f, 1.f, 0.f);

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
	Ready_Component();
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
HRESULT CCollider::Ready_Component()
{
	if (FAILED(m_pGraphicDev->CreateVertexBuffer(sizeof(VTXLINE) * 8, 0, FVF_LINE,
		D3DPOOL_MANAGED, &m_pVB, nullptr)))
		return E_FAIL;

	if (FAILED(m_pGraphicDev->CreateIndexBuffer(sizeof(WORD) * 24, 0, D3DFMT_INDEX16,
		D3DPOOL_MANAGED, &m_pIB, nullptr)))
		return E_FAIL;

	WORD* pIndices = nullptr;
	m_pIB->Lock(0, 0, (void**)&pIndices, 0);
	WORD indices[24] = {
		0,1, 1,2, 2,3, 3,0,
		4,5, 5,6, 6,7, 7,4,
		0,4, 1,5, 2,6, 3,7
	};
	memcpy(pIndices, indices, sizeof(indices));

	m_pIB->Unlock();

	return S_OK;
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

	_vec3 corners[8] = {
		{aabb.vMin.x, aabb.vMin.y, aabb.vMin.z}, {aabb.vMax.x, aabb.vMin.y, aabb.vMin.z},
		{aabb.vMax.x, aabb.vMax.y, aabb.vMin.z}, {aabb.vMin.x, aabb.vMax.y, aabb.vMin.z},
		{aabb.vMin.x, aabb.vMin.y, aabb.vMax.z}, {aabb.vMax.x, aabb.vMin.y, aabb.vMax.z},
		{aabb.vMax.x, aabb.vMax.y, aabb.vMax.z}, {aabb.vMin.x, aabb.vMax.y, aabb.vMax.z}
	};

	for (int i = 0; i < 8; ++i)
		D3DXVec3TransformCoord(&corners[i], &corners[i], pWorld);

	// AABB ���� ��ǥ ����
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

	// Bounding ���� ���
	if (m_tBound.eType == BoundingType::OBB)
	{
		// OBB ĳ��
		// Update_Component()���� ���� ��Ʈ���� ���� ���� �ÿ���
		if (memcmp(&m_matPrevWorld, pWorld, sizeof(_matrix)) != 0)
		{
			// Calc_Transform() ȣ�� 
			m_matPrevWorld = *pWorld;

			_vec3 vHalf = (aabb.vMax - aabb.vMin) * 0.5f;
			_vec3 vScale = {
				D3DXVec3Length((_vec3*)&pWorld->_11),
				D3DXVec3Length((_vec3*)&pWorld->_21),
				D3DXVec3Length((_vec3*)&pWorld->_31)
			};
			m_tBound.vHalf = _vec3(vHalf.x * vScale.x, vHalf.y * vScale.y, vHalf.z * vScale.z);
			m_tBound.Calc_Transform(*pWorld);
		}
	}

	else
	{
		// AABB������ OBB ����(Calc_Push_OBB)�� ���� ������ ����
		m_tBound.vCorners.clear();
		m_tBound.vCorners.reserve(8);
		for (int i = 0; i < 8; ++i)
			m_tBound.vCorners.push_back(corners[i]);


		m_tBound.vCenter = (m_tAABBWorld.vMin + m_tAABBWorld.vMax) * 0.5f;


		m_tBound.vAxisX = _vec3(1.f, 0.f, 0.f);
		m_tBound.vAxisY = _vec3(0.f, 1.f, 0.f);
		m_tBound.vAxisZ = _vec3(0.f, 0.f, 1.f);


		m_tBound.vHalf = (m_tAABBWorld.vMax - m_tAABBWorld.vMin) * 0.5f;
	}


	CCollisionMgr::Get_Instance()->Add_Collider(this);
}

void CCollider::LateUpdate_Component(const _float& fTimeDelta)
{
	CRenderMgr::Get_Instance()->Add_Collider(this);
}

void CCollider::Render(LPDIRECT3DDEVICE9 pDevice)
{
	const _matrix* pView = CCameraMgr::Get_Instance()->Get_MainViewMatrix();
	const _matrix* pProj = CCameraMgr::Get_Instance()->Get_MainProjectionMatrix();

	pDevice->SetTransform(D3DTS_VIEW, pView);
	pDevice->SetTransform(D3DTS_PROJECTION, pProj);

	VTXLINE* pVertices = nullptr;
	m_pVB->Lock(0, 0, (void**)&pVertices, 0);

	_matrix matIdentity;
	D3DXMatrixIdentity(&matIdentity);
	pDevice->SetTransform(D3DTS_WORLD, &matIdentity);
	pDevice->SetFVF(FVF_LINE);
	pDevice->SetStreamSource(0, m_pVB, 0, sizeof(VTXLINE));
	pDevice->SetIndices(m_pIB);

	D3DCOLOR color;
	// OBB
	if(m_tBound.eType==BoundingType::OBB)
	{
		color = D3DCOLOR_ARGB(255, 255, 0, 0); 
		for (int i = 0; i < 8; ++i)
			pVertices[i] = { m_tBound.vCorners[i], color };

		m_pVB->Unlock();
		pDevice->DrawIndexedPrimitive(D3DPT_LINELIST, 0, 0, 8, 0, 12);
	}
	
	// AABB
	m_pVB->Lock(0, 0, (void**)&pVertices, 0); 
	_vec3 min = m_tAABBWorld.vMin;
	_vec3 max = m_tAABBWorld.vMax;
	color = D3DCOLOR_ARGB(255, 0, 255, 0); 
	pVertices[0] = { {min.x, min.y, min.z}, color };
	pVertices[1] = { {max.x, min.y, min.z}, color };
	pVertices[2] = { {max.x, max.y, min.z}, color };
	pVertices[3] = { {min.x, max.y, min.z}, color };
	pVertices[4] = { {min.x, min.y, max.z}, color };
	pVertices[5] = { {max.x, min.y, max.z}, color };
	pVertices[6] = { {max.x, max.y, max.z}, color };
	pVertices[7] = { {min.x, max.y, max.z}, color };

	m_pVB->Unlock();
	pDevice->DrawIndexedPrimitive(D3DPT_LINELIST, 0, 0, 8, 0, 12);

}

void CCollider::On_Collision_Enter(CCollider* pOther)
{
	m_pOther = pOther;
	Handle_Collision(pOther);

	if (m_eState == ColliderState::NONE || m_eState == ColliderState::EXIT)
		m_eState = ColliderState::ENTER;
}

void CCollider::On_Collision_Stay(CCollider* pOther)
{
	Handle_Collision(pOther);
	m_eState = ColliderState::STAY;
}

void CCollider::On_Collision_Exit(CCollider* pOther)
{
	if (m_pOther == pOther)	m_pOther = nullptr;

	if (m_eType == ColliderType::ACTIVE && pOther->Get_ColTag() == ColliderTag::GROUND)
	{
		if (m_pRigid)
			m_pRigid->Set_OnGround(false);
	}

	m_eState = ColliderState::EXIT;
}

bool CCollider::Broad_Phase(CCollider* pOther)
{
	const AABB& a = this->Get_AABBW();
	const AABB& b = pOther->Get_AABBW();

	return !(a.vMax.x < b.vMin.x || a.vMin.x > b.vMax.x ||
		a.vMax.y < b.vMin.y || a.vMin.y > b.vMax.y ||
		a.vMax.z < b.vMin.z || a.vMin.z > b.vMax.z);
}

bool CCollider::Narrow_Phase(CCollider* pOther, _vec3& push)
{
	const auto& myBound = this->Get_Bound();
	const auto& otherBound = pOther->Get_Bound();

	if (myBound.eType == BoundingType::AABB && otherBound.eType == BoundingType::AABB)
		return Calc_Push_AABB(Get_AABBW(), pOther->Get_AABBW(), push);
	else
		return Calc_Push_OBB(myBound, otherBound, push);
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
			if (D3DXVec3LengthSq(&cross) > 0.0001f)
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
			return false;

		if (overlap < minOverlap)
		{
			minOverlap = overlap;
			mtvAxis = axis;

			_vec3 dir = a.vCenter - b.vCenter;
			if (D3DXVec3Dot(&dir, &axis) < 0.f)
				mtvAxis = -mtvAxis;
		}
	}

	push = mtvAxis * minOverlap;
	return true;
}

void CCollider::Handle_Collision(CCollider* pOther)
{
	ColliderType oType = pOther->Get_ColType();
	if (m_eType != ColliderType::ACTIVE || oType == ColliderType::TRIGGER)
		return;

	_vec3 push(0.f, 0.f, 0.f);
	if (Calc_Push_OBB(Get_Bound(), pOther->Get_Bound(), push))
	{
		// 바닥 전용 처리
		if (pOther->Get_ColTag() == ColliderTag::GROUND)
		{
			Handle_Ground(pOther, push);
		}
		
		else
		{
			Handle_Active(pOther, push);
		}
	}
}


// 둘중 하나 이상이 ACTIVE일때의 처리
void CCollider::Handle_Active(CCollider* pOther, const _vec3& push)
{
	CRigidBody* pRigid1 = m_pRigid;
	CRigidBody* pRigid2 = pOther->m_pRigid;

	// 내 Rigidbody가 없으면 아무 처리도 하지 않음
	if (!pRigid1) return;

	// --- Case 1: 상대가 움직이지 않는 PASSIVE 물체일 경우 ---
	if (pOther->Get_ColType() == ColliderType::PASSIVE || pRigid2 == nullptr)
	{
		// 위치 보정: 나만 100% 밀려남
		m_pOwner->Get_Component<CTransform>()->Move_Pos(&push, 1.f, 1.f);

		// 속도 보정: 수직으로 부딪혔을 때만 내 수직 속도를 0으로 만들어 진동 방지
		_vec3 vPushDir = push;
		D3DXVec3Normalize(&vPushDir, &vPushDir);
		if (D3DXVec3Dot(&vPushDir, &up) > 0.5f)
		{
			pRigid1->Set_OnGround(true);

			_vec3 vVel = pRigid1->Get_Velocity();
			if (vVel.y < 0.f)
			{
				vVel.y = 0.f;
				pRigid1->Set_Velocity(vVel);
			}
		}
	}
	// --- Case 2: 둘 다 움직이는 ACTIVE 물체일 경우 ---
	else
	{
		// 충돌 방향을 미리 계산하여 수직/수평 여부 판단
		_vec3 vPushDir = push;
		D3DXVec3Normalize(&vPushDir, &vPushDir);
		float fVerticality = abs(D3DXVec3Dot(&vPushDir, &up));

		// --- 2-1: 수직에 가까운 충돌 (쌓기) ---
		// 안정성을 위해 한쪽이 미는 '우선순위' 방식을 사용
		if (fVerticality > 0.7f)
		{
			CTransform* pTransform1 = m_pOwner->Get_Component<CTransform>();
			CTransform* pTransform2 = pOther->m_pOwner->Get_Component<CTransform>();

			// Y축 위치를 기준으로 더 높은 쪽(Pushed)을 결정
			CGameObject* pPushedObject = (pTransform1->Get_Pos().y > pTransform2->Get_Pos().y) ? m_pOwner : pOther->m_pOwner;

			// push 벡터 방향을 항상 '아래에서 위로' 향하도록 보정
			_vec3 vFinalPush = push;
			if (m_pOwner != pPushedObject) // 내가 아래쪽(Pusher)이었다면 push 방향을 뒤집음
			{
				vFinalPush = -vFinalPush;
			}

			// 위쪽(Pushed) 오브젝트의 위치와 속도만 보정하여 안정화
			CTransform* pPushedTransform = pPushedObject->Get_Component<CTransform>();
			CRigidBody* pPushedRigid = pPushedObject->Get_Component<CRigidBody>();

			if (pPushedTransform) pPushedTransform->Move_Pos(&vFinalPush, 1.f, 1.f);

			if (pPushedRigid)
			{
				pPushedRigid->Set_OnGround(true);
				_vec3 vVel = pPushedRigid->Get_Velocity();
				if (vVel.y < 0.f)
				{
					vVel.y = 0.f;
					pPushedRigid->Set_Velocity(vVel);
				}
			}
		}
		// --- 2-2: 수평에 가까운 충돌 (밀기) ---
		// 물리적인 느낌을 위해 '충격량'을 전달하는 방식을 사용
		else
		{
			// 위치 보정: 질량에 따라 공평하게 분배
			float m1 = pRigid1->Get_Mass();
			float m2 = pRigid2->Get_Mass();
			float totalMass = m1 + m2;

			if (totalMass > 0.f)
			{
				_vec3 vMyPush = push * (m2 / totalMass);
				_vec3 vOtherPush = -push * (m1 / totalMass);
				m_pOwner->Get_Component<CTransform>()->Move_Pos(&vMyPush, 1.f, 1.f);
				pOther->m_pOwner->Get_Component<CTransform>()->Move_Pos(&vOtherPush, 1.f, 1.f);
			}

			// 충격량 기반 속도 교환 (1차원 탄성 충돌 공식)
			_vec3 vVel1 = pRigid1->Get_Velocity();
			_vec3 vVel2 = pRigid2->Get_Velocity();

			// 충돌 축에 대한 각 속도의 성분
			float v1_on_axis = D3DXVec3Dot(&vVel1, &vPushDir);
			float v2_on_axis = D3DXVec3Dot(&vVel2, &vPushDir);

			// 충돌 후 새로운 속도 성분 계산
			float v1_new_on_axis = (v1_on_axis * (m1 - m2) + 2 * m2 * v2_on_axis) / totalMass;
			float v2_new_on_axis = (v2_on_axis * (m2 - m1) + 2 * m1 * v1_on_axis) / totalMass;

			// 실제 속도 변화량(충격량) 계산
			float impulse1 = v1_new_on_axis - v1_on_axis;
			float impulse2 = v2_new_on_axis - v2_on_axis;

			// 계산된 충격량을 각 Rigidbody에 Add_Velocity로 적용
			pRigid1->Add_Velocity(vPushDir * impulse1);
			pRigid2->Add_Velocity(vPushDir * impulse2);
		}
	}
}

	if (!m_pRigid) return;

	_vec3 vPushDir = push;
	D3DXVec3Normalize(&vPushDir, &vPushDir);

	if (D3DXVec3Dot(&vPushDir, &up) > 0.5f) // 수직 충돌 확인
	{
		m_pRigid->Set_OnGround(true);

		_vec3 vVel = m_pRigid->Get_Velocity();

		if (vVel.y < 0.f)
		{
			const float fMinBounceVelocity = 1.0f;

			// 빠르게 부딪혔을 때: 탄성 적용
			if (vVel.y < -fMinBounceVelocity)
			{
				vVel.y *= -m_pRigid->Get_Bounce();
			}
			// 살포시 내려앉았을 때: 진동 방지를 위해 속도 0으로
			else
			{
				vVel.y = 0.f;
			}
			m_pRigid->Set_Velocity(vVel);
		}
		// 위로 움직이는 중(점프 등)일때는 속도를 건드리지 않습니다
	}
}


//void CCollider::Handle_Ground(CCollider* pOther, const _vec3& push)
//{
//	if (!m_pRigid || pOther->Get_ColTag() != ColliderTag::GROUND)
//		return;
//
//	if (D3DXVec3LengthSq(&push) < 0.0001f)
//		return;
//
//	_vec3 vPush = push;
//	D3DXVec3Normalize(&vPush, &vPush);
//	_vec3 vUp = { 0.f,1.f,0.f };
//	_float fDot = D3DXVec3Dot(&vPush, &vUp);
//
//	if (fDot > 0.5f)
//	{
//		m_pRigid->Set_OnGround(true);
//	}
//}



void CCollider::Free()
{
	if (CCollisionMgr::Get_Instance())
	{
		CCollisionMgr::Get_Instance()->Remove_Collider(this);
	}
	Safe_Release(m_pVB);
	Safe_Release(m_pIB);
	Safe_Release(m_pGraphicDev);
	//Safe_Release(m_pOwner);
	m_pRigid = nullptr;
	m_pOther = nullptr;
}
