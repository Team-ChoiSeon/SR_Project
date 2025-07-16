#include "pch.h"
#include "CMainPlayer.h"
#include "CCubeTex.h"

#include "CCube.h"
#include "CSwitch.h"
#include "CCameraObject.h"

#include "CVIBuffer.h"
#include "CTransform.h"
#include "CCollider.h"

#include "CInputMgr.h"
#include "CCameraMgr.h"
#include "CPickingMgr.h"

#include "CFactory.h"
CMainPlayer::CMainPlayer(LPDIRECT3DDEVICE9 pGraphicDev)
	: CGameObject(pGraphicDev)
{
}
CMainPlayer::~CMainPlayer()
{
}

HRESULT CMainPlayer::Ready_GameObject()
{
	Add_Component<CModel>(ID_DYNAMIC, m_pGraphicDev);
	m_pModel = Get_Component<CModel>();

	Add_Component<CTransform>(ID_DYNAMIC, m_pGraphicDev);
	m_pTransform = Get_Component<CTransform>();

	Add_Component<CRigidBody>(ID_DYNAMIC, m_pGraphicDev, m_pTransform);
	m_pRigid = Get_Component<CRigidBody>();

	Add_Component<CCollider>(ID_DYNAMIC, m_pGraphicDev, m_pRigid);
	m_pCollider = Get_Component<CCollider>();

	m_pTransform->Ready_Component();

	m_pTransform->Set_Scale({ .8f, 4.f, .8f });
	m_pTransform->Set_Pos({ 0.f, 0.f, 0.f });
	m_pTransform->Set_Look({ 0.f, 0.f, 1.f });
	m_pTransform->Set_Up({ 0.f, 1.f, 0.f });
	m_pTransform->Set_Right({ 1.f, 0.f, 0.f });
	m_fMoveSpeed = 10.f;
	
	// �ӽ��߰� 
	m_pRigid->Set_Mass(6.f);
	m_pRigid->Set_Friction(10.f);
	m_pRigid->Set_Gravity(5.f);

	m_eCurState = PLAYER_STATE::PLAYER_IDLE;
	m_ePrevState = PLAYER_STATE::PLAYER_IDLE;
	m_fPickObjDist = 0.f;
	m_fPickPointDist = 0.f;
	m_vPickObjDist = { 0.f, 0.f, 0.f };
	m_vPickPointDist = { 0.f, 0.f, 0.f };

	CFactory::Save_Prefab(this, "CMainPlayer");

	return S_OK;
}

int CMainPlayer::Update_GameObject(const _float& fTimeDelta)
{
	m_fJumpTime += fTimeDelta;
	KeyInput(fTimeDelta);
	Update_State(fTimeDelta);

	CGameObject::Update_GameObject(fTimeDelta);
	
	return S_OK;
}

void CMainPlayer::LateUpdate_GameObject(const _float& fTimeDelta)
{
	CGameObject::LateUpdate_GameObject(fTimeDelta);
}

CMainPlayer* CMainPlayer::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CMainPlayer* pPlayer = new CMainPlayer(pGraphicDev);

	if (FAILED(pPlayer->Ready_GameObject()))
	{
		Safe_Release(pPlayer);
		MSG_BOX("Player Create Failed");
		return nullptr;
	}

	return pPlayer;
}

void CMainPlayer::Free()
{
	Safe_Release(m_pTransform);
	Safe_Release(m_pCollider);
	Safe_Release(m_pModel);
	Safe_Release(m_pRigid);

}

