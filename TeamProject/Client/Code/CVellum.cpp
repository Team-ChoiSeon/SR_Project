#pragma once
#include "pch.h"
#include "CVellum.h"
#include "CRigidBody.h"
#include "CCollider.h"


#include "CInputMgr.h"
#include "CSceneMgr.h"

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
    m_vPart.reserve(m_iPartCnt);

    CMonsterPart* pPrev = nullptr;

    for (int i = 0; i < m_iPartCnt; ++i)
    {
        CMonsterPart* pPart = CMonsterPart::Create(m_pGraphicDev);
        if (!pPart)
        {
            MSG_BOX("Vellum ���� ���� ����");
            return E_FAIL;
        }

        // ���� ��ġ �ʱ�ȭ (���� �迭 ����)
        _vec3 partPos = { 0.f, 20.f - 2.f * i, 0.f };
        pPart->Get_Component<CTransform>()->Set_Pos(partPos);

        // �� ������ ���󰡰� ����
        if (pPrev)
            pPart->Set_Target(pPrev);
        
        pPart->Set_Index(i, m_iPartCnt);
        m_vPart.push_back(pPart);
        pPrev = pPart;
    }

    // ��� ���� ����
    m_pTransform = m_vPart[0]->Get_Component<CTransform>();
    m_pRigid = m_vPart[0]->Get_Component<CRigidBody>();
    m_pCol = m_vPart[0]->Get_Component<CCollider>();
   
	return CGameObject::Ready_GameObject();;
}

int CVellum::Update_GameObject(const _float& fTimeDelta)
{
    if (m_vPart.empty()) return -1;
    m_pTarget = CSceneMgr::Get_Instance()->Get_Player();
    if (!m_pTarget) return;
    
    switch (m_eCurPattern)
    {
    case VPattern::CHASE:
        Chase(fTimeDelta, m_pTransform, m_pRigid);
        break;
    case VPattern::DIVE:
        Dive(fTimeDelta, m_pTransform, m_pRigid);
        break;
    }

    Update_Pattern(fTimeDelta);

    Key_Input(fTimeDelta);
    for (int i = 1; i < m_iPartCnt; i++)
    {
        /*Snake_Curve(fTimeDelta, m_vPart[i]->Get_Component<CTransform>(), i);*/
    }
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
    m_pTransform = nullptr;
    m_pRigid = nullptr;
    m_pCol = nullptr;

    for (auto* pPart : m_vPart)
        Safe_Release(pPart);

    m_vPart.clear();

    CGameObject::Free();

}

void CVellum::Update_Pattern(const _float& fTimeDelta)
{
    m_fPatternTime += fTimeDelta;

    if (m_eCurPattern == VPattern::DIVE && m_bPattern)
        return;

    // ���� ���� ��
    if (m_bPattern && m_fPatternTime > m_fSwitchTime)
    {
        if (!m_vPart.empty())
        {

            m_pCol->Set_ColType(ColliderType::ACTIVE);
            m_pRigid->Set_Velocity(_vec3(0.f, 0.f, 0.f));
            m_pRigid->Set_AVelocity(_vec3(0.f, 0.f, 0.f));
        }

        m_bPattern = false;
        m_fPatternTime = 0.f;
        m_fSwitchTime = 3.f;    // ���� ���ϱ����� ���͹�

        return;
    }

    // ���� ���� �� ����
    if (!m_bPattern && m_fPatternTime >= m_fSwitchTime)
    {
        m_bPattern = true;
        m_fPatternTime = 0.f;

        m_eCurPattern = static_cast<VPattern>(rand() % 2);
        
        // ���� ��� �ð�
        switch (m_eCurPattern)
        {
        case VPattern::CHASE:
            m_fSwitchTime = 3.f;
            break;
        case VPattern::DIVE:
            m_fSwitchTime = FLT_MAX;  // ������ ������ ����
            // 
            if (m_eDPhase == DivePhase::None)
                m_pRigid->Set_Velocity(_vec3(0.f, 10.f, 0.f));
            
            m_eDPhase = DivePhase::Ready;
            break;
        }

        

        if (!m_vPart.empty())
            m_pCol->Set_ColType(ColliderType::PASSIVE);

        switch (m_eCurPattern)
        {
        case VPattern::CHASE:  OutputDebugString(L"����: CHASE\n"); break;
        case VPattern::DIVE:  OutputDebugString(L"����: DIVE\n"); break;
        }
    }
}



