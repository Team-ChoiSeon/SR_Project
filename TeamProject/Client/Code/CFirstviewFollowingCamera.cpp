#pragma once
#include "pch.h"
#include "CFirstviewFollowingCamera.h"
#include "CCameraObject.h"
#include "CTransform.h"
#include "CCamera.h"
#include "CInputMgr.h"

CFirstviewFollowingCamera::CFirstviewFollowingCamera(LPDIRECT3DDEVICE9 pGraphicDev, CGameObject* target)
	: CCameraObject(pGraphicDev), m_pFollowingTarget(target)
{
	m_pTransform = Get_Component<CTransform>();
	m_pCamera = Get_Component<CCamera>();
	auto target_transform = m_pFollowingTarget->Get_Component<CTransform>();

	m_pTransform->Set_Pos(target_transform->Get_Pos());
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
	: CCameraObject(rhs), m_pFollowingTarget(rhs.m_pFollowingTarget)
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
	//if (CInputMgr::Get_Instance()->Key_Tap(DIK_TAB))
	//{
	//	if (m_bCursorMove)
	//		m_bCursorMove = false;
	//	else
	//		m_bCursorMove = true;
	//}

	//if (!m_bCursorMove)
	//	CursorRotate();

	//Get and Change Target Angle, Set transform same as target
	auto target_transform = m_pFollowingTarget->Get_Component<CTransform>();
	m_pTransform->Set_Pos(target_transform->Get_Pos());
	m_pTransform->Set_Angle(target_transform->Get_Angle());
	m_pTransform->Set_Look(target_transform->Get_Info(INFO_LOOK));
	AngleClamping();

	//Set camera com 
	m_pCamera->Set_Eye(m_pTransform->Get_Pos()); 
	m_pCamera->Set_Up(m_pTransform->Get_Info(INFO_UP));
	m_pCamera->Set_Look(m_pTransform->Get_Info(INFO_LOOK));
	m_pCamera->Set_Right(m_pTransform->Get_Info(INFO_RIGHT));
	m_pCamera->Set_At(m_pTransform->Get_Pos() + m_pTransform->Get_Info(INFO_LOOK));

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
	const _matrix* world = m_pTransform->Get_WorldMatrix();
	m_pGraphicDev->SetTransform(D3DTS_WORLD, world);
}

CFirstviewFollowingCamera* CFirstviewFollowingCamera::Create(LPDIRECT3DDEVICE9 pGraphicDev, CGameObject* target)
{
	auto pCam = new CFirstviewFollowingCamera(pGraphicDev, target);
	if (FAILED(pCam->Ready_GameObject())) {
		Safe_Release(pCam);
		return nullptr;
	}
	return pCam;
}
void CFirstviewFollowingCamera::Free()
{
	Safe_Release(m_pFollowingTarget);
	Safe_Release(m_pCamera);
	Safe_Release(m_pTransform);
	Safe_Release(m_pGraphicDev);
}

void CFirstviewFollowingCamera::CursorRotate()
{
	//커서 고정
	ShowCursor(false);
	float cx = WINCX / 2.f;
	float cy = WINCY / 2.f;
	POINT cursor = { cx, cy };
	ClientToScreen(g_hWnd, &cursor);
	SetCursorPos(cursor.x, cursor.y);

	//화면 회전
	float dx = CInputMgr::Get_Instance()->Get_DIMouseMove(MOUSEMOVESTATE::DIMS_X);
	float dy = CInputMgr::Get_Instance()->Get_DIMouseMove(MOUSEMOVESTATE::DIMD_Y);

	float sensitivity = 300.f;
	float rx = dx / sensitivity;
	float ry = dy / sensitivity;

	auto target_transform = m_pFollowingTarget->Get_Component<CTransform>();
	target_transform->Set_Angle(target_transform->Get_Angle() + _vec3{ ry, rx, 0.f });

}

void CFirstviewFollowingCamera::AngleClamping()
{
	float pi = D3DX_PI;
	auto target_transform = m_pFollowingTarget->Get_Component<CTransform>();
	float fxlowGap = (-pi * 0.5f + 0.1f) - target_transform->Get_Angle().x;
	float fxhighGap = target_transform->Get_Angle().x - (pi * 0.5f - 0.1f);

	if (fxlowGap > 0.f) {
		target_transform->Set_Angle(target_transform->Get_Angle() + _vec3{ fxlowGap, 0.f, 0.f});
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
