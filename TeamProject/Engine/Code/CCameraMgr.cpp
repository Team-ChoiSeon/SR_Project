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

_int CCameraMgr::Update_Camera(LPDIRECT3DDEVICE9 pGraphicDev,  const _float& fTimeDelta)
{
	if (m_pMainCam == nullptr)
		return -1;

	m_matView = *m_pMainCam->Get_Component<CCamera>()->Get_ViewMatrix();
	m_matProjection = *m_pMainCam->Get_Component<CCamera>()->Get_ProjectionMatrix();

	pGraphicDev->SetTransform(D3DTS_VIEW, &m_matView);
	pGraphicDev->SetTransform(D3DTS_PROJECTION, &m_matProjection);
	return 0;
}

void CCameraMgr::LateUpdate_Camera(const _float& fTimeDelta)
{
}

void CCameraMgr::Render_Camera()
{
}

void CCameraMgr::Free()
{
	Safe_Release(m_pMainCam);
}

//void CCameraMgr::Add_Camera(const wstring& wCamTag, CGameObject* pCamera)
//{
//	if (pCamera == nullptr)
//	{
//		MSG_BOX("[CameraMgr] Add_Camera : nullptr");
//		return;
//	}
//
//	for (auto& iter : m_umCam)
//	{
//		auto TempCam = find(m_umCam.begin(), m_umCam.end(), iter.first == wCamTag);
//		if (TempCam == m_umCam.end())
//		{
//			MSG_BOX("[CameraMgr] Add_Camera : Has that tag");
//			return;
//		}
//	}
//
//	m_umCam.insert(make_pair(wCamTag, pCamera));
//	return;
//}
//
//CGameObject* CCameraMgr::Get_Camera(const wstring& wCamTag)
//{
//	for (auto& iter : m_umCam)
//	{
//		auto tempcam = find_if(m_umCam.begin(), m_umCam.end(), iter.first == wCamTag);
//		if (tempcam == m_umCam.end())
//		{
//			MSG_BOX("[CameraMgr] Get_Camera : None camera with that tag");
//			return nullptr;
//		}
//		else
//			return dynamic_cast<CGameObject*>(tempcam->second);
//	}
//}

const _matrix* CCameraMgr::Get_CamViewMatrix(CGameObject* pCamera)
{
	return pCamera->Get_Component<CCamera>()->Get_ViewMatrix();
}

const _matrix* CCameraMgr::Get_CamProjectionMatrix(CGameObject* pCamera)
{
	return pCamera->Get_Component<CCamera>()->Get_ProjectionMatrix();
}

const _matrix* CCameraMgr::Get_MainViewMatrix()
{
	return m_pMainCam->Get_Component<CCamera>()->Get_ViewMatrix();
}

const _matrix* CCameraMgr::Get_MainProjectionMatrix()
{
	return m_pMainCam->Get_Component<CCamera>()->Get_ProjectionMatrix();
}

