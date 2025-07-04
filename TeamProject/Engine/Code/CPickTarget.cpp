#include "CPickTarget.h"
#include "CPickingMgr.h"
#include "CTransform.h"
#include "CGameObject.h"

CPickTarget::CPickTarget()
{
}

CPickTarget::CPickTarget(LPDIRECT3DDEVICE9 pGraphicDev, RAYCHECKTYPE boundtype)
	: CComponent(pGraphicDev), m_eRayCheckType(boundtype)
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
	Safe_Release(m_pGraphicDev);
}

bool CPickTarget::Check_Collision()
{
	m_RayHitInfo = {};
	Ray ray = CPickingMgr::Get_Instance()->Get_Ray();
	m_pRay = &ray;
	switch (m_eRayCheckType)
	{
	case RAY_AABB:
		break;
	case RAY_OBB:
		break;
	case RAY_SPHERE:
		break;
	case RAY_TRI:
		float fu, fv;
		if (D3DXIntersectTri(&m_pRayTri->_v0, &m_pRayTri->_v1, &m_pRayTri->_v2, &m_pRay->_position, &m_pRay->_direction, &fu, &fv, &m_RayHitInfo._distance))				//hit true;
		{
			_vec3 vu = (fu * (m_pRayTri->_v1 - m_pRayTri->_v0));
			_vec3 vv = (fv * (m_pRayTri->_v2 - m_pRayTri->_v0));
			float w0 = 1.0f - fu - fv;
			_vec3 vNormal = m_pRayTri->_nv0 * w0 + m_pRayTri->_nv1 * fu + m_pRayTri->_nv2 * fv;
			D3DXVec3Normalize(&vNormal, &vNormal);
			m_RayHitInfo._hittedobject = m_pOwner;
			m_RayHitInfo._hittedcoord = m_pRayTri->_v0 + vu + vv;
			m_RayHitInfo._hitteduv = { D3DXVec3Length(&vu), D3DXVec3Length(&vv) };
			m_RayHitInfo._hittedcoordnormal = vNormal;
			return true;
		}
		else
			return false;
		break;
	}
}

void CPickTarget::Add_HitInfo()
{
	auto it = std::find_if(CPickingMgr::Get_Instance()->Get_HitTargetList()->begin(), CPickingMgr::Get_Instance()->Get_HitTargetList()->end(),
		[&](const Ray_Hit h) { return h._hittedobject == m_RayHitInfo._hittedobject; });			//이미 hit 되어있는 경우
	if (it != CPickingMgr::Get_Instance()->Get_HitTargetList()->end()) {
		*it = m_RayHitInfo;
	}
	else {
		CPickingMgr::Get_Instance()->Get_HitTargetList()->push_back(m_RayHitInfo);
	}
}

void CPickTarget::TranslationIntoWorld()
{
	_matrix world = *m_pOwner->Get_Component<CTransform>()->Get_WorldMatrix();

	//_vec3* temp = &m_pRayTri->_v0;
	////for(int i = 0; i < size(m_pRayTri), )

	//for (const Ray_Tri& tri : m_pRayTri) {
	//	std::cout << "이름: " << person.name << ", 나이: " << person.age << std::endl;
	//}

	//for (auto memberPtr : { &Ray_Tri::_v0, &Ray_Tri::_v1, &Ray_Tri::_v2 }) {
	//	_vec3& v = m_pRayTri->*memberPtr;  // 포인터‐to‐멤버를 통해 해당 벡터에 접근
	//	D3DXVec3TransformCoord(&v, &v, &world);
	//}
	//// 노멀(법선)들에 대해서는 TransformNormal + 정규화
	//for (auto memberPtr : { &Ray_Tri::_nv0, &Ray_Tri::_nv1, &Ray_Tri::_nv2 }) {
	//	_vec3& n = m_pRayTri->*memberPtr;
	//	D3DXVec3TransformNormal(&n, &n, &world);
	//	D3DXVec3Normalize(&n, &n);
	//}
}
