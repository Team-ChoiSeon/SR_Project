#pragma once
#include "CBase.h"
#include "Engine_Define.h"
#include "Engine_Struct.h"

BEGIN(Engine)
class CGameObject;
class CScene;
class CLayer;
class ENGINE_DLL PickingMgr : public CBase
{
	DECLARE_SINGLETON(PickingMgr)
private:
	explicit PickingMgr();
	virtual ~PickingMgr();

public:
	//Basic Function
	void		Ready_Picking(LPDIRECT3DDEVICE9 GrahphicDev);
	HRESULT		Update_Picking(const float& fTimeDelta);
	_int		LateUpdate_Picking(const float& fTimeDelta);
	void		Check_CollisionWorld();

	//Create, Release Function
	void		Free();

	//Getter, Setter Function
	Ray*				Get_Ray(){ return m_pRay; }
	CGameObject*		Get_HitTarget();
	vector<Ray_Hit*>*	Get_HitTargetList() { SortHitVectorASC();  return &m_vecHit; }

	void Set_CursorPos	(POINT* cursor) { m_pCursor = cursor; }

private:
	//Compute Function
	void Make_Ray(float x, float y);
	void TransformRayIntoWorld();
	void SortHitVectorASC();
	void SortHitVectorDESC();

	//Variables
	LPDIRECT3DDEVICE9		m_pGraphicDev;
	POINT*					m_pCursor;
	Ray*					m_pRay;
	vector<Ray_Hit*>		m_vecHit;

	D3DVIEWPORT9			m_vp;
	_matrix					m_mProj;



};

END