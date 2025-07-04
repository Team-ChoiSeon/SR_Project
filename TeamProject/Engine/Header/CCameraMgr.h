#pragma once
#include "CBase.h"
#include "Engine_Define.h"

BEGIN(Engine)
class CGameObject;
class  ENGINE_DLL CCameraMgr : public CBase
{
	DECLARE_SINGLETON(CCameraMgr)
private:
	explicit	CCameraMgr();
	virtual		~CCameraMgr();

public:
	//Basic Function
	HRESULT				Set_MainCamera(CGameObject* pCamera);
	_int				Update_Camera(LPDIRECT3DDEVICE9 pGraphicDev, const _float& fTimeDelta);
	void				LateUpdate_Camera(const _float& fTimeDelta);
	void				Render_Camera();


	//Create, Release Fucntion
	void				Free();


	//Optional Function
	const _matrix* Get_CamViewMatrix(CGameObject* pCamera);
	const _matrix* Get_CamProjectionMatrix(CGameObject* pCamera);


	//Getter,Setter Function
	CGameObject*		Get_MainCamera() {return m_pMainCam;}
	const _matrix*		Get_MainViewMatrix();
	const _matrix*		Get_MainProjectionMatrix();

	//template

private:
	//Variables
	CGameObject*							m_pMainCam;
	unordered_map<wstring, CGameObject*>	m_umCam;
	
	_matrix		m_matView;
	_matrix		m_matProjection;
};


//template <typename T>
//T* CCameraMgr::Get_Camera(const wstring& wCamTag)
//{
//	for (auto& iter : m_umCam)
//	{
//		if (iter.first == wCamTag)
//			return dynamic_cast<T*>(iter);
//	}
//
//	MSG_BOX(L"[CameraMgr] Get_Camera fail : no such tag");
//	return T*;
//}


END