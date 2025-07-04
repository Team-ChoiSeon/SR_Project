#include "Engine_Define.h"
#include "CPickingMgr.h"
#include "CGameObject.h"
#include "CPickTarget.h"
#include "CScene.h"
#include "CCameraMgr.h"

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

HRESULT CPickingMgr::Update_Picking(const _float& fTimeDelta)
{
	if(!m_vecHit.empty())
		m_vecHit.clear();

	ComputeCursor();
	Make_Ray(m_Cursor.x, m_Cursor.y);			//ray 생성
	TransformRayIntoWorld();					//ray를 월드공간으로 변화
	return S_OK;
}

_int CPickingMgr::LateUpdate_Picking(const _float& fTimeDelta)
{
	return _int();
}

void CPickingMgr::Free()
{
	Safe_Release(m_pGraphicDev);
}

CGameObject* CPickingMgr::Get_HitTarget()
{
	auto pFirstObject = min_element(m_vecHit.begin(), m_vecHit.end(), [](const Ray_Hit& a, const Ray_Hit& b) {return a._distance < b._distance; });
	if (pFirstObject == m_vecHit.end())
		return nullptr;
	else
		return m_vecHit.front()._hittedobject;
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
	m_mProj = *CCameraMgr::Get_Instance()->Get_MainProjectionMatrix();

	px = (((2.f * x) / m_vp.Width) - 1.f) / m_mProj(0, 0);
	py = (((-2.f * y) / m_vp.Height) + 1.f) / m_mProj(1, 1);

	m_Ray._position = _vec3(0, 0, 0);
	m_Ray._direction = _vec3(px, py, 1.f);

	return;
}

void CPickingMgr::TransformRayIntoWorld()
{
	_matrix view, inverseview;
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &view);
	D3DXMatrixInverse(&inverseview, 0, &view);

	D3DXVec3TransformCoord(&m_Ray._position, &m_Ray._position, &inverseview);
	D3DXVec3TransformNormal(&m_Ray._direction, &m_Ray._direction, &inverseview);

	D3DXVec3Normalize(&m_Ray._direction, &m_Ray._direction);
}

//void PickingMgr::TransformRayIntoLocal(_matrix* targetWorldMatrix)
//{
//	_matrix world, inverseworld;
//	world = *targetWorldMatrix;
//	D3DXMatrixInverse(&inverseworld, 0, &world);
//}

void CPickingMgr::SortHitVectorASC()
{
	sort(m_vecHit.begin(), m_vecHit.end(), [](const Ray_Hit& a, const Ray_Hit& b) {return a._distance < b._distance; });
}

void CPickingMgr::SortHitVectorDESC()
{
	sort(m_vecHit.begin(), m_vecHit.end(), [](const Ray_Hit& a, const Ray_Hit& b) {return a._distance > b._distance; });

}
