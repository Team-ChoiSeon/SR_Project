#pragma once
#include "pch.h"
#include "CDiveState.h"

#include "CVellum.h"
#include "CTransform.h"
#include "CRigidBody.h"
#include "CCollider.h"
#include "CIdleState.h"

#include "CTestTile.h"
#include "CSceneMgr.h"

void CDiveState::Enter(CVellum* pVellum)
{
    OutputDebugString(L"Dive : Enter\n");
    m_eDPhase = DivePhase::Ready;
    m_fSearch = 0.f;
    m_iCnt = pVellum->Get_PartCnt();
    pVellum->Get_HCol()->Set_ColType(ColliderType::PASSIVE);
    if (pVellum->Get_HTransform()->Get_Pos().y < 30.f)
        pVellum->Get_HRigid()->Set_Velocity(_vec3(0.f, 10.f, 0.f));
}

void CDiveState::Update(const _float fTimeDelta, CVellum* pVellum)
{
    CRigidBody* pRigid = pVellum->Get_HRigid();
    CTransform* pTransform = pVellum->Get_HTransform();

    _vec3 vVel = pRigid->Get_Velocity();
    if (D3DXVec3LengthSq(&vVel) > 0.001f)   pTransform->Set_Look(vVel);
    
    _vec3 dir = pVellum->Get_Target()->Get_Component<CTransform>()->Get_Pos()
        - pTransform->Get_Pos();

    _vec3 diff = dir;
    diff.y = 0.f;
    _float fDist = D3DXVec3Length(&diff);
    D3DXVec3Normalize(&diff, &diff);

    switch (m_eDPhase)
    {
    // force → phase = DiveIn;
    case DivePhase::Ready:
        if (fDist < 5.f)
        {
            pRigid->Stop_Motion();
            m_fSearch = 0.f;
            m_eDPhase = DivePhase::In;
            OutputDebugString(L"Ready->In\n");
        }
        m_fSearch += fTimeDelta;
        if (m_fSearch > 1.f)
        {
            pRigid->Stop_Motion();
            m_fSearch = 0.f;
        }

        pRigid->Add_Force(diff * 10.f);
        break;

     // 도달 체크 → phase = Wait;
    case DivePhase::In:
        if (pTransform->Get_Pos().y < -m_iCnt * 4.f)
        {
            pRigid->Stop_Motion();
            m_eDPhase = DivePhase::Wait;
            OutputDebugString(L"In->Wait\n");
            CTestTile* pTile = Calc_Tile(pTransform->Get_Pos(), pVellum);
            if (pTile) pTile->Set_Destroy(true);
        }
        pRigid->Add_Force({ 0.f,-1.f * 15.f, 0.f });
        break;

     // 시간 경과 → phase = DiveOut;
    case DivePhase::Wait:
        if (fDist < 5.f)
        {
            pRigid->Stop_Motion();
            m_eDPhase = DivePhase::Out;
            m_fSearch = 0.f;
            OutputDebugString(L"Wait->Out\n");
        }
        m_fSearch += fTimeDelta;
        if (m_fSearch > 1.f)
        {
            pRigid->Stop_Motion();
            m_fSearch = 0.f;
        }

        pRigid->Add_Force(diff * 10.f);
        break;

     // 상승 
    case DivePhase::Out:
        if (pTransform->Get_Pos().y > m_iCnt * 4.f)
        {
            pRigid->Stop_Motion();
            OutputDebugString(L"Out\n");
            CTestTile* pTile = Calc_Tile(pTransform->Get_Pos(), pVellum);
            if (pTile) pTile->Set_Destroy(true);

        }
        pRigid->Add_Force({ 0.f,1.f * 15.f, 0.f });
        break;

    }

    if (pTransform->Get_Pos().y > m_iCnt * 4.f && m_eDPhase == DivePhase::Out)
    {

        pVellum->Change_Pattern(new CIdleState());
    }

}

void CDiveState::Exit(CVellum* pVellum)
{
    pVellum->Get_HRigid()->Stop_Motion();
    OutputDebugString(L"Dive : Exit\n");
}

CTestTile* CDiveState::Calc_Tile(const _vec3& pos, CVellum* pVellum)
{
    CScene* pScene = CSceneMgr::Get_Instance()->Get_Scene();
    if (!pScene) return nullptr;

    for (int z = 0; z < 3; ++z)
    {
        for (int x = 0; x < 3; ++x)
        {
            wstring wTileName = to_wstring(z) + to_wstring(x);

            CTestTile* pTile = pScene->Get_Layer(LAYER_TILE)->Get_GameObject<CTestTile>(wTileName);
            if (!pTile) continue;

            CTransform* pTransform = pTile->Get_Component<CTransform>();
            _vec3 vTilePos = pTransform->Get_Pos();
            _vec3 vScale = pTransform->Get_Scale();
            _vec3 vPos = pVellum->Get_Component<CTransform>()->Get_Pos();
            if ((vPos.x >= vTilePos.x - vScale.x && vPos.x <= vTilePos.x + vScale.x) &&
                (vPos.z >= vTilePos.z - vScale.z && vPos.z <= vTilePos.z + vScale.z))
            {
                return pTile; // 벨룸이 속한 타일을 찾았으므로 반환
            }
        }
    }

    return nullptr; // 9개 타일 모두 해당 없음
}
