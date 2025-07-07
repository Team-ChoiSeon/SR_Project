#include "pch.h"
#include "CMainPlayer.h"
#include "CInputMgr.h"
#include "CVIBuffer.h"
#include "CCubeTex.h"
#include "CTransform.h"
#include "CCollider.h"

CMainPlayer::CMainPlayer(LPDIRECT3DDEVICE9 pGraphicDev)
	: CGameObject(pGraphicDev)
{
}
CMainPlayer::~CMainPlayer()
{
}

HRESULT CMainPlayer::Ready_GameObject()
{
	DefaultCubeModel tModel;
	Add_Component<CTransform>(ID_DYNAMIC, m_pGraphicDev);
	Add_Component<CCollider>(ID_DYNAMIC, m_pGraphicDev);
	Add_Component<CModel>(ID_DYNAMIC, m_pGraphicDev, tModel);
	m_pModel = Get_Component<CModel>();
	m_pCollider = Get_Component<CCollider>();
	m_pTransform = Get_Component<CTransform>();

	m_pTransform->Set_Pos({ 0.f, 0.f, -10.f });
	m_pTransform->Set_Look({ 0.f, 0.f, 1.f });
	m_pTransform->Set_Up({ 0.f, 1.f, 0.f });
	m_pTransform->Set_Right({ 1.f, 0.f, 0.f });
	m_fMoveSpeed = 10.f;


	Add_Component<CRigidbody>(ID_DYNAMIC, m_pGraphicDev, m_pTransform);
	m_pRigid = Get_Component<CRigidbody>();

	return S_OK;
}

int CMainPlayer::Update_GameObject(const _float& fTimeDelta)
{
	KeyInput(fTimeDelta);

	Player_Jump(fTimeDelta);

	Set_GroundCheck();

	for (auto& pComponent : m_umComponent[ID_DYNAMIC])
		pComponent.second->Update_Component(fTimeDelta);
	return S_OK;
}

void CMainPlayer::LateUpdate_GameObject(const _float& fTimeDelta)
{
	for (auto& pComponent : m_umComponent[ID_DYNAMIC])
		pComponent.second->LateUpdate_Component();
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
	if (CInputMgr::Get_Instance()->Key_Tap(DIK_TAB))
		m_bCursorMove = !m_bCursorMove;

	if (m_bCursorMove)
		CursorRotate();

	if (CInputMgr::Get_Instance()->Key_Hold(DIK_LSHIFT)) {
		m_fMoveSpeed = 30.f;
	}

	// 홀드 처리
	if (CInputMgr::Get_Instance()->Mouse_Hold(DIM_LB)) {
		m_bObjHold = true;
	}
	else {
		m_bObjHold = false;
	}

	if (CInputMgr::Get_Instance()->Key_Away(DIK_LSHIFT))
	{
		m_fMoveSpeed = 10.f;
	}
	if (CInputMgr::Get_Instance()->Key_Down(DIK_W)) {
		m_pTransform->Move(DIR_FORWARD, m_fMoveSpeed, fTimeDelta);
		//m_pTransform->Set_Pos(m_pTransform->Get_Pos() + m_pTransform->Get_Info(INFO_LOOK) * m_fMoveSpeed * fTimeDelta);
	}
	if (CInputMgr::Get_Instance()->Key_Down(DIK_S)) {
		m_pTransform->Move(DIR_BACKWARD, m_fMoveSpeed, fTimeDelta);
		//m_pTransform->Set_Pos(m_pTransform->Get_Pos() - m_pTransform->Get_Info(INFO_LOOK) * m_fMoveSpeed * fTimeDelta);
	}
	if (CInputMgr::Get_Instance()->Key_Down(DIK_D)) {
		m_pTransform->Move(DIR_RIGHT, m_fMoveSpeed, fTimeDelta);
		// m_pTransform->Set_Pos(m_pTransform->Get_Pos() + m_pTransform->Get_Info(INFO_RIGHT) * m_fMoveSpeed * fTimeDelta);
	}
	if (CInputMgr::Get_Instance()->Key_Down(DIK_A)) {
		m_pTransform->Move(DIR_LEFT, m_fMoveSpeed, fTimeDelta);
		// m_pTransform->Set_Pos(m_pTransform->Get_Pos() - m_pTransform->Get_Info(INFO_RIGHT) * m_fMoveSpeed * fTimeDelta);
	}
	if (CInputMgr::Get_Instance()->Key_Down(DIK_Q)) {
		m_pTransform->Move(DIR_UP, m_fMoveSpeed, fTimeDelta);
		//m_pTransform->Set_Pos(m_pTransform->Get_Pos() + m_pTransform->Get_Info(INFO_UP) * m_fMoveSpeed * fTimeDelta);
	}
	if (CInputMgr::Get_Instance()->Key_Down(DIK_E)) {
		m_pTransform->Move(DIR_DOWN, m_fMoveSpeed, fTimeDelta);
		//m_pTransform->Set_Pos(m_pTransform->Get_Pos() - m_pTransform->Get_Info(INFO_UP) * m_fMoveSpeed * fTimeDelta);
	}

	// 임시 점프 처리
	if (CInputMgr::Get_Instance()->Key_Down(DIK_SPACE)) {
		if (m_bGround && !m_bJump)
		{
			m_bJump = true;
			m_fVelocityY = m_fJumpPower;
		}
	}
	
}

void CMainPlayer::CursorRotate()
{
	//커서 고정
	ShowCursor(false);
	float cx = WINCX / 2.f;
	float cy = WINCY / 2.f;
	POINT cursor = { cx, cy };
	ClientToScreen(g_hWnd, &cursor);
	SetCursorPos(cursor.x, cursor.y);

	//화면 회전
	float dx = CInputMgr::Get_Instance()->Get_DIMouseMove(MOUSEMOVESTATE::DIMS_X);
	float dy = CInputMgr::Get_Instance()->Get_DIMouseMove(MOUSEMOVESTATE::DIMD_Y);

	float sensitivity = 300.f;
	float rx = dx / sensitivity;
	float ry = dy / sensitivity;

	m_pTransform->Set_Angle(m_pTransform->Get_Angle() + _vec3{ ry, rx, 0.f });

}

void CMainPlayer::Player_Jump(const _float& fTimeDelta)
{

	if (!m_bGround || m_bJump) {
		m_fVelocityY -= m_fGravity * fTimeDelta; 
		m_pTransform->Set_Pos(m_pTransform->Get_Pos() + m_pTransform->Get_Info(INFO_UP) * m_fVelocityY * fTimeDelta);
	}
}

void CMainPlayer::Set_GroundCheck()
{
	const float groundY = -5.f;

	_vec3 pos = m_pTransform->Get_Pos();

	if (pos.y <= groundY)
	{
		pos.y = groundY;
		m_fVelocityY = 0.f;
		m_bGround = true;
		m_bJump = false;
	}
	else
	{
		m_bGround = false;
	}

	m_pTransform->Set_Pos(pos);
}