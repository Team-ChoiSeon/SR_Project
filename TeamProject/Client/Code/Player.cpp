#include "pch.h"
#include "Player.h"
#include "CInputMgr.h"
#include "CVIBuffer.h"
#include "CCubeTex.h"
#include "CTransform.h"

Player::Player(LPDIRECT3DDEVICE9 pGraphicDev) : CGameObject(pGraphicDev), m_pVB(nullptr), m_pIB(nullptr)
{
	Add_Component<CTransform>(ID_DYNAMIC,pGraphicDev);
	m_pTransform = Get_Component<CTransform>();

	m_fMoveSpeed = 10.f;
	
}
Player::~Player()
{
}

HRESULT Player::Ready_GameObject()
{
	auto transform = Get_Component<CTransform>();
	transform->Ready_Transform();
	transform->Set_Pos({ 0.f, 0.f, -10.f });
	transform->Set_Look({ 0.f, 0.f, 1.f });
	transform->Set_Up({ 0.f, 1.f, 0.f });
	transform->Set_Right({ 1.f, 0.f, 0.f });
	//Get_Component<CTransform>(L"Transform")->Set_Scale({ 50.f, 50.f, 50.f });


	m_dwVtxCnt = 8;
	m_dwVtxSize = sizeof(VTXCOL);
	m_dwFVF = FVF_COL;
	m_dwTriCnt = 12;
	m_dwIdxSize = sizeof(INDEX32);
	m_IdxFmt = D3DFMT_INDEX32;

	//m_pVIBuffer->Ready_Buffer();


	if (FAILED(m_pGraphicDev->CreateVertexBuffer(m_dwVtxCnt * m_dwVtxSize,
		0, 
		m_dwFVF,
		D3DPOOL_MANAGED,
		&m_pVB,
		NULL)))
	{
		return E_FAIL;
	}

	if (FAILED(m_pGraphicDev->CreateIndexBuffer(m_dwTriCnt * m_dwIdxSize,
		0,      
		m_IdxFmt,
		D3DPOOL_MANAGED, 
		&m_pIB,
		NULL)))
	{
		return E_FAIL;
	}


	m_pVB->Lock(0, 0, (void**)&vertecies, 0);

	vertecies[0].vPosition = { -0.5f, 0.5f, -0.5f };
	vertecies[0].dwColor = D3DCOLOR_RGBA(255, 255, 255, 255);

	vertecies[1].vPosition = { 0.5f, 0.5f, -0.5f };
	vertecies[1].dwColor = D3DCOLOR_RGBA(255, 255, 255, 255);

	vertecies[2].vPosition = { 0.5f, -0.5f, -0.5f };
	vertecies[2].dwColor = D3DCOLOR_RGBA(255, 255, 255, 255);

	vertecies[3].vPosition = { -0.5f, -0.5f, -0.5f };
	vertecies[3].dwColor = D3DCOLOR_RGBA(255, 255, 255, 255);

	vertecies[4].vPosition = { -0.5f, 0.5f, 0.5f };
	vertecies[4].dwColor = D3DCOLOR_RGBA(255, 255, 255, 255);

	vertecies[5].vPosition = { 0.5f, 0.5f, 0.5f };
	vertecies[5].dwColor = D3DCOLOR_RGBA(255, 255, 255, 255);

	vertecies[6].vPosition = { 0.5f, -0.5f, 0.5f };
	vertecies[6].dwColor = D3DCOLOR_RGBA(255, 255, 255, 255);

	vertecies[7].vPosition = { -0.5f, -0.5f, 0.5f };
	vertecies[7].dwColor = D3DCOLOR_RGBA(255, 255, 255, 255);


	m_pVB->Unlock();

	m_pIB->Lock(0, 0, (void**)&indecies, 0);

	//정면
	indecies[0] = { 0, 1, 2 };
	indecies[1] = { 0, 2, 3 };

	//후면
	indecies[2] = { 5, 4, 7 };
	indecies[3] = { 5, 7, 6 };

	//좌측면
	indecies[4] = { 4, 0, 3 };
	indecies[5] = { 4, 3, 7 };

	//우측면
	indecies[6] = { 2, 5, 6 };
	indecies[7] = { 2, 6, 3 };

	//윗면
	indecies[8] = { 4, 5, 1 };
	indecies[9] = { 4, 1, 0 };

	//아래면
	indecies[10] = { 3, 2, 6 };
	indecies[11] = { 3, 6, 7 };

	m_pIB->Unlock();

	return S_OK;
}

