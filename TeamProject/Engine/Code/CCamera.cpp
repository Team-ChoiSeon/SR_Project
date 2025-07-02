#include "CCamera.h"

CCamera::CCamera()
{
	D3DXMatrixIdentity(&m_mView);
	D3DXMatrixIdentity(&m_mProj);
}

CCamera::CCamera(LPDIRECT3DDEVICE9 pGraphicDev) : CComponent(pGraphicDev)
{
	D3DXMatrixIdentity(&m_mView);
	D3DXMatrixIdentity(&m_mProj);
}

CCamera::CCamera(const CCamera& rhs) : CComponent(rhs.m_pGraphicDev)
{
	D3DXMatrixIdentity(&m_mView);
	D3DXMatrixIdentity(&m_mProj);
}

CCamera::~CCamera()
{
}

HRESULT CCamera::Ready_Component()
{
    return S_OK;
}

void CCamera::Update_Component(const _float& fTimeDelta)
{
	ComputeViewMatrix();
	ComputeProjMatrix();
}

void CCamera::LateUpdate_Component()
{
}

void CCamera::ComputeLookRightUpVectors()
{
	m_vLook = m_vAt - m_vEye;
	D3DXVec3Cross(&m_vRight, &m_vUp, &m_vLook);
	D3DXVec3Cross(&m_vUp, &m_vLook, &m_vRight);

	D3DXVec3Normalize(&m_vLook, &m_vLook);
	D3DXVec3Normalize(&m_vRight, &m_vRight);
	D3DXVec3Normalize(&m_vUp, &m_vUp);
}

void CCamera::ComputeViewMatrix()
{
	D3DXMatrixLookAtLH(&m_mView, &m_vEye, &m_vAt, &m_vUp);
}

void CCamera::ComputeProjMatrix()
{
	D3DXMatrixPerspectiveFovLH(&m_mProj, m_fFov, m_fAspect, m_fNear, m_fFar);
}

CCamera* CCamera::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CCamera* pCamera = new CCamera(pGraphicDev);

	if (FAILED(pCamera->Ready_Component()))
	{
		Safe_Release(pCamera);
		MSG_BOX("Camera Component Create Failed");
		return nullptr;
	}

	return pCamera;
}

void CCamera::Free()
{
	Safe_Release(m_pGraphicDev);
}
