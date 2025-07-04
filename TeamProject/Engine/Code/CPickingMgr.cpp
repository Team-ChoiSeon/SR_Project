#include "Engine_Define.h"
#include "CPickingMgr.h"
#include "CGameObject.h"
#include "CPickTarget.h"
#include "CScene.h"

IMPLEMENT_SINGLETON(CPickingMgr);
CPickingMgr::CPickingMgr()
{
}

CPickingMgr::~CPickingMgr()
{
}

void CPickingMgr::Ready_Picking(LPDIRECT3DDEVICE9 GraphicDev, HWND hWnd)
{
	m_pGraphicDev = GraphicDev;
	m_pGraphicDev->AddRef();

	m_hWnd = hWnd;
}

HRESULT CPickingMgr::Update_Picking(const float& fTimeDelta)
{
	if(!m_vecHit.empty())
		m_vecHit.clear();

	ComputeCursor();
	Make_Ray(m_Cursor.x, m_Cursor.y);		//ray 생성
	TransformRayIntoWorld();					//ray를 월드공간으로 변화
	//Check_CollisionWorld();
	return S_OK;
}

_int CPickingMgr::LateUpdate_Picking(const float& fTimeDelta)
{
	return _int();
}

void CPickingMgr::Check_CollisionWorld()
{
	//Ray_Hit hitinfo = {};
	//for (auto& iter : m_vecPickable)
	//{
	//	switch (iter->Get_Component<CPickTarget>()->Get_RayCollisionType())
	//	{
	//	case COL_AABB:
	//		break;
	//	case COL_OBB:
	//		break;
	//	case COL_SPHERE:
	//		break;
	//	case COL_TRI:
	//		float fu, fv;
	//		Ray_Tri tri = iter->Get_Component<CPickTarget>()->Get_RayCollisionStruct();
	//		if (D3DXIntersectTri(&tri._v0, &tri._v1, &tri._v2, &m_pRay->_position, &m_pRay->_direction, &fu, &fv, &hitinfo._distance))				//hit true;
	//		{
	//			_vec3 vu = (fu * (tri._v1 - tri._v0));
	//			_vec3 vv = (fv * (tri._v2 - tri._v0));
	//			float w0 = 1.0f - fu - fv;
	//			_vec3 vNormal = tri._nv0 * w0 + tri._nv1 * fu + tri._nv2 * fv;
	//			D3DXVec3Normalize(&vNormal, &vNormal);
	//			hitinfo._hittedobject = iter;
	//			hitinfo._hittedcoord = tri._v0 + fu * (tri._v1 - tri._v0) + fv * (tri._v2 - tri._v0);
	//			hitinfo._hitteduv = { D3DXVec3Length(&vu), D3DXVec3Length (&vv)};
	//			hitinfo._hittedcoordnormal = vNormal;

	//			auto it = std::find_if(m_vecHit.begin(), m_vecHit.end(),
	//				[&](const Ray_Hit* h) { return h->_hittedobject == hitinfo._hittedobject; });			//이미 hit 되어있는 경우
	//			if (it != m_vecHit.end()) {
	//				*it = &hitinfo;          
	//			}
	//			else {
	//				m_vecHit.push_back(&hitinfo);
	//			}


	//		}
	//		break;
	//	}
	//}
}

void CPickingMgr::Free()
{
	Safe_Release(m_pGraphicDev);
}

CGameObject* CPickingMgr::Get_HitTarget()
{
	auto pFirstObject = min_element(m_vecHit.begin(), m_vecHit.end(), [](const Ray_Hit* a, const Ray_Hit* b) {return a->_distance < b->_distance; });
	if (pFirstObject == m_vecHit.end())
		return nullptr;
	else
		return m_vecHit.front()->_hittedobject;
}

void CPickingMgr::ComputeCursor()
{
	GetCursorPos(&m_Cursor);
	ScreenToClient(m_hWnd, &m_Cursor);
}

void CPickingMgr::Make_Ray(float x, float y)
{
	float px = 0.f;
	float py = 0.f;	

	m_pGraphicDev->GetViewport(&m_vp);
	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &m_mProj);

	px = (((2.f * x) / m_vp.Width) - 1.f) / m_mProj(0, 0);
	py = (((-2.f * y) / m_vp.Height) + 1.f) / m_mProj(1, 1);

	m_pRay->_position = _vec3(0, 0, 0);
	m_pRay->_direction = _vec3(px, py, 1.f);

	return;
}

void CPickingMgr::TransformRayIntoWorld()
{
	_matrix view, inverseview;
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &view);
	D3DXMatrixInverse(&inverseview, 0, &view);

	D3DXVec3TransformCoord(&m_pRay->_position, &m_pRay->_position, &inverseview);
	D3DXVec3TransformNormal(&m_pRay->_direction, &m_pRay->_direction, &inverseview);

	D3DXVec3Normalize(&m_pRay->_direction, &m_pRay->_direction);
}

//void PickingMgr::TransformRayIntoLocal(_matrix* targetWorldMatrix)
//{
//	_matrix world, inverseworld;
//	world = *targetWorldMatrix;
//	D3DXMatrixInverse(&inverseworld, 0, &world);
//}

void CPickingMgr::SortHitVectorASC()
{
	sort(m_vecHit.begin(), m_vecHit.end(), [](const Ray_Hit* a, const Ray_Hit* b) {return a->_distance < b->_distance; });
}

void CPickingMgr::SortHitVectorDESC()
{
	sort(m_vecHit.begin(), m_vecHit.end(), [](const Ray_Hit* a, const Ray_Hit* b) {return a->_distance > b->_distance; });

}