int Player::Update_GameObject(const _float& fTimeDelta)
{
	KeyInput(fTimeDelta);
	for (auto& pComponent : m_umComponent[ID_DYNAMIC])
		pComponent.second->Update_Component(fTimeDelta);
	return S_OK;
}

void Player::LateUpdate_GameObject(const _float& fTimeDelta)
{
	for (auto& pComponent : m_umComponent[ID_DYNAMIC])
		pComponent.second->LateUpdate_Component();
}

void Player::Render_GameObject()
{
	m_mWorld = Get_Component<CTransform>()->Get_WorldMatrix();
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_mWorld);

	m_pGraphicDev->SetStreamSource(0, m_pVB, 0, m_dwVtxSize);
	m_pGraphicDev->SetFVF(m_dwFVF);
	m_pGraphicDev->SetIndices(m_pIB);
	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, FALSE);
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	m_pGraphicDev->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, m_dwVtxCnt, 0, m_dwTriCnt);
}

Player* Player::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	Player* pPlayer = new Player(pGraphicDev);

	if (FAILED(pPlayer->Ready_GameObject()))
	{
		Safe_Release(pPlayer);
		MSG_BOX("Player Create Failed");
		return nullptr;
	}

	return pPlayer;
}

void Player::Free()
{
	Safe_Release(m_pVB);
	Safe_Release(m_pIB);
	Safe_Release(m_pTransform);

}

void Player::KeyInput(const _float& fTimeDelta)
{
	if (CInputMgr::Get_Instance()->Key_Hold(DIK_LSHIFT)) {
		m_fMoveSpeed = 30.f;
	}
	if (CInputMgr::Get_Instance()->Key_Away(DIK_LSHIFT))
	{
		m_fMoveSpeed = 10.f;
	}
	if (CInputMgr::Get_Instance()->Key_Down(DIK_W)) {
		m_pTransform->Set_Pos(m_pTransform->Get_Pos() + m_pTransform->Get_Info(INFO_LOOK) * m_fMoveSpeed * fTimeDelta);
	}
	if (CInputMgr::Get_Instance()->Key_Down(DIK_S)) {
		m_pTransform->Set_Pos(m_pTransform->Get_Pos() - m_pTransform->Get_Info(INFO_LOOK) * m_fMoveSpeed * fTimeDelta);
	}
	if (CInputMgr::Get_Instance()->Key_Down(DIK_D)) {
		m_pTransform->Set_Pos(m_pTransform->Get_Pos() + m_pTransform->Get_Info(INFO_RIGHT) * m_fMoveSpeed * fTimeDelta);
	}
	if (CInputMgr::Get_Instance()->Key_Down(DIK_A)) {
		m_pTransform->Set_Pos(m_pTransform->Get_Pos() - m_pTransform->Get_Info(INFO_RIGHT) * m_fMoveSpeed * fTimeDelta);
	}
	if (CInputMgr::Get_Instance()->Key_Down(DIK_Q)) {
		m_pTransform->Set_Pos(m_pTransform->Get_Pos() + m_pTransform->Get_Info(INFO_UP) * m_fMoveSpeed * fTimeDelta);
	}
	if (CInputMgr::Get_Instance()->Key_Down(DIK_E)) {
		m_pTransform->Set_Pos(m_pTransform->Get_Pos() - m_pTransform->Get_Info(INFO_UP) * m_fMoveSpeed * fTimeDelta);
	}
}