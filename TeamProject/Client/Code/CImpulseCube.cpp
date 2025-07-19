#include "pch.h"
#include "CImpulseCube.h"
#include "CCollider.h"
#include "CTransform.h"
#include "CRigidBody.h"
#include "CFactory.h"
#include "Engine_GUI.h"
#include "CGuiSystem.h"

CImpulseCube::CImpulseCube(LPDIRECT3DDEVICE9 pGraphicDev)
	: CCube(pGraphicDev)
{
}

CImpulseCube::CImpulseCube(const CImpulseCube& rhs)
	: CCube(rhs.m_pGraphicDev)
{
}

CImpulseCube::~CImpulseCube()
{
}

HRESULT CImpulseCube::Ready_GameObject()
{
	Add_Component<CTransform>(ID_DYNAMIC, m_pGraphicDev);
	m_pTransform = Get_Component<CTransform>();
	m_pTransform->Ready_Component();
	m_pTransform->Set_Scale({ 1.f, 1.f, 1.f });
	m_pTransform->Set_Pos({ 0.f, 0.f, 0.f });
	m_pTransform->Set_Look({ 0.f, 0.f, 1.f });
	m_pTransform->Set_Up({ 0.f, 1.f, 0.f });
	m_pTransform->Set_Right({ 1.f, 0.f, 0.f });

	Add_Component<CModel>(ID_DYNAMIC, m_pGraphicDev);
	m_pModel = Get_Component<CModel>();

	Add_Component<CRigidBody>(ID_DYNAMIC, m_pGraphicDev, m_pTransform);
	m_pRigid = Get_Component<CRigidBody>();
	m_pRigid->Set_OnGround(false);
	m_pRigid->Set_UseGravity(true);
	m_pRigid->Set_Bounce(0.5f);
	m_pRigid->Set_Friction(0.f);
	m_pRigid->Set_Mass(1.f);


	Add_Component<CCollider>(ID_DYNAMIC, m_pGraphicDev, m_pRigid);
	m_pCollider = Get_Component<CCollider>();
	m_pCollider->Set_ColTag(ColliderTag::NONE);
	m_pCollider->Set_ColType(ColliderType::ACTIVE);
	m_pCollider->Set_BoundType(BoundingType::AABB);



	CFactory::Save_Prefab(this, "CImpulseCube");

	return S_OK;
}

_int CImpulseCube::Update_GameObject(const _float& fTimeDelta)
{
	CGameObject::Update_GameObject(fTimeDelta);

	////Deubbing Code
	//CGuiSystem::Get_Instance()->RegisterPanel("velocity", [this]() {
	//	// °£´ÜÇÑ GUI Ã¢ ÇÏ³ª Ãâ·Â
	//	ImGui::SetNextWindowSize(ImVec2{ 200,200 });
	//	if (m_pRigid->Get_OnGround()) {
	//		ImGui::Begin("on Ground");
	//	}
	//	else {
	//		ImGui::Begin("not on Ground");
	//	}
	//	ImGui::End();
	//	});
	return S_OK;
}

void CImpulseCube::LateUpdate_GameObject(const _float& fTimeDelta)
{
	CGameObject::LateUpdate_GameObject(fTimeDelta);
}

CImpulseCube* CImpulseCube::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CImpulseCube* pImpulseCube = new CImpulseCube(pGraphicDev);
	if (FAILED(pImpulseCube->Ready_GameObject()))
	{
		Safe_Release(pImpulseCube);
		MSG_BOX("FCube Create Failed");
		return nullptr;
	}
	return pImpulseCube;
}

void CImpulseCube::Free()
{
	Safe_Release(m_pTransform);
	Safe_Release(m_pModel);
	Safe_Release(m_pCollider);
	Safe_Release(m_pRigid);
}

void CImpulseCube::Set_Info(const _vec3& StartPos)
{
	m_pTransform->Set_Pos(StartPos);
}


REGISTER_GAMEOBJECT(CImpulseCube)