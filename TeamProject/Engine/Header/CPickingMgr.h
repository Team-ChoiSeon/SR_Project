#pragma once
#include "CBase.h"

BEGIN(Engine)
class CGameObject;
class CScene;
class CLayer;
class ENGINE_DLL CPickingMgr : public CBase
{
	DECLARE_SINGLETON(CPickingMgr)
private:
	explicit CPickingMgr();
	virtual ~CPickingMgr();

public:
	//Basic Function
	void		Ready_Picking(LPDIRECT3DDEVICE9 GrahphicDev, HWND hWnd);
	HRESULT		Update_Picking(const _float& fTimeDelta);
	_int		LateUpdate_Picking(const _float& fTimeDelta);

	//Create, Release Function
	void		Free();

	//Getter, Setter Function
	Ray*				Get_Ray(){ return &m_Ray; }
	vector<Ray_Hit>&	Get_HitTargetList() { SortHitVectorASC();  return m_vecHit; }
	
	void				Add_HitInfo(Ray_Hit& hitinfo) { m_vecHit.push_back(hitinfo); }

	CGameObject* Get_HitNearObject(float _Range);
	CGameObject* Get_PickedObject();
	_vec3 Get_DragVector() { return m_vDragDistance; }
	_vec3 CalcRayPlaneIntersection(const Ray& ray, const _vec3& planePoint, const _vec3& planeNormal);

private:
	//Compute Function
	void ComputeCursor();
	void Make_Ray(float x, float y);
	void TransformRayIntoWorld();
	void SortHitVectorASC();
	void SortHitVectorDESC();

	void Check_RayHitted();

private:
	//Variables
	LPDIRECT3DDEVICE9		m_pGraphicDev;
	HWND					m_hWnd;
	Ray						m_Ray;

	_vec3 m_vLastPt;
	_vec3 m_vPlanePt;
	_vec3 m_vPlaneNorm;
	_vec3 m_vDragDistance;

	Ray_Hit			m_RayHitted;
	Ray_Hit			m_Picked;

	_bool m_bDragging;

	vector<Ray_Hit>			m_vecHit;
	POINT					m_Cursor;

	D3DVIEWPORT9			m_vp;
	_matrix					m_mProj;
};

END