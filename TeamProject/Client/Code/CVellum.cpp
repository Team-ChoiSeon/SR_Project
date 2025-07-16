#pragma once
#include "pch.h"
#include "CVellum.h"

#include "CTransform.h"
#include "CRigidBody.h"
#include "CCollider.h"

#include "CInputMgr.h"
#include "CSceneMgr.h"

#include "CIdleState.h"

#include "CFactory.h"



CVellum::CVellum(LPDIRECT3DDEVICE9 pGraphicDev)
	:CGameObject(pGraphicDev)
{

}

CVellum::~CVellum()
{
}

CVellum* CVellum::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CVellum* pVellum = new CVellum(pGraphicDev);

	if (FAILED(pVellum->Ready_GameObject()))
	{
		Safe_Release(pVellum);
		MSG_BOX("Vellum Create Failed");
		return nullptr;
	}

	return pVellum;
}

HRESULT CVellum::Ready_GameObject()
{
    Add_Component<CModel>(ID_DYNAMIC, m_pGraphicDev);
    m_pModel = Get_Component<CModel>();

    Add_Component<CTransform>(ID_DYNAMIC, m_pGraphicDev);
    m_pTransform = Get_Component<CTransform>();

    Add_Component<CRigidBody>(ID_DYNAMIC, m_pGraphicDev, m_pTransform);
    m_pRigid = Get_Component<CRigidBody>();

    Add_Component<CCollider>(ID_DYNAMIC, m_pGraphicDev, m_pRigid);
    m_pCol = Get_Component<CCollider>();

    m_pTransform->Set_Pos({ 0.f, 20.f, 0.f });
    m_pTransform->Set_Scale({ 1.33f, 1.33f, 1.33f });

    m_pRigid->Set_OnGround(false);
    m_pRigid->Set_UseGravity(false);
    m_pRigid->Set_Mass(1.f);
    m_pRigid->Set_Friction(1.f);
    m_pRigid->Set_Bounce(0.f);

    m_pCol->Set_ColTag(ColliderTag::NONE);
    m_pCol->Set_ColType(ColliderType::PASSIVE);
    m_pCol->Set_BoundType(BoundingType::OBB);


    m_vPart.reserve(m_iPartCnt);
    CGameObject* pTarget = this;

    for (int i = 0; i < m_iPartCnt; ++i)
    {
        CMonsterPart* pPart = CMonsterPart::Create(m_pGraphicDev);
        if (!pPart) return E_FAIL;

        // ∆ƒ√˜ ¿ßƒ° √ ±‚»≠ (º±«¸ πËø≠ «¸≈¬)
        _vec3 headPos = m_pTransform->Get_Info(INFO_POS);
        _vec3 partPos = headPos + _vec3(0.f, 2.f * (i + 1), 0.f);
        pPart->Get_Component<CTransform>()->Set_Pos(partPos);

        pPart->Set_Target(pTarget);
        pPart->Set_Index(i, m_iPartCnt);
        m_vPart.push_back(pPart);
        pTarget = pPart;
    }

    m_pTarget = CSceneMgr::Get_Instance()->Get_Player();

    m_pState = new CIdleState();
    m_pState->Enter(this);

    CFactory::Save_Prefab(this, "CVellum");
	return CGameObject::Ready_GameObject();;
}

int CVellum::Update_GameObject(const _float& fTimeDelta)
{
    
    if (m_vPart.empty()) return -1;

    if (m_pState)
        m_pState->Update(fTimeDelta, this);

    Key_Input(fTimeDelta);

	CGameObject::Update_GameObject(fTimeDelta);
    for (auto* pPart : m_vPart)
    {
        pPart->Update_GameObject(fTimeDelta);
    }

	return 0;
}

void CVellum::LateUpdate_GameObject(const _float& fTimeDelta)
{
	CGameObject::LateUpdate_GameObject(fTimeDelta);
    for (auto* pPart : m_vPart)
        pPart->LateUpdate_GameObject(fTimeDelta);
}


void CVellum::Free()
{
    Safe_Delete(m_pState);

    m_pTransform = nullptr;
    m_pRigid = nullptr;
    m_pCol = nullptr;

    for (auto* pPart : m_vPart)
        Safe_Release(pPart);
    m_vPart.clear();

    CGameObject::Free();
}


void CVellum::Change_Pattern(IVellumState* pState)
{
    // Í∏∞Ï°¥ ?ÅÌÉúÍ∞Ä ?àÎã§Î©?Exit ?®ÏàòÎ•??∏Ï∂ú
    if (m_pState) 
    {
        m_pState->Exit(this);
        Safe_Delete(m_pState);
    }

    m_pState = pState;
    if (m_pState)  m_pState->Enter(this);
}




void CVellum::On_Hit(const _vec3& hitpos)
{

}

void CVellum::Key_Input(const _float& fTimeDelta)
{
    if (m_vPart.empty())
        return;

    if (!m_pTransform || !m_pRigid)
        return;

    const float speed = 10.0f;
    _vec3 pos = m_pTransform->Get_Pos();

    if (CInputMgr::Get_Instance()->Key_Down(DIK_UP))
    {
        m_pRigid->Add_Torque(_vec3(1.f, 0.f, 0.f) * 50.f); // 
    }

    if (CInputMgr::Get_Instance()->Key_Down(DIK_DOWN))
    {
        m_pRigid->Add_Torque(_vec3(-1.f, 0.f, 0.f) * 50.f); //  
    }

    if (CInputMgr::Get_Instance()->Key_Down(DIK_LEFT))
    {
        m_pRigid->Add_Torque(_vec3(0.f, 0.f, 1.f) * 50.f); // 
    }

    if (CInputMgr::Get_Instance()->Key_Down(DIK_RIGHT))
    {
        m_pRigid->Add_Torque(_vec3(0.f, 0.f, -1.f) * 50.f); // 
    }

    if (CInputMgr::Get_Instance()->Key_Down(DIK_NUMPAD8)) // +Y
        pos.y += speed * fTimeDelta;
    if (CInputMgr::Get_Instance()->Key_Down(DIK_NUMPAD2)) // -Y
        pos.y -= speed * fTimeDelta;
    if (CInputMgr::Get_Instance()->Key_Down(DIK_NUMPAD6)) // +X
        pos.x += speed * fTimeDelta;
    if (CInputMgr::Get_Instance()->Key_Down(DIK_NUMPAD4)) // -X
        pos.x -= speed * fTimeDelta;
    if (CInputMgr::Get_Instance()->Key_Down(DIK_NUMPAD9)) // +Z
        pos.z += speed * fTimeDelta;
    if (CInputMgr::Get_Instance()->Key_Down(DIK_NUMPAD7)) // -Z
        pos.z -= speed * fTimeDelta;

 
    //if (CInputMgr::Get_Instance()->Key_Down(DIK_R))
    //{
    //    m_vPart.clear();
    //}

    m_pTransform->Set_Pos(pos); // ?ÅÏö©
}

REGISTER_GAMEOBJECT(CVellum)