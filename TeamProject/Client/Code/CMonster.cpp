#pragma once
#include "pch.h"
#include "CMonster.h"
#include "CInputMgr.h"
#include "CCollisionMgr.h"
#include "Engine_Model.h"

CMonster::CMonster(LPDIRECT3DDEVICE9 pGraphicDev)
	: CGameObject(pGraphicDev)
{
}
CMonster::~CMonster()
{
}

HRESULT CMonster::Ready_GameObject()
{

	Add_Component<CTransform>(ID_DYNAMIC, m_pGraphicDev);
	m_pTransform = Get_Component<CTransform>();
	m_pTransform->Ready_Component();
	m_pTransform->Set_Pos({ 1.f, -2.f, 0.f });
	m_pTransform->Set_Look({ 0.f, 0.f, 1.f });
	m_pTransform->Set_Up({ 0.f, 1.f, 0.f });
	m_pTransform->Set_Right({ 1.f, 0.f, 0.f });

	//DefaultCubeModel tModel;
	//tModel.meshKey = L"DirtObj.obj";

	Add_Component<CModel>(ID_DYNAMIC, m_pGraphicDev);
	m_pModel = Get_Component<CModel>();

	Add_Component<CRigidBody>(ID_DYNAMIC, m_pGraphicDev, m_pTransform);
	m_pRigid = Get_Component<CRigidBody>();

	Add_Component<CCollider>(ID_DYNAMIC, m_pGraphicDev, m_pRigid);
	m_pCollider = Get_Component<CCollider>();
	m_pCollider->Set_ColTag(ColliderTag::NONE);
	m_pCollider->Set_ColType(ColliderType::ACTIVE);


	Get_Component<CRigidBody>()->Set_OnGround(false);
	Get_Component<CRigidBody>()->Set_UseGravity(true);
	Get_Component<CRigidBody>()->Set_Bounce(0.5f);
	Get_Component<CRigidBody>()->Set_Friction(0.2f);
	// Get_Component<CCollider>()->Set_Offset({ 1.5,1.5,1.5 });
	Get_Component<CCollider>()->Set_BoundType(BoundingType::OBB);



	return S_OK;
}

int CMonster::Update_GameObject(const _float& fTimeDelta)
{
	//KeyInput(fTimeDelta);
	CGameObject::Update_GameObject(fTimeDelta);
	return 0;
}

void CMonster::LateUpdate_GameObject(const _float& fTimeDelta)
{
	CGameObject::LateUpdate_GameObject(fTimeDelta);
}

void CMonster::KeyInput(const _float& fTimeDelta)
{
	// test code
	if (CInputMgr::Get_Instance()->Key_Down(DIK_UP))
	{
		m_pRigid->Add_Torque(_vec3(1.f, 0.f, 0.f) * 50.f); //  ȸ��
		m_pRigid->Add_Force(_vec3(0.f, 0.f, 1.f) * 1.f);
	}

	if (CInputMgr::Get_Instance()->Key_Down(DIK_DOWN))
	{
		m_pRigid->Add_Torque(_vec3(-1.f, 0.f, 0.f) * 50.f); //  ȸ��
		m_pRigid->Add_Force(_vec3(0.f, 0.f, -1.f) * 1.f);
	}

	if (CInputMgr::Get_Instance()->Key_Down(DIK_LEFT))
	{
		m_pRigid->Add_Torque(_vec3(0.f, 0.f, 1.f) * 50.f); // ���� ȸ��
		m_pRigid->Add_Force(_vec3(-1.f, 0.f, 0.f) * 1.f);
	}

	if (CInputMgr::Get_Instance()->Key_Down(DIK_RIGHT))
	{
		m_pRigid->Add_Torque(_vec3(0.f, 0.f, -1.f) * 50.f); // ������ ȸ��
		m_pRigid->Add_Force(_vec3(1.f, 0.f, 0.f) * 1.f);
	}

	if (CInputMgr::Get_Instance()->Key_Down(DIK_R))
	{
		// ������ ��� (Y-up ����)
		_matrix matIdentity;
		D3DXMatrixIdentity(&matIdentity);
		m_pTransform->Set_RotMatrix(&matIdentity);

		m_pRigid->Set_Velocity(_vec3{ 0.f,0.f,0.f });
		// ���ӵ� �� ��ũ �ʱ�ȭ
		m_pRigid->Set_AVelocity(_vec3(0.f, 0.f, 0.f));
		m_pRigid->Set_Torque(_vec3(0.f, 0.f, 0.f));
	}
		
}

CMonster* CMonster::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CMonster* pMonster = new CMonster(pGraphicDev);

	if (FAILED(pMonster->Ready_GameObject()))
	{
		Safe_Release(pMonster);
		MSG_BOX("Monster Create Failed");
		return nullptr;
	}

	return pMonster;
}

void CMonster::Free()
{
	Safe_Release(m_pRigid);
	Safe_Release(m_pModel);
	Safe_Release(m_pCollider);
	Safe_Release(m_pTransform);
}