void CVellum::Chase(const _float& fTimeDelta, CTransform* pTransform, CRigidBody* pRigid)
{


    _vec3 myPos = pTransform->Get_Pos();
    _vec3 playerPos = m_pTarget->Get_Component<CTransform>()->Get_Pos();
    _vec3 dir = playerPos - myPos;

    D3DXVec3Normalize(&dir, &dir);

    pTransform->Set_Look(dir);
    pRigid->Add_Torque(dir * 30.f);
    pRigid->Add_Force(dir * 5.f);
}

void CVellum::Dive(const _float& fTimeDelta, CTransform* pTransform, CRigidBody* pRigid)
{

    if (!pTransform || !pRigid)
        return;

    _vec3 myPos = pTransform->Get_Pos();
    _vec3 playerPos = m_pTarget->Get_Component<CTransform>()->Get_Pos();
    _vec3 diff = playerPos - myPos;
    diff.y = 0.f;
    _float fDist = D3DXVec3Length(&diff);
    D3DXVec3Normalize(&diff, &diff);

    switch (m_eDPhase)
    {
    // force �� phase = DiveIn;
    case DivePhase::Ready:
        if (fDist < 5.f)
        {
            pRigid->Set_Accel({ 0.f,0.f,0.f });
            pRigid->Set_Velocity({ 0.f,0.f,0.f });
            m_fSearch = 0.f;
            m_eDPhase = DivePhase::In;
            OutputDebugString(L"Ready->In\n");
        }
        m_fSearch += fTimeDelta;
        if (m_fSearch > 1.f)
        {
            pRigid->Set_Accel({ 0.f,0.f,0.f });
            pRigid->Set_Velocity({ 0.f,0.f,0.f });
            m_fSearch = 0.f;
        }

        pRigid->Add_Force(diff * 10.f);
        break;

     // ���� üũ �� phase = Wait;
    case DivePhase::In:
        if (pTransform->Get_Pos().y < -5.f)
        {
            pRigid->Set_Accel({ 0.f,0.f,0.f });
            pRigid->Set_Velocity({ 0.f,0.f,0.f });
            m_eDPhase = DivePhase::Wait;
            OutputDebugString(L"In->Wait\n");
        }
        pRigid->Add_Force({ 0.f,-1.f * 15.f, 0.f });
        break;

    // �ð� ��� �� phase = DiveOut;
    case DivePhase::Wait:
        if (fDist < 5.f)
        {
            pRigid->Set_Accel({ 0.f,0.f,0.f });
            pRigid->Set_Velocity({ 0.f,0.f,0.f });
            m_eDPhase = DivePhase::Out;
            m_fSearch = 0.f;
            OutputDebugString(L"Wait->Out\n");
        }
        m_fSearch += fTimeDelta;
        if (m_fSearch > 1.f)
        {
            pRigid->Set_Accel({ 0.f,0.f,0.f });
            pRigid->Set_Velocity({ 0.f,0.f,0.f });
            m_fSearch = 0.f;
        }

        pRigid->Add_Force(diff * 10.f);
        break;

    // ��� �Ϸ� �� phase = Ready, m_bPattern = false;
    case DivePhase::Out:
        if (pTransform->Get_Pos().y > 15.f)
        {
            pRigid->Set_Accel({ 0.f,0.f,0.f });
            pRigid->Set_Velocity({ 0.f,0.f,0.f });
            m_eDPhase = DivePhase::None;
            m_vPart[0]->Get_Component<CCollider>()->Set_ColType(ColliderType::ACTIVE);
            OutputDebugString(L"Out->None\n");

            m_bPattern = false;
            m_fPatternTime = 0.f;
            m_fSwitchTime = 5.f; // ���� ���� ���͹�
        }
        pRigid->Add_Force({ 0.f,1.f * 15.f, 0.f });
        break;
        
    }
    
}



//void CVellum::Snake_Curve(const _float& fTimeDelta, CTransform* pTransform, int idx)
//{
//    m_fCurveTime += fTimeDelta;
//
//    const _float fAmp = 0.05f; // ����
//    const _float fFre = 0.1f;  // ������
//    
//    _vec3 vPos = pTransform->Get_Pos();
//    vPos.y += fAmp * sinf(fFre * m_fCurveTime + idx * 0.5f); // 0.5f : ���� offset
//
//    pTransform->Set_Pos(vPos);
//
//}

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

    //  ���� : �޸� ���� �߻��ϴ� �ǵ��� ������� ����
    if (CInputMgr::Get_Instance()->Key_Down(DIK_R))
    {
        m_vPart.clear();
    }

    m_pTransform->Set_Pos(pos); // ����
}

REGISTER_GAMEOBJECT(CVellum)