void CMainPlayer::KeyInput(const _float& fTimeDelta)
{
	if (CInputMgr::Get_Instance()->Key_Tap(DIK_TAB)) {
		m_bCursorMove = !m_bCursorMove;
	}
	if (m_bCursorMove)
		CursorRotate();

	if (CInputMgr::Get_Instance()->Key_Hold(DIK_LSHIFT)) {
		m_fMoveSpeed = 30.f;
	}

	//======================================== New Picking ===============================================//
	
	Picking_Init();
	if (m_pPickObj) {
		if (CInputMgr::Get_Instance()->Mouse_Tap(DIM_LB))
		{
			Tap_Picking();
		}
		else if (CInputMgr::Get_Instance()->Mouse_Hold(DIM_LB))
		{
			Hold_Picking();
		}
		else if (CInputMgr::Get_Instance()->Mouse_Away(DIM_LB))
		{
			Away_Picking();
		}
		else
		{
			m_pCrosshair->Set_State(CCrosshairUIObject::CROSSHAIR_STATE::CROSS_HOVER);
		}
	}
	else
		m_pCrosshair->Set_State(CCrosshairUIObject::CROSSHAIR_STATE::CROSS_DEFAULT);

	//=====================================================================================================//


	//Ray* pRay = CPickingMgr::Get_Instance()->Get_Ray();
	//m_pPickObj = CPickingMgr::Get_Instance()->Get_HitNearObject(100.f);
	//auto* pPickCubeObj = dynamic_cast<CCube*>(m_pPickObj);
	//auto* pPickSwitchObj = dynamic_cast<CSwitch*>(m_pPickObj);
	//if (pPickCubeObj) {
	//	pPickCubeObj->Set_Grab(false);
	//}
	//if (pPickSwitchObj)
	//{
	//	pPickSwitchObj->Set_Grab(false);
	//}

	//m_pCrosshair
	//m_bMouseTap = false;
	//m_bMouseAway = false;
	//if(m_pCrosshair)
	//{
	//	if (m_pPickObj)
	//	{
	//		if (CInputMgr::Get_Instance()->Mouse_Tap(DIM_LB))
	//		{
	//			CTransform* targetTrans = m_pPickObj->Get_Component<CTransform>();
	//			CCamera* pMainCam = CCameraMgr::Get_Instance()->Get_MainCamera()->Get_Component<CCamera>();

	//			//m_vPlanePt = PickObj->Get_Component<CTransform>()->Get_Pos();
	//			m_vPlanePt = targetTrans->Get_Pos();
	//			m_vPlaneNorm = targetTrans->Get_Pos() - pMainCam->Get_Eye();
	//			D3DXVec3Normalize(&m_vPlaneNorm, &m_vPlaneNorm);

	//			m_vLastPt = CPickingMgr::Get_Instance()->CalcRayPlaneIntersection(*pRay, m_vPlanePt, m_vPlaneNorm);
	//			m_pCrosshair->Set_State(CCrosshairUIObject::CROSSHAIR_STATE::CROSS_HOLD);
	//			m_bObjHold = true;
	//			m_bMouseTap = true;
	//			m_pPrevPickObj = m_pPickObj;
	//		}

	//		if (CInputMgr::Get_Instance()->Mouse_Down(DIM_LB))
	//		{
	//			if (m_bObjHold)
	//			{
	//				m_pCrosshair->Set_State(CCrosshairUIObject::CROSSHAIR_STATE::CROSS_HOLD);
	//				_vec3 nowPt = CPickingMgr::Get_Instance()->CalcRayPlaneIntersection(*pRay, m_vPlanePt, m_vPlaneNorm);
	//				m_vDragDistance = nowPt - m_vLastPt;
	//				m_vLastPt = nowPt;

	//				if (pPickCubeObj) {
	//					pPickCubeObj->Set_Grab(true);
	//					pPickCubeObj->Set_CursorVec(m_vDragDistance);
	//				}
	//				if (pPickSwitchObj) {
	//					pPickSwitchObj->Set_Grab(true);
	//					pPickSwitchObj->Set_CursorVec(m_vDragDistance);
	//				}

	//			}
	//		}
	//		else {
	//			m_pCrosshair->Set_State(CCrosshairUIObject::CROSSHAIR_STATE::CROSS_HOVER);
	//		}
	//	}
	//	else {
	//		if (m_bObjHold)
	//			m_bMouseAway = true;

	//		m_pCrosshair->Set_State(CCrosshairUIObject::CROSSHAIR_STATE::CROSS_DEFAULT);
	//		m_bObjHold = false;
	//		m_vDragDistance = { 0,0,0 };
	//	}


	//	if (CInputMgr::Get_Instance()->Mouse_Away(DIM_LB)) {
	//		if (m_pPickObj)
	//		{
	//			m_pCrosshair->Set_State(CCrosshairUIObject::CROSSHAIR_STATE::CROSS_HOVER);
	//		}
	//		else {
	//			m_pCrosshair->Set_State(CCrosshairUIObject::CROSSHAIR_STATE::CROSS_DEFAULT);

	//		}
	//		m_bObjHold = false;
	//		m_bMouseAway = true;
	//		m_vDragDistance = { 0,0,0 };
	//	}
	//}


	if (CInputMgr::Get_Instance()->Key_Away(DIK_LSHIFT))
	{
		m_fMoveSpeed = 10.f;
	}


	CTransform* pCamTransform = CCameraMgr::Get_Instance()->Get_MainCamera()->Get_Component<CTransform>();
	if (!pCamTransform)
		return;

	_vec3 camLook = pCamTransform->Get_Info(INFO_LOOK);
	_vec3 camRight = pCamTransform->Get_Info(INFO_RIGHT);

	camLook.y = 0.f;
	camRight.y = 0.f;
	D3DXVec3Normalize(&camLook, &camLook);
	D3DXVec3Normalize(&camRight, &camRight);

	_vec3 moveDir = { 0.f, 0.f, 0.f };

	if (CInputMgr::Get_Instance()->Key_Down(DIK_W)) {
		moveDir += camLook;
	}
	if (CInputMgr::Get_Instance()->Key_Down(DIK_S)) {
		moveDir -= camLook;
	}
	if (CInputMgr::Get_Instance()->Key_Down(DIK_D)) {
		moveDir += camRight;
	}
	if (CInputMgr::Get_Instance()->Key_Down(DIK_A)) {
		moveDir -= camRight;
	}

	if (D3DXVec3Length(&moveDir) > 0.f) {
		D3DXVec3Normalize(&moveDir, &moveDir);
		m_pTransform->Set_Pos(m_pTransform->Get_Pos() + moveDir * m_fMoveSpeed * fTimeDelta);
	}

	// 나중에 삭제
	if (CInputMgr::Get_Instance()->Key_Down(DIK_Q)) {
		m_pTransform->Move(DIR_UP, m_fMoveSpeed, fTimeDelta);
	}
	if (CInputMgr::Get_Instance()->Key_Down(DIK_E)) {
		m_pTransform->Move(DIR_DOWN, m_fMoveSpeed, fTimeDelta);
	}

	if (CInputMgr::Get_Instance()->Key_Down(DIK_SPACE)) {
		if (m_pRigid->Get_OnGround()) {
			if (m_pRigid->Get_OnGround() && m_fJumpTime >= 1.f) {
				m_pRigid->Add_Velocity(_vec3(0.f, m_fJumpPower, 0.f));
				m_pRigid->Set_OnGround(false);
				m_fJumpTime = 0.f;
			}
		}
	}
}


void CMainPlayer::Picking_Init()
{
	m_bMouseTap = false;													//탭 초기화
	m_bMouseAway = false;													//어웨이 초기화
	m_bObjHold = false;														//홀드 초기화			//문제시 삭제
	m_pRay = CPickingMgr::Get_Instance()->Get_Ray();						//ray 계산
	m_pPickObj = CPickingMgr::Get_Instance()->Get_HitNearObject(100.f);		//Pickobj 계산


	m_PickedCube = dynamic_cast<CCube*>(m_pPickObj);						//큐브인지 확인
	if (m_PickedCube) {
		m_PickedCube->Set_Grab(false);
		m_PickedCube->Set_Tap(false);
		m_PickedCube->Set_Away(false);

	}

	m_PickedSwitch = dynamic_cast<CSwitch*>(m_pPickObj);					//스위치인지 확인
	if (m_PickedSwitch) {
		m_PickedSwitch->Set_Grab(false);
		m_PickedSwitch->Set_Tap(false);
		m_PickedSwitch->Set_Away(false);
	}
}

void CMainPlayer::Tap_Picking()
{
	CTransform* pPickTrans = m_pPickObj->Get_Component<CTransform>();
	CGameObject* pMainCam = CCameraMgr::Get_Instance()->Get_MainCamera();

	m_vPickPoint = pMainCam->Get_Component<CTransform>()->Get_Pos() + (m_pRay->_direction * CPickingMgr::Get_Instance()->Get_HitTargetList().front()._distance);
	m_vPickObjPos = pPickTrans->Get_Pos();
	m_vPickPointGap = m_vPickObjPos - m_vPickPoint;
	m_vPickPointDist = pMainCam->Get_Component<CTransform>()->Get_Pos() - m_vPickPoint;
	m_fPickPointDist = D3DXVec3Length(&m_vPickPointDist);
	m_vPickObjDist = pMainCam->Get_Component<CTransform>()->Get_Pos() - m_vPickObjPos;
	m_fPickObjDist = D3DXVec3Length(&m_vPickObjDist);

	m_vPrePickObjPos = m_vPickObjPos;
	m_vPrePickPoint = m_vPickPoint;
	m_vDragDistance = { 0,0,0 };

	m_pCrosshair->Set_State(CCrosshairUIObject::CROSSHAIR_STATE::CROSS_HOLD);
	m_bMouseTap = true;

	if (m_PickedCube)
		m_PickedCube->Set_Tap(true);
	if (m_PickedSwitch)
		m_PickedSwitch->Set_Tap(true);

	if (m_pPrevPickObj == nullptr)
		m_pPrevPickObj = m_pPickObj;
}

