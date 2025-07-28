#pragma once
#include "pch.h"
#include "CSceneTestHS.h"
#include "CFactory.h"

#include "CTimeMgr.h"
#include "CInputMgr.h"
#include "CLightMgr.h"
#include "CCameraMgr.h"
#include "CUiMgr.h"
#include "CResourceMgr.h"
#include "CPickingMgr.h"
#include "CCollisionMgr.h"
#include "CSceneMgr.h"

#include "CMainPlayer.h"
#include "CLightObject.h"
#include "CTestLightMeshObject.h"
#include "CCrosshairUIObject.h"
#include "DummyCube.h"
#include "CDirectionalCube.h"
#include "CTestTile.h"
#include "CCinematicCamera.h"

#include "CCamera.h"
#include "CFirstviewFollowingCamera.h"

CSceneTestHS::CSceneTestHS(LPDIRECT3DDEVICE9 pGraphicDev)
    :CScene(pGraphicDev)
{

}

CSceneTestHS::~CSceneTestHS()
{
}

HRESULT CSceneTestHS::Ready_Scene()
{
    Init_Layers();

    CMainPlayer* pPlayer = CMainPlayer::Create(m_pGraphicDev);
    pPlayer->Get_Component<CTransform>()->Set_Pos({ 0.f, 30.f, -75.f });
    pPlayer->Set_ResponPos({ 0.f, 30.f, -75.f });
    CSceneMgr::Get_Instance()->Set_Player(pPlayer);

    for (auto& tile : Get_Layer(LAYER_TILE)->Get_ObjVec()) {
        tile.pObj->Get_Component<CRigidBody>()->Set_UseGravity(false);
    }

    auto pSetCameraObj = Get_Layer(LAYER_OBJECT)->Get_GameObject<CTestTile>(L"CSetCamera");

    CCinematicCamera* pCine = CCinematicCamera::Create(m_pGraphicDev);
    pCine->Set_Target(pSetCameraObj);

    Get_Layer(LAYER_PLAYER)->Add_GameObject(L"Player", pPlayer);
    Get_Layer(LAYER_CAMERA)->Add_GameObject(L"Cinematic", pCine);

    CCameraMgr::Get_Instance()->Set_MainCamera(pCine);

    pCine->Start_Cinematic();

    return S_OK;
}

_int CSceneTestHS::Update_Scene(const _float& fTimeDelta)
{
    CScene::Update_Scene(fTimeDelta);
    StartEnding(fTimeDelta);

    return 0;
}

void CSceneTestHS::LateUpdate_Scene(const _float& fTimeDelta)
{
    CScene::LateUpdate_Scene(fTimeDelta);
}

void CSceneTestHS::StartEnding(const _float& fTimeDelta)
{
    auto pSetCameraObj = Get_Layer(LAYER_OBJECT)->Get_GameObject<CTestTile>(L"CSetCamera");
    if (!pSetCameraObj) return;

    auto pTransform = pSetCameraObj->Get_Component<CTransform>();
    auto pRigid = pSetCameraObj->Get_Component<CRigidBody>();
    if (!pTransform || !pRigid) return;

    if (!bLaunched)
    {
        pRigid->Set_UseGravity(false);
        pRigid->Set_Mass(1.f);
        bLaunched = true;
    }

    if (fElapsed < 3.f)
    {
        pTransform->Rotate_Axis(_vec3(0.f, 1.f, 0.f), D3DXToRadian(-15.f * fTimeDelta));

        _vec3 vBack = -pTransform->Get_Info(INFO_LOOK);
        D3DXVec3Normalize(&vBack, &vBack);

        _float fSpeed = fElapsed * 120.f;
        pTransform->Move_Pos(&vBack, fSpeed, fTimeDelta);

    }
    else if (fElapsed < 3.5f)
    {

    }
    else if (!bReadyToLaunch)
    {

        if (fElapsed < 6.5f)
        {
            pTransform->Rotate_Axis(_vec3(0.f, 1.f, 0.f), D3DXToRadian(-20.f * fTimeDelta));
        }
        _vec3 vDir = pTransform->Get_Info(INFO_LOOK);
        D3DXVec3Normalize(&vDir, &vDir);

        _float fLaunchSpeed = 200.f;
        pTransform->Move_Pos(&vDir, fLaunchSpeed, fTimeDelta);

        //bReadyToLaunch = true;
    }
    fElapsed += fTimeDelta;
}





CSceneTestHS* CSceneTestHS::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
    CSceneTestHS* pScene = new CSceneTestHS(pGraphicDev);

    return pScene;
}

void CSceneTestHS::Free()
{
    CScene::Free();
}