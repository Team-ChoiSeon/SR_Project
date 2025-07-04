#pragma once
#include "CBase.h"
#include "Engine_Define.h"
#include "Engine_Struct.h"

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
	HRESULT		Update_Picking(const float& fTimeDelta);
	_int		LateUpdate_Picking(const float& fTimeDelta);
	void		Check_CollisionWorld();

	//Create, Release Function
	void		Free();

	//Getter, Setter Function
	Ray					Get_Ray(){ return m_Ray; }
	CGameObject*		Get_HitTarget();
	vector<Ray_Hit>*	Get_HitTargetList() { SortHitVectorASC();  return &m_vecHit; }

private:
	//Compute Function
	void ComputeCursor();
	void Make_Ray(float x, float y);
	void TransformRayIntoWorld();
	void SortHitVectorASC();
	void SortHitVectorDESC();

	//Variables
	LPDIRECT3DDEVICE9		m_pGraphicDev;
	HWND					m_hWnd;
	POINT					m_pCursor;
	Ray						m_Ray;
	vector<Ray_Hit>			m_vecHit;

	D3DVIEWPORT9			m_vp;
	_matrix					m_mProj;



};

END