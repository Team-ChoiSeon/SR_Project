#pragma once
#include "pch.h"
#include "CFirstviewFollowingCamera.h"
#include "CCameraObject.h"
#include "CTransform.h"
#include "CCamera.h"
#include "CInputMgr.h"

CFirstviewFollowingCamera::CFirstviewFollowingCamera(LPDIRECT3DDEVICE9 pGraphicDev)
	: CCameraObject(pGraphicDev)
{
	m_pTransform = Get_Component<CTransform>();
	m_pCamera = Get_Component<CCamera>();

	m_pTransform->Set_Pos({0.f, 0.f, 0.f});
	m_pTransform->Set_Angle({0.f, 0.f, 0.f});
	m_pTransform->Set_Scale({1.f, 1.f, 1.f});
	m_pTransform->Set_Look({0.f, 0.f, 1.f});
	m_pTransform->Set_Up({ 0.f, 1.f, 0.f });
	m_pTransform->Set_Right({ 1.f, 0.f, 0.f });

	m_pCamera->Set_Eye(m_pTransform->Get_Pos());
	m_pCamera->Set_At(m_pTransform->Get_Pos() + m_pTransform->Get_Info(INFO_LOOK));
	m_pCamera->Set_Up(m_pTransform->Get_Info(INFO_UP));
	m_pCamera->Set_Look(m_pTransform->Get_Info(INFO_LOOK));
	m_pCamera->Set_Right(m_pTransform->Get_Info(INFO_RIGHT));
	m_pCamera->Set_Fov(D3DX_PI * 0.25f);
	m_pCamera->Set_Aspect(WINCX / (WINCY * 1.f));
	m_pCamera->Set_Near(0.1f);
	m_pCamera->Set_Far(1000.f);
}

CFirstviewFollowingCamera::CFirstviewFollowingCamera(const CFirstviewFollowingCamera& rhs)
	: CCameraObject(rhs)
{
}

CFirstviewFollowingCamera::~CFirstviewFollowingCamera()
{
}

HRESULT CFirstviewFollowingCamera::Ready_GameObject()
{

	return S_OK;
}

int CFirstviewFollowingCamera::Update_GameObject(const _float& fTimeDelta)
{
	//Get and Change Target Angle, Set transform same as target
	m_pTransform->Set_Pos(m_pTargetTransform->Get_Pos());
	m_pTransform->Set_Angle(m_pTargetTransform->Get_Angle());
	m_pTransform->Set_Look(m_pTargetTransform->Get_Info(INFO_LOOK));

	m_pCamera->AngleClamping();


	for (auto& pComponent : m_umComponent[ID_DYNAMIC])
		pComponent.second->Update_Component(fTimeDelta);


	return 0;
}

void CFirstviewFollowingCamera::LateUpdate_GameObject(const _float& fTimeDelta)
{
	for (auto& pComponent : m_umComponent[ID_DYNAMIC])
		pComponent.second->LateUpdate_Component();
}

void CFirstviewFollowingCamera::Render_GameObject()
{
}

CFirstviewFollowingCamera* CFirstviewFollowingCamera::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	auto pCam = new CFirstviewFollowingCamera(pGraphicDev);
	if (FAILED(pCam->Ready_GameObject())) {
		Safe_Release(pCam);
		return nullptr;
	}
	return pCam;
}
void CFirstviewFollowingCamera::Free()
{
	Safe_Release(m_pCamera);
	Safe_Release(m_pTransform);
	Safe_Release(m_pGraphicDev);
}

