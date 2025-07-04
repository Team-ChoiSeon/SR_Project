#pragma once
#include "CPickTarget.h"
#include "CPickingMgr.h"
#include "CTransform.h"
#include "CGameObject.h"
#include "CCollider.h"

CPickTarget::CPickTarget()
{
}

CPickTarget::CPickTarget(LPDIRECT3DDEVICE9 pGraphicDev, RAYCHECKTYPE boundtype)
	: m_eRayCheckType(boundtype)
{
}

CPickTarget::CPickTarget(const CPickTarget& rhs)
{
}

CPickTarget::~CPickTarget()
{
}

HRESULT CPickTarget::Ready_Component()
{
	return S_OK;
}

void CPickTarget::Update_Component(const float& fTimeDelta)
{
	if (Check_Collision())
		Add_HitInfo();
}

void CPickTarget::LateUpdate_Component(const float& fTimeDelta)
{
}

CPickTarget* CPickTarget::Create(LPDIRECT3DDEVICE9 pGraphicDev, RAYCHECKTYPE boundtype)
{
	CPickTarget* pPickCom = new CPickTarget(pGraphicDev, boundtype);

	if (FAILED(pPickCom->Ready_Component()))
	{
		Safe_Release(pPickCom);
		MSG_BOX("PickTraget Component Create Failed");
		return nullptr;
	}

	return pPickCom;
}

void CPickTarget::Free()
{
	Safe_Release(m_pCollider);
}

bool CPickTarget::Check_Collision()
{
	switch (m_eRayCheckType)
	{
	case RAY_AABB:
		return CheckAABB();
		break;
	case RAY_OBB:
		return CheckOBB();
		break;
	case RAY_SPHERE:
		return CheckSPHERE();
		break;
	case RAY_TRI:
		return CheckTRI();
		break;
	}
}

void CPickTarget::Add_HitInfo()
{
	auto it = std::find_if(CPickingMgr::Get_Instance()->Get_HitTargetList().begin(), CPickingMgr::Get_Instance()->Get_HitTargetList().end(),
		[&]( Ray_Hit& h) { return h._hittedobject == m_RayHitInfo._hittedobject; });			//이미 hit 되어있는 경우
	if (it != CPickingMgr::Get_Instance()->Get_HitTargetList().end()) {
		*it = m_RayHitInfo;
	}
	else {
		CPickingMgr::Get_Instance()->Add_HitInfo(m_RayHitInfo);
	}
	//CPickingMgr::Get_Instance()->Add_HitInfo(&m_RayHitInfo);
}

void CPickTarget::TranslationIntoLocal()
{
	_matrix world = *m_pOwner->Get_Component<CTransform>()->Get_WorldMatrix();
	_matrix inverseworld;

	D3DXMatrixInverse(&inverseworld, 0, &world);

}

bool CPickTarget::CheckAABB()
{
	m_pRay = CPickingMgr::Get_Instance()->Get_Ray();
	m_pCollider = m_pOwner->Get_Component<CCollider>();
	// m_pCollider 가 유효한지 재확인
	if (!m_pCollider) return false;

	const AABB& box = m_pCollider->Get_AABBW();

	const float EPS = 1e-6f;
	float tMin = -FLT_MAX, tMax = FLT_MAX;

	for (int i = 0; i < 3; ++i) {
		float origin = (i == 0 ? m_pRay->_position.x : i == 1 ? m_pRay->_position.y : m_pRay->_position.z);
		float direction = (i == 0 ? m_pRay->_direction.x : i == 1 ? m_pRay->_direction.y : m_pRay->_direction.z);
		float minB = (i == 0 ? box.vMin.x : i == 1 ? box.vMin.y : box.vMin.z);
		float maxB = (i == 0 ? box.vMax.x : i == 1 ? box.vMax.y : box.vMax.z);

		if (fabsf(direction) < EPS) {
			if (origin < minB || origin > maxB)
				return false;
		}
		else {
			float t1 = (minB - origin) / direction;
			float t2 = (maxB - origin) / direction;
			if (t1 > t2) std::swap(t1, t2);

			tMin = max(tMin, t1);
			tMax = min(tMax, t2);
			if (tMin > tMax)
				return false;
		}
	}

	if (tMax < 0.0f)
		return false;

	float distance = (tMin >= 0.0f) ? tMin : tMax;
	m_RayHitInfo._distance = distance;
	m_RayHitInfo._hittedobject = m_pOwner;
	//m_RayHitInfo._hittedcoord = m_pRay->_position + m_pRay->_direction * distance;

	return true;
}

bool CPickTarget::CheckOBB()
{
	return false;
}

bool CPickTarget::CheckSPHERE()
{
	return false;
}

bool CPickTarget::CheckTRI()
{
	//float fu, fv;
		//if (D3DXIntersectTri(&m_pRayTri->_v0, &m_pRayTri->_v1, &m_pRayTri->_v2, &m_pRay->_position, &m_pRay->_direction, &fu, &fv, &m_RayHitInfo._distance))				//hit true;
		//{
		//	_vec3 vu = (fu * (m_pRayTri->_v1 - m_pRayTri->_v0));
		//	_vec3 vv = (fv * (m_pRayTri->_v2 - m_pRayTri->_v0));
		//	float w0 = 1.0f - fu - fv;
		//	_vec3 vNormal = m_pRayTri->_nv0 * w0 + m_pRayTri->_nv1 * fu + m_pRayTri->_nv2 * fv;
		//	D3DXVec3Normalize(&vNormal, &vNormal);
		//	m_RayHitInfo._hittedobject = m_pOwner;
		//	m_RayHitInfo._hittedcoord = m_pRayTri->_v0 + vu + vv;
		//	m_RayHitInfo._hitteduv = { D3DXVec3Length(&vu), D3DXVec3Length(&vv) };
		//	m_RayHitInfo._hittedcoordnormal = vNormal;
		//	return true;
		//}
		//else
		//	return false;
	return false;
}
