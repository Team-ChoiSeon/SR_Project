#include "CCamera.h"
#include "CGameObject.h"
#include "CTransform.h"
CCamera::CCamera()
	: m_pTarget(nullptr)
{
	D3DXMatrixIdentity(&m_mView);
	D3DXMatrixIdentity(&m_mProj);
}

CCamera::CCamera(LPDIRECT3DDEVICE9 pGraphicDev) 
	: CComponent(pGraphicDev), m_pTarget(nullptr)
{
	D3DXMatrixIdentity(&m_mView);
	D3DXMatrixIdentity(&m_mProj);
}

CCamera::CCamera(const CCamera& rhs) 
	: CComponent(rhs.m_pGraphicDev), m_pTarget(nullptr)
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
	Set_LRUbyTransform();
	ComputeViewMatrix();
	ComputeProjMatrix();
}

void CCamera::LateUpdate_Component(const _float& fTimeDelta)
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

void CCamera::Set_LRUbyTransform()
{
	auto transform = m_pOwner->Get_Component<CTransform>();
	
	Set_Eye(transform->Get_Pos());
	Set_Up(transform->Get_Info(INFO_UP));
	Set_Look(transform->Get_Info(INFO_LOOK));
	Set_Right(transform->Get_Info(INFO_RIGHT));
	Set_At(transform->Get_Pos() + transform->Get_Info(INFO_LOOK));
}

void CCamera::ComputeViewMatrix()
{
	D3DXMatrixLookAtLH(&m_mView, &m_vEye, &m_vAt, &m_vUp);
}

void CCamera::ComputeProjMatrix()
{
	D3DXMatrixPerspectiveFovLH(&m_mProj, m_fFov, m_fAspect, m_fNear, m_fFar);
}

void CCamera::AngleClamping()
{
	if (m_pTarget != nullptr) {
		float pi = D3DX_PI;
		auto transform = m_pOwner->Get_Component<CTransform>();
		auto target_transform = m_pTarget->Get_Component<CTransform>();
		float fxlowGap = (-pi * 0.5f + 0.1f) - target_transform->Get_Angle().x;
		float fxhighGap = target_transform->Get_Angle().x - (pi * 0.5f - 0.1f);

		if (fxlowGap > 0.f) {
			target_transform->Set_Angle(target_transform->Get_Angle() + _vec3{ fxlowGap, 0.f, 0.f });
		}
		else if (fxhighGap > 0.f) {
			target_transform->Set_Angle(target_transform->Get_Angle() - _vec3{ fxhighGap, 0.f, 0.f });
		}

		if (target_transform->Get_Angle().y > pi * 2.f) {
			target_transform->Set_Angle(target_transform->Get_Angle() - _vec3{ 0.f, pi * 2.f, 0.f });
		}
		else if (target_transform->Get_Angle().y < 0.f) {
			target_transform->Set_Angle(target_transform->Get_Angle() + _vec3{ 0.f, pi * 2.f, 0.f });
		}
	}
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
