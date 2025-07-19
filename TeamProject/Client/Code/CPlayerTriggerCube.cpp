#pragma once
#include "pch.h"
#include "CPlayerTriggerCube.h"
#include "Engine_Model.h"
#include "Engine_Macro.h"
#include "CGameObject.h"

#include "CTransform.h"
#include "CCollider.h"
#include "CModel.h"
#include "CRigidBody.h"

#include "CMainPlayer.h"
#include "SceneHW.h"
#include "DummyCube.h"
#include "CTestTile.h"

#include "CCollisionMgr.h"
#include "CInputMgr.h"
#include "CSceneMgr.h"

#include "CFactory.h"

CPlayerTriggerCube::CPlayerTriggerCube(LPDIRECT3DDEVICE9 pGraphicDev)
	: CGameObject(pGraphicDev)
{
}

CPlayerTriggerCube::~CPlayerTriggerCube()
{
	Free();
}

HRESULT CPlayerTriggerCube::Ready_GameObject()
{

	Add_Component<CTransform>(ID_DYNAMIC, m_pGraphicDev);
	m_pTransform = Get_Component<CTransform>();

	Add_Component<CRigidBody>(ID_DYNAMIC, m_pGraphicDev, m_pTransform);
	m_pRigid = Get_Component<CRigidBody>();
	m_pRigid->Set_UseGravity(false);
	m_pRigid->Set_OnGround(true);

	Add_Component<CCollider>(ID_DYNAMIC, m_pGraphicDev, m_pRigid);
	m_pCollider = Get_Component<CCollider>();
	m_pCollider->Set_ColType(ColliderType::TRIGGER);

	m_pTransform->Set_Pos({ 0.f, -7.f, -20.f });
	//m_pTransform->Set_Scale({ 2.f, 2.f, 2.f });
	m_pTransform->Set_Scale({ 4.f, 2.f, 5.f });

	return S_OK;
}

int CPlayerTriggerCube::Update_GameObject(const _float& fTimeDelta)
{
	CGameObject::Update_GameObject(fTimeDelta);

	return 0;
}

void CPlayerTriggerCube::LateUpdate_GameObject(const _float& fTimeDelta)
{
	CGameObject::LateUpdate_GameObject(fTimeDelta);

	m_bInGate = false;
	CCollider* pOtherCol = m_pCollider->Get_Other();

	if (pOtherCol) {
		CGameObject* pOtherObj = pOtherCol->m_pOwner;
		if (pOtherObj != nullptr)
		{
			if (dynamic_cast<CMainPlayer*>(pOtherObj))
			{
				m_bInGate = true;
			}
		}
		if (pOtherCol && dynamic_cast<CMainPlayer*>(pOtherCol->m_pOwner))
			m_bInGate = true;
	}

}

CPlayerTriggerCube* CPlayerTriggerCube::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CPlayerTriggerCube* pPlayerTriggerCube = new CPlayerTriggerCube(pGraphicDev);

	if (FAILED(pPlayerTriggerCube->Ready_GameObject()))
	{
		Safe_Release(pPlayerTriggerCube);
		MSG_BOX("Dummy Create Failed");
		return nullptr;
	}

	return pPlayerTriggerCube;
}

void CPlayerTriggerCube::Free()
{
	Safe_Release(m_pCollider);
	Safe_Release(m_pTransform);
	Safe_Release(m_pRigid);

	CGameObject::Free();
}

// REGISTER_GAMEOBJECT(CSceneGate)