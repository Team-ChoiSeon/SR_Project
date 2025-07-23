#pragma once
#include "pch.h"
#include "CDeadState.h"

#include "CVellum.h"
#include "CProjectile.h"

#include "CModel.h"
#include "CTransform.h"
#include "CRigidBody.h"
#include "CCollider.h"

#include "CSceneMgr.h"

void CDeadState::Enter(CVellum* pVellum)
{
    pVellum->Get_Component<CModel>()->Set_Model(L"Head_Dead.obj", L"Head_Dead.mtl");
    pVellum->Get_Component<CModel>()->Get_Material()->Set_Shader(L"g_UVScale.fx");
	// sound
	m_fDeadTime = 0.f;
    m_fDuration = 10.f;
	// color
}

void CDeadState::Update(const _float fTimeDelta, CVellum* pVellum)
{
	m_fDeadTime += fTimeDelta;
	
    if (m_fDeadTime >= m_fDuration)
    {
        Exit(pVellum);
        return;
    }
    
    CModel* pModel = pVellum->Get_Component<CModel>();
    if (!pModel) return;


    if (m_fDeadTime >= m_fDuration - 1.f)
    {
        pModel->Set_Alpha(1.f);
        if(!m_bFire)
        {
            CTransform* pTransform = pVellum->Get_Component<CTransform>();
            _vec3 vPos = pTransform->Get_Pos();
            _float fAngle = 0.f;
            for (size_t i = 0; i < 30; ++i)
            {
                _float fRadian = D3DXToRadian(fAngle);
                _vec3 vFireDir = { cosf(fRadian), 0.f, sinf(fRadian) };

                D3DXVec3Normalize(&vFireDir, &vFireDir);

                CProjectile* pProjectile = CProjectile::Create(pVellum->Get_Dev());

                pProjectile->Get_Component<CTransform>()->Set_Pos(vPos + vFireDir * 1.f);
                pProjectile->Get_Component<CCollider>()->Set_ColType(ColliderType::PASSIVE);
                pProjectile->Get_Component<CRigidBody>()->Add_Velocity(vFireDir * 50.f);
                
                static int iProjectileCnt = 0;
                CSceneMgr::Get_Instance()->Get_Scene()->
                    Get_Layer(LAYER_OBJECT)->Add_GameObject(L"DeadProjectile_" + to_wstring(iProjectileCnt++), pProjectile);
                fAngle += 12.f;
            }
            m_bFire = true;
            CSoundMgr::Get_Instance()->Play("Dead");
        }
    }
    else 
    {
        const _float fSpeed = 10.f;
        const _float fBlinkingDuration = m_fDuration - 1.f;
        _float alpha = 1.f - (m_fDeadTime / fBlinkingDuration);
        alpha *= (0.5f + 0.5f * sin(m_fDeadTime * fSpeed * D3DX_PI));
        pModel->Set_Alpha(alpha < 0.f ? 0.f : alpha);
    }
}

void CDeadState::Exit(CVellum* pVellum)
{
    pVellum->Get_Component<CCollider>()->Set_Active(false);
    pVellum->Get_Component<CModel>()->Set_Active(false);
	pVellum->Set_Dead(true);
    
}
