#pragma once
#include "pch.h"
#include "CPlayer.h"
#include "CInputMgr.h"
#include "Engine_Model.h"

CPlayer::CPlayer(LPDIRECT3DDEVICE9 pGraphicDev)
	: CGameObject(pGraphicDev)
{
}
CPlayer::~CPlayer()
{
}

HRESULT CPlayer::Ready_GameObject()
{
	OutputDebugString(L"[CPlayer] Ready_GameObject 시작\n");
	m_fSpeed = 10.f;

	Add_Component<CTransform>(ID_DYNAMIC, m_pGraphicDev);
	m_pTransform = Get_Component<CTransform>();
	m_pTransform->Ready_Transform();
	m_pTransform->Set_Pos({ 0.f, 0.f, -10.f });
	m_pTransform->Set_Look({ 0.f, 0.f, 1.f });
	m_pTransform->Set_Up({ 0.f, 1.f, 0.f });
	m_pTransform->Set_Right({ 1.f, 0.f, 0.f });

	DefaultCubeModel tModel;
	Add_Component<CModel>(ID_DYNAMIC, m_pGraphicDev, tModel);
	OutputDebugString(L"[CPlayer] Add_Component<CModel> 호출 완료\n");
	m_pModel = Get_Component<CModel>();

	if (m_pModel == nullptr)
		OutputDebugString(L"[CPlayer] m_pModel is nullptr\n");
	else
		OutputDebugString(L"[CPlayer] m_pModel is OK\n");

	Add_Component<CCollider>(ID_DYNAMIC, m_pGraphicDev);
	m_pCollider = Get_Component<CCollider>();

	Add_Component<CRigidbody>(ID_DYNAMIC, m_pGraphicDev, m_pTransform);
	m_pRigid = Get_Component<CRigidbody>();


	return S_OK;
}

int CPlayer::Update_GameObject(const _float& fTimeDelta)
{
	KeyInput(fTimeDelta);

	for (auto& pComponent : m_umComponent[ID_DYNAMIC])
		pComponent.second->Update_Component(fTimeDelta);

	// For velocity debug
	//wstring wDebug = to_wstring(m_pRigid->Get_Velocity().y);
	//OutputDebugString(wDebug.c_str());
	//OutputDebugString(L"\n");
	return S_OK;
}

void CPlayer::LateUpdate_GameObject(const _float& fTimeDelta)
{
	for (auto& pComponent : m_umComponent[ID_DYNAMIC])
		pComponent.second->LateUpdate_Component();
}

CPlayer* CPlayer::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CPlayer* pPlayer = new CPlayer(pGraphicDev);

	if (FAILED(pPlayer->Ready_GameObject()))
	{
		Safe_Release(pPlayer);
		MSG_BOX("Player Create Failed");
		return nullptr;
	}

	return pPlayer;
}

void CPlayer::Free()
{
	Safe_Release(m_pTransform);
	Safe_Release(m_pModel);
	Safe_Release(m_pCollider);
}

void CPlayer::KeyInput(const _float& fTimeDelta)
{
	if (CInputMgr::Get_Instance()->Key_Tap(DIK_TAB))
	{
		if (m_bCursorMove)
			m_bCursorMove = false;
		else
			m_bCursorMove = true;
	}

	if (!m_bCursorMove)
		CursorRotate();

	if (CInputMgr::Get_Instance()->Key_Hold(DIK_LSHIFT)) {
		m_fSpeed = 30.f;
	}
	if (CInputMgr::Get_Instance()->Key_Away(DIK_LSHIFT))
	{
		m_fSpeed = 10.f;
	}

	if (CInputMgr::Get_Instance()->Key_Down(DIK_W))
		m_pTransform->Move(DIR_FORWARD, m_fSpeed, fTimeDelta);
	
	if (CInputMgr::Get_Instance()->Key_Down(DIK_S))
		m_pTransform->Move(DIR_BACKWARD, m_fSpeed, fTimeDelta);

	if (CInputMgr::Get_Instance()->Key_Down(DIK_D))
		m_pTransform->Move(DIR_RIGHT, m_fSpeed, fTimeDelta);

	if (CInputMgr::Get_Instance()->Key_Down(DIK_A))
		m_pTransform->Move(DIR_LEFT, m_fSpeed, fTimeDelta);

	if (CInputMgr::Get_Instance()->Key_Down(DIK_Q))
		m_pTransform->Move(DIR_UP, m_fSpeed, fTimeDelta);

	if (CInputMgr::Get_Instance()->Key_Down(DIK_E))
		m_pTransform->Move(DIR_DOWN, m_fSpeed, fTimeDelta);
}

void CPlayer::CursorRotate()
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