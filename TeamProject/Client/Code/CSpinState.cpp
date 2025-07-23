#pragma once
#include "pch.h"
#include "CSpinState.h"
#include "CIdleState.h"

#include "CVellum.h"
#include "CTransform.h"
#include "CRigidBody.h"
#include "CCollider.h"

#include "CProjectile.h"

#include "CSceneMgr.h"

void CSpinState::Enter(CVellum* pVellum)
{
	OutputDebugString(L"Spin : Enter\n");
	pVellum->Get_HCol()->Set_ColType(ColliderType::PASSIVE);
    pVellum->Get_HRigid()->Set_Velocity(_vec3(0.f, 15.f, 0.f));
	m_fPatternTime = 0.f;
	m_fSwitchTime = 8.f;
	m_fSpeed = 150.f;
    m_fFireSpeed = 15.f;
	m_fFireDelay = 0.f;
	m_fAngle = 0.f;
    m_bRotationStarted = false;

}

void CSpinState::Update(const _float fTimeDelta, CVellum* pVellum)
{
    const float TARGET_HEIGHT = 40.f;
    _vec3 vVellumPos = pVellum->Get_HTransform()->Get_Pos();
    vector<CMonsterPart*> pParts = pVellum->Get_Part();

    for (auto pPart : pParts)
    {
        pPart->Set_Follow(false);
    }

    if (vVellumPos.y < TARGET_HEIGHT)
    {
        for (size_t i = 0; i < pParts.size(); ++i)
        {
            const float fPartSpacing = 2.f;
            _vec3 vTargetPartPos = vVellumPos;
            vTargetPartPos.y -= (i + 1) * fPartSpacing;
            vTargetPartPos.x = vVellumPos.x;
            vTargetPartPos.z = vVellumPos.z;

            _vec3 vCurrentPartPos = pParts[i]->Get_Component<CTransform>()->Get_Pos();
            _vec3 vNewPos;
          
            D3DXVec3Lerp(&vNewPos, &vCurrentPartPos, &vTargetPartPos, fTimeDelta);
            pParts[i]->Get_Component<CTransform>()->Set_Pos(vNewPos);
        }
        return;
    }

    if (!m_bRotationStarted)
    {
        m_bRotationStarted = true;
        pVellum->Get_HRigid()->Stop_Motion();
        m_fPatternTime = 0.f;


    }

    m_fAngle += D3DXToRadian(m_fSpeed * fTimeDelta);
    const float fOrbitRadius = 25.f;
    const float fAngleBetweenParts = (D3DX_PI * 2.f) / pParts.size();

    for (size_t i = 0; i < pParts.size(); ++i)
    {
        float fPartAngle = m_fAngle + (fAngleBetweenParts * i);

        // 목표 위치 계산
        _vec3 vTargetOrbitPos;
        vTargetOrbitPos.x = vVellumPos.x + fOrbitRadius * cosf(fPartAngle);
        vTargetOrbitPos.y = vVellumPos.y;
        vTargetOrbitPos.z = vVellumPos.z + fOrbitRadius * sinf(fPartAngle);

        _vec3 vCurrentPartPos = pParts[i]->Get_Component<CTransform>()->Get_Pos();
        _vec3 vNewPos;

        // 보간
        D3DXVec3Lerp(&vNewPos, &vCurrentPartPos, &vTargetOrbitPos, fTimeDelta);
        pParts[i]->Get_Component<CTransform>()->Set_Pos(vNewPos);


        // 파츠가 바깥쪽을 바라보도록 설정
        _vec3 vLookDir = vNewPos - vVellumPos;
        vLookDir.y = 0.f;
        D3DXVec3Normalize(&vLookDir, &vLookDir);
        pParts[i]->Get_Component<CTransform>()->Set_Look(vLookDir);
    }

    const float fFireInterval = 1.8f;
    m_fFireDelay += fTimeDelta;
    
    if (m_fFireDelay >= fFireInterval)
    {
        m_fFireDelay = 0.f;
        CSoundMgr::Get_Instance()->Play("Spread");
        for (auto pPart : pParts)
        {
            _vec3 vPartPos = pPart->Get_Component<CTransform>()->Get_Pos();

            _vec3 vFireDir = vPartPos - vVellumPos;
            vFireDir.y = 0.f;
            D3DXVec3Normalize(&vFireDir, &vFireDir);

            CProjectile* pProjectile = CProjectile::Create(pVellum->Get_Dev());

            pProjectile->Get_Component<CTransform>()->Set_Pos(vPartPos + vFireDir * 1.5f);
            pProjectile->Get_Component<CRigidBody>()->Add_Velocity(vFireDir * m_fFireSpeed);

            
            static int iProjectileCnt = 0;
            CSceneMgr::Get_Instance()->Get_Scene()->
                Get_Layer(LAYER_OBJECT)->Add_GameObject(L"SpinProjectile_" + to_wstring(iProjectileCnt++), pProjectile);
            
        }
        m_fFireSpeed *= 0.66f;
    }

    m_fPatternTime += fTimeDelta;
    if (m_fPatternTime >= m_fSwitchTime)
    {
        for (auto pPart : pParts)
        {
            pPart->Set_Follow(true);
        }
        pVellum->Change_Pattern(new CIdleState());
    }
}


void CSpinState::Exit(CVellum* pVellum)
{
	OutputDebugString(L"Spin : Exit\n");
	vector<CMonsterPart*> pPart = pVellum->Get_Part();
	for (auto part : pPart)
	{
		part->Get_Component<CRigidBody>()->Stop_AMotion();
		part->Get_Component<CRigidBody>()->Stop_Motion();
		part->Get_Component<CCollider>()->Set_ColType(ColliderType::PASSIVE);
	}
	pVellum->Get_HCol()->Set_ColType(ColliderType::PASSIVE);
}
