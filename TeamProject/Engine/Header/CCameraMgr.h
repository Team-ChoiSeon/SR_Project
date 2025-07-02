#pragma once
#include "CBase.h"
#include "Engine_Define.h"

BEGIN(Engine)
class CGameObject;
class  ENGINE_DLL CCameraMgr
{
	DECLARE_SINGLETON(CCameraMgr)
private:
	explicit	CCameraMgr();
	virtual		~CCameraMgr();

public:
	//Basci Function
	HRESULT				Set_MainCamera(CGameObject* pCamera);
	_int				Update_Camera(const _float& fTimeDelta);
	void				LateUpdate_Camera(const _float& fTimeDelta);
	void				Render_Camera();

	//Create, Release Fucntion
	void				Free();

	//Optional Function
	const _matrix*			Get_MainViewMatrix();
	const _matrix*			Get_MainProjectionMatrix();

	//Getter,Setter Function
	CGameObject*		Get_MainCamera() {return m_pMainCam;}

private:
	//Variables
	CGameObject*		m_pMainCam;
};

END