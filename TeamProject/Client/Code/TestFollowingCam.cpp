#pragma once
#include "pch.h"
#include "TestFollowingCam.h"
#include "CCameraObject.h"
#include "CTransform.h"
#include "CCamera.h"

TestFollowingCam::TestFollowingCam(LPDIRECT3DDEVICE9 pGraphicDev, CGameObject* target, _vec3 distance)
	: CCameraObject(pGraphicDev), m_pFollowingTarget(target), m_vTargetDistance(distance)
{
	auto transform = Get_Component<CTransform>(L"Transform");
	auto cam = Get_Component<CCamera>(L"Camera");

	transform->Set_Pos({ 0.f, 0.f, -1.f });
	transform->Set_Angle({ 0.f, 0.f, 0.f });
	transform->Set_Scale({ 1.f, 1.f, 1.f });

	cam->Set_Eye(transform->Get_Pos());
	cam->Set_At({ 0.f, 0.f, 0.f });
	cam->Set_Up({ 0.f, 1.f, 0.f });
	cam->Set_Fov(D3DX_PI * 0.5f);
	cam->Set_Aspect(WINCX / WINCY * 1.f);
	cam->Set_Near(0.1f);
	cam->Set_Far(1000.f);
}

TestFollowingCam::TestFollowingCam(LPDIRECT3DDEVICE9 pGraphicDev)
	: CCameraObject(pGraphicDev), m_pFollowingTarget(nullptr), m_vTargetDistance({ 0.f, 0.f, 0.f })
{
	auto transform = Get_Component<CTransform>(L"Transform");
	auto cam = Get_Component<CCamera>(L"Camera");

	transform->Set_Pos({ 0.f, 30.f, 0.f });
	transform->Set_Angle({ 0.f, 0.f, 0.f });
	transform->Set_Scale({ 1.f, 1.f, 1.f });

	cam->Set_Eye(transform->Get_Pos());
	cam->Set_At({ 0.f, 0.f, 0.f });
	cam->Set_Up({ 0.f, 0.f, 1.f });
	cam->Set_Fov(D3DX_PI * 0.5f);
	cam->Set_Aspect(WINCX / WINCY * 1.f);
	cam->Set_Near(0.1f);
	cam->Set_Far(1000.f);
}

TestFollowingCam::TestFollowingCam(const TestFollowingCam& rhs)
	: CCameraObject(rhs), m_pFollowingTarget(rhs.m_pFollowingTarget), m_vTargetDistance(rhs.m_vTargetDistance)
{
}

TestFollowingCam::~TestFollowingCam()
{
}

TestFollowingCam* TestFollowingCam::Clone()
{
	return new TestFollowingCam(*this);
}

TestFollowingCam* TestFollowingCam::Create(LPDIRECT3DDEVICE9 pGraphicDev, CGameObject* target, _vec3 distance)
{
	auto pCam = new TestFollowingCam(pGraphicDev, target, distance);
	if (FAILED(pCam->Ready_Fcam())) {
		Safe_Release(pCam);
		return nullptr;
	}
	return pCam;
}
TestFollowingCam* TestFollowingCam::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	auto pCam = new TestFollowingCam(pGraphicDev);
	if (FAILED(pCam->Ready_Fcam())) {
		Safe_Release(pCam);
		return nullptr;
	}
	return pCam;
}


HRESULT TestFollowingCam::Ready_Fcam()
{
	return S_OK;
}

void TestFollowingCam::Update_FCam(const _float& fTimeDelta)
{
	auto cam = Get_Component<CCamera>(L"Camera");
	auto transform = Get_Component<CTransform>(L"Transform");

	if (m_pFollowingTarget != nullptr)
	{
		auto tar_transform = m_pFollowingTarget->Get_Component<CTransform>(L"Transform");
		transform->Set_Pos(tar_transform->Get_Pos() + m_vTargetDistance);
		cam->Set_At(tar_transform->Get_Pos());
	}
	cam->Set_Eye(transform->Get_Pos());
	cam->Update_Camera(fTimeDelta);

}

void TestFollowingCam::LateUpdate_FCam()
{
}

void TestFollowingCam::Render_FCam()
{
	auto transform = Get_Component<CTransform>(L"Transform");
	const _matrix* world = transform->Get_WorldMatrix();
	m_pGraphicDev->SetTransform(D3DTS_WORLD, world);
}

void TestFollowingCam::ComputeCamera()
{
}

