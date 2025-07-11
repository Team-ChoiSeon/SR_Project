#pragma once
#include "pch.h"
#include "CSceneGate.h"
#include "Engine_Model.h"
#include "Engine_Macro.h"
#include "CGameObject.h"

#include "CTransform.h"
#include "CCollider.h"
#include "CModel.h"
#include "CRigidBody.h"

#include "CMainPlayer.h"
#include "SceneHW.h"

#include "CCollisionMgr.h"
#include "CInputMgr.h"
#include "CSceneMgr.h"

#include "CFactory.h"

CSceneGate::CSceneGate(LPDIRECT3DDEVICE9 pGraphicDev)
	: CGameObject(pGraphicDev)
{
}

CSceneGate::~CSceneGate()
{
}

HRESULT CSceneGate::Ready_GameObject()
{
	Add_Component<CModel>(ID_DYNAMIC, m_pGraphicDev);
	m_pModel = Get_Component<CModel>();

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
	m_pTransform->Set_Scale({ 2.f, 2.f, 2.f });

	return S_OK;
}

int CSceneGate::Update_GameObject(const _float& fTimeDelta)
{
	for (auto& pComponent : m_umComponent[ID_DYNAMIC])
		pComponent.second->Update_Component(fTimeDelta);

	return 0;
}

void CSceneGate::LateUpdate_GameObject(const _float& fTimeDelta)
{
	for (auto& pComponent : m_umComponent[ID_DYNAMIC])
		pComponent.second->LateUpdate_Component();

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

	}


}



CSceneGate* CSceneGate::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CSceneGate* pSceneGate = new CSceneGate(pGraphicDev);

	if (FAILED(pSceneGate->Ready_GameObject()))
	{
		Safe_Release(pSceneGate);
		MSG_BOX("Dummy Create Failed");
		return nullptr;
	}

	return pSceneGate;
}

void CSceneGate::Free()
{
	Safe_Release(m_pModel);
	Safe_Release(m_pCollider);
	Safe_Release(m_pTransform);
	Safe_Release(m_pRigid);
}

// REGISTER_GAMEOBJECT(CSceneGate)