#include "Engine_Define.h"
#include "CPickingMgr.h"
#include "CGameObject.h"
#include "CPickTarget.h"
#include "CScene.h"
#include "CCameraMgr.h"
#include "CInputMgr.h"
#include "CCamera.h"
#include "CTransform.h"

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
	m_pGraphicDev->GetViewport(&m_vp);

	m_hWnd = hWnd;
}

HRESULT CPickingMgr::Update_Picking(const _float& fTimeDelta)
{

	ComputeCursor();
	Make_Ray(m_Cursor.x, m_Cursor.y);			//ray 생성
	TransformRayIntoWorld();					//ray를 월드공간으로 변화
	Check_RayHitted();								//Search Ray
	Check_Input();											// Fix Target
	

	if (!m_vecHit.empty())
		m_vecHit.clear();
	
	return S_OK;
}

_int CPickingMgr::LateUpdate_Picking(const _float& fTimeDelta)
{
	return 0;
}

void CPickingMgr::Free()
{
	Safe_Release(m_pGraphicDev);
}

void CPickingMgr::Check_RayHitted()
{
	//1. Empty RayHitObj
	if (m_vecHit.empty()) {
		m_RayHitted = { nullptr,0 };
		return;
	}

	//2. Check Nearest Object
	auto iter = min_element(m_vecHit.begin(), m_vecHit.end(),
		[](const Ray_Hit& a, const Ray_Hit& b) {return a._distance < b._distance;
		});

	m_RayHitted = *iter;
}

void CPickingMgr::Check_Input()
{
	if (CInputMgr::Get_Instance()->Mouse_Tap(DIM_LB))
	{
		m_Picked = m_RayHitted;

		if (m_Picked._hittedobject)
		{
			CTransform* targetTrans = m_Picked._hittedobject->Get_Component<CTransform>();
			CCamera* pMainCam = CCameraMgr::Get_Instance()->Get_MainCamera()->Get_Component<CCamera>();

			m_vPlanePt = m_Picked._hittedobject->Get_Component<CTransform>()->Get_Pos();
			m_vPlaneNorm = targetTrans->Get_Pos() - pMainCam->Get_Eye();
			D3DXVec3Normalize(&m_vPlaneNorm, &m_vPlaneNorm);

			m_vLastPt = CalcRayPlaneIntersection(m_Ray, m_vPlanePt, m_vPlaneNorm);
			m_bDragging = true;
		}
	}
	if (CInputMgr::Get_Instance()->Mouse_Hold(DIM_LB))
	{
		if (m_bDragging && m_Picked._hittedobject)
		{
			_vec3 nowPt = CalcRayPlaneIntersection(m_Ray, m_vPlanePt, m_vPlaneNorm);
			m_vDragDistance = nowPt - m_vLastPt;
			m_vLastPt = nowPt;
		}
	}
	if (CInputMgr::Get_Instance()->Mouse_Away(DIM_LB))
	{
		m_Picked = { nullptr, 0 };
		m_bDragging = false;
		m_vDragDistance = { 0,0,0 };
	}
}

CGameObject* CPickingMgr::Get_HitNearObject()
{
	// if (m_RayHitted._hittedobject) {
	// 	MessageBoxW(0,L"맞음",L"hit",MB_OK);
	// }

	return m_RayHitted._hittedobject;
}

CGameObject* CPickingMgr::Get_PickedObject(float _Range)
{
	if(!m_Picked._hittedobject)
		return nullptr;
	else {
		if (m_Picked._distance > _Range) {
			return nullptr;
		}
		else {
			return m_Picked._hittedobject;
		}
	}
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

//집은 물체 기준, 카메라와 물체의 벡터와 수직인 평면을 생성. 그 평면 기준 이동 거리를 측정
_vec3 CPickingMgr::CalcRayPlaneIntersection(const Ray& ray, const _vec3& planePoint, const _vec3& planeNormal)
{
	float denom = D3DXVec3Dot(&planeNormal, &ray._direction); //평면 법선하고 레이 방향 내적
	if (fabs(denom) < 1e-6f) return _vec3(0, 0, 0); // 각도 0 -> 평행

	_vec3 diff = planePoint - ray._position; // 거리 계산
	float t = D3DXVec3Dot(&diff, &planeNormal) / denom; //거리 닷 법선  / 각도- > 얼마나 남았나
	//방향 벡터 * t는 실제 거리
	return ray._position + ray._direction * t;
}

void CPickingMgr::SortHitVectorASC()
{
	sort(m_vecHit.begin(), m_vecHit.end(), [](const Ray_Hit& a, const Ray_Hit& b) {return a._distance < b._distance; });
}

void CPickingMgr::SortHitVectorDESC()
{
	sort(m_vecHit.begin(), m_vecHit.end(), [](const Ray_Hit& a, const Ray_Hit& b) {return a._distance > b._distance; });
}