void CMainPlayer::Hold_Picking()
{
	CTransform* pPickTrans = m_pPickObj->Get_Component<CTransform>();
	CGameObject* pMainCam = CCameraMgr::Get_Instance()->Get_MainCamera();

	_vec3 MovedPoint = pMainCam->Get_Component<CTransform>()->Get_Pos() + (m_pRay->_direction * m_fPickPointDist);
	_vec3 MovedObjPos = MovedPoint + m_vPickPointGap;
	
	m_vDragDistance = MovedObjPos - m_vPrePickObjPos;
	m_vPrePickPoint = MovedPoint;
	m_vPrePickObjPos = MovedObjPos;

	//m_vPickPoint = m_pRay->_direction * CPickingMgr::Get_Instance()->Get_HitTargetList().front()._distance;
	//m_vPickedDist = pMainCam->Get_Component<CTransform>()->Get_Pos() - m_vPickPoint;
	//m_vPickObjDist = pMainCam->Get_Component<CTransform>()->Get_Pos() - pPickTrans->Get_Pos();
	//m_vPickPointDist = pMainCam->Get_Component<CTransform>()->Get_Pos() - m_vPickPoint;

	if (m_PickedCube) {
		m_PickedCube->Set_Grab(true);
		m_PickedCube->Set_CursorVec(m_vDragDistance);
	}
	if (m_PickedSwitch) {
		m_PickedSwitch->Set_Grab(true);
		m_PickedSwitch->Set_CursorVec(m_vDragDistance);
	}



	m_pCrosshair->Set_State(CCrosshairUIObject::CROSSHAIR_STATE::CROSS_HOLD);
	m_bObjHold = true;
	m_bMouseTap = false;
}

void CMainPlayer::Away_Picking()
{
	m_bObjHold = false;
	m_bMouseAway = true;
	m_vDragDistance = { 0,0,0 };
	m_pPickObj = nullptr;
	m_PickedCube = nullptr;
	m_PickedSwitch = nullptr;


	if (m_PickedCube)
		m_PickedCube->Set_Away(true);
	if (m_PickedSwitch)
		m_PickedSwitch->Set_Away(true);

	m_pCrosshair->Set_State(CCrosshairUIObject::CROSSHAIR_STATE::CROSS_HOVER);
}


void CMainPlayer::CursorRotate()
{
	//Ŀ�� ����
	ShowCursor(false);
	float cx = WINCX / 2.f;
	float cy = WINCY / 2.f;
	POINT cursor = { cx, cy };
	ClientToScreen(g_hWnd, &cursor);
	SetCursorPos(cursor.x, cursor.y);

	//ȭ�� ȸ��
	float dx = CInputMgr::Get_Instance()->Get_DIMouseMove(MOUSEMOVESTATE::DIMS_X);
	float dy = CInputMgr::Get_Instance()->Get_DIMouseMove(MOUSEMOVESTATE::DIMD_Y);

	float sensitivity = 300.f;
	float rx = dx / sensitivity;
	float ry = dy / sensitivity;

	m_pTransform->Set_Angle(m_pTransform->Get_Angle() + _vec3{ ry, rx, 0.f });

}



void CMainPlayer::Update_State(const _float& fTimeDelta)
{
// 	switch (m_eCurState)
// 	{
// 	case PLAYER_STATE::PLAYER_IDLE:
// 		break;
// 	case PLAYER_STATE::PLAYER_MOVE:
// 		break;
// 	case PLAYER_STATE::PLAYER_JUMP:
// 		break;
// 	case PLAYER_STATE::PLAYER_FALL:
// 		break;
// 	}
// 
}

void CMainPlayer::Change_State(PLAYER_STATE eNewState)
{
	if (m_eCurState == eNewState)
		return;

	m_ePrevState = m_eCurState;
	m_eCurState = eNewState;
}

REGISTER_GAMEOBJECT(CMainPlayer)