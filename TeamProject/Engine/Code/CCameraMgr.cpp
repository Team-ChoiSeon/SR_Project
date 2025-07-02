#include "CCameraMgr.h"
#include "CGameObject.h"
#include "CCamera.h"

IMPLEMENT_SINGLETON(CCameraMgr)

CCameraMgr::CCameraMgr()
{
}

CCameraMgr::~CCameraMgr()
{
	Free();
}

HRESULT CCameraMgr::Set_MainCamera(CGameObject* pCamera)
{
	if (pCamera == nullptr)
		return E_FAIL;

	m_pMainCam = pCamera;
	return S_OK;
}

_int CCameraMgr::Update_Camera(const _float& fTimeDelta)
{
	if (m_pMainCam == nullptr)
		return -1;
	return	m_pMainCam->Update_GameObject(fTimeDelta);
}

void CCameraMgr::LateUpdate_Camera(const _float& fTimeDelta)
{
	m_pMainCam->LateUpdate_GameObject(fTimeDelta);
}

void CCameraMgr::Render_Camera()
{
	m_pMainCam->Render_GameObject();
}

void CCameraMgr::Free()
{
	Safe_Release(m_pMainCam);
}

const _matrix* CCameraMgr::Get_MainViewMatrix()
{
	return m_pMainCam->Get_Component<CCamera>()->Get_ViewMatrix();
}

const _matrix* CCameraMgr::Get_MainProjectionMatrix()
{
	return m_pMainCam->Get_Component<CCamera>()->Get_ProjectionMatrix();
}

