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
#include "DummyCube.h"
#include "CTestTile.h"

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
	Free();
}

HRESULT CSceneGate::Ready_GameObject()
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
	m_pTransform->Set_Scale({ 4.f, 4.f, 5.f });

	pGateDoorL = Set_GateDoor(true);
	pGateDoorR = Set_GateDoor(false);

	const _vec3 vGatePos = m_pTransform->Get_Pos();
	const _vec3 vGateScale = m_pTransform->Get_Scale();
	const _vec3 vPillarScale = { 1.f, 6.f, 1.f };

	pGatePillarL = Set_GateStructure(vGatePos + _vec3(-5.f, -2.f, -vGateScale.z - 1.f), vPillarScale);
	pGatePillarR = Set_GateStructure(vGatePos + _vec3(5.f, -2.f, -vGateScale.z - 1.f), vPillarScale);
	const _vec3 vTopScale = { 6.f, 1.f, 2.f };
	pGateTop = Set_GateStructure(vGatePos + _vec3(0.f, 5.f, -vGateScale.z -1.f), vTopScale);


	pTriggerCube = CPlayerTriggerCube::Create(m_pGraphicDev);
	pTriggerCube->Get_Component<CTransform>()->Set_Pos(vGatePos + _vec3(0.f, 0.f, -vGateScale.z - 2.f));
	pTriggerCube->Get_Component<CTransform>()->Set_Scale({ 6.f, 2.f, 1.f });
	pTriggerCube->Get_Component<CRigidBody>()->Set_UseGravity(false);


	m_vGameObjects.push_back(pGateDoorL);
	m_vGameObjects.push_back(pGateDoorR);
	m_vGameObjects.push_back(pGatePillarL);
	m_vGameObjects.push_back(pGatePillarR);
	m_vGameObjects.push_back(pGateTop);
	m_vGameObjects.push_back(pTriggerCube);


	CFactory::Save_Prefab(this, "CSceneGate");

	return S_OK;
}

int CSceneGate::Update_GameObject(const _float& fTimeDelta)
{
	CGameObject::Update_GameObject(fTimeDelta);

	pGateDoorL->Update_GameObject(fTimeDelta);
	pGateDoorR->Update_GameObject(fTimeDelta);
	pGatePillarL->Update_GameObject(fTimeDelta);
	pGatePillarR->Update_GameObject(fTimeDelta);
	pGateTop->Update_GameObject(fTimeDelta);
	pTriggerCube->Update_GameObject(fTimeDelta);

	return 0;
}

void CSceneGate::LateUpdate_GameObject(const _float& fTimeDelta)
{
	CGameObject::LateUpdate_GameObject(fTimeDelta);
	
	m_bInGate = pTriggerCube->Get_InGate();

	m_bInOpendoor = false;
	CCollider* pOtherCol = m_pCollider->Get_Other();

	if (pOtherCol) {
		CGameObject* pOtherObj = pOtherCol->m_pOwner;
		if (pOtherObj != nullptr)
		{
			if (dynamic_cast<CMainPlayer*>(pOtherObj))
			{
				m_bInOpendoor = true;
			}
		}
		if (pOtherCol && dynamic_cast<CMainPlayer*>(pOtherCol->m_pOwner))
			m_bInOpendoor = true;

	}
	const _float fTargetAngle = D3DXToRadian(135.f);
	const _float fSpeed = D3DXToRadian(45.f) * fTimeDelta;

	if (m_bInOpendoor)
		m_fDoorAngle = min(m_fDoorAngle + fSpeed, fTargetAngle);
	else
		m_fDoorAngle = max(m_fDoorAngle - fSpeed, 0.f);

	const _vec3 vGatePos = m_pTransform->Get_Pos();
	const _vec3 vGateScale = m_pTransform->Get_Scale();

	CTransform* pTransL = pGateDoorL->Get_Component<CTransform>();
	const _vec3 vHingeL = vGatePos + _vec3(-4.0f, 0.f, -vGateScale.z);
	const _vec3 vLocalOffsetL = _vec3(2.0f, 0.f, 0.f);

	_matrix matRotL;
	D3DXMatrixRotationY(&matRotL, -m_fDoorAngle);

	_vec3 vFinalPosL;
	D3DXVec3TransformCoord(&vFinalPosL, &vLocalOffsetL, &matRotL);
	vFinalPosL += vHingeL;

	pTransL->Set_Pos(vFinalPosL);
	pTransL->Set_RotMatrix(&matRotL);

	CTransform* pTransR = pGateDoorR->Get_Component<CTransform>();
	const _vec3 vHingeR = vGatePos + _vec3(4.0f, 0.f, -vGateScale.z);
	const _vec3 vLocalOffsetR = _vec3(-2.0f, 0.f, 0.f);

	_matrix matRotR;
	D3DXMatrixRotationY(&matRotR, m_fDoorAngle);

	_vec3 vFinalPosR;
	D3DXVec3TransformCoord(&vFinalPosR, &vLocalOffsetR, &matRotR);
	vFinalPosR += vHingeR;

	pTransR->Set_Pos(vFinalPosR);
	pTransR->Set_RotMatrix(&matRotR);

	pGateDoorL->LateUpdate_GameObject(fTimeDelta);
	pGateDoorR->LateUpdate_GameObject(fTimeDelta);
	pGatePillarL->LateUpdate_GameObject(fTimeDelta);
	pGatePillarR->LateUpdate_GameObject(fTimeDelta);
	pGateTop->LateUpdate_GameObject(fTimeDelta);
	pTriggerCube->LateUpdate_GameObject(fTimeDelta);
}

CTestTile* CSceneGate::Set_GateDoor(bool bLeft)
{
	CTestTile* pDoor = CTestTile::Create(m_pGraphicDev);

	const _float fDoorWidth = 2.0f;
	const _float fDoorHeight = 4.0f;
	const _float fDoorDepth = 0.1f;
	_vec3 vScale = { fDoorWidth, fDoorHeight, fDoorDepth };
	pDoor->Get_Component<CTransform>()->Set_Scale(vScale);

	_vec3 vGatePos = m_pTransform->Get_Info(INFO_POS);

	_vec3 vOffset = { bLeft ? -4.0f : 4.0f, 0.f, -5.0f };
	_vec3 vHingePos = vGatePos + vOffset;

	pDoor->Get_Component<CTransform>()->Set_Pos(vHingePos);
	pDoor->Get_Component<CRigidBody>()->Set_UseGravity(false);

	return pDoor;
}

CTestTile* CSceneGate::Set_GateStructure(const _vec3& vPos, const _vec3& vScale)
{
	CTestTile* pObj = CTestTile::Create(m_pGraphicDev);
	pObj->Get_Component<CTransform>()->Set_Pos(vPos);
	pObj->Get_Component<CTransform>()->Set_Scale(vScale);
	pObj->Get_Component<CRigidBody>()->Set_UseGravity(false);
	return pObj;
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
	Safe_Release(pGateDoorL);
	Safe_Release(pGateDoorR);

	Safe_Release(pGatePillarL);
	Safe_Release(pGatePillarR);
	Safe_Release(pGateTop);

	Safe_Release(pTriggerCube);

	Safe_Release(m_pCollider);
	Safe_Release(m_pTransform);
	Safe_Release(m_pRigid);

	CGameObject::Free();
}

REGISTER_GAMEOBJECT(CSceneGate)