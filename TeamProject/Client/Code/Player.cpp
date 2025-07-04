#include "pch.h"
#include "Player.h"
#include "CInputMgr.h"
#include "CVIBuffer.h"
#include "CCubeTex.h"
#include "CTransform.h"

Player::Player(LPDIRECT3DDEVICE9 pGraphicDev) : CGameObject(pGraphicDev), m_pVB(nullptr), m_pIB(nullptr)
{
	//m_pTransformCom->Create(pGraphicDev);
	Add_Component<CTransform>(ID_DYNAMIC,pGraphicDev);

	m_fMoveSpeed = 10.f;
}
Player::~Player()
{
	Safe_Release(m_pVB);
	Safe_Release(m_pIB);
}

HRESULT Player::Ready_GameObject()
{
	Get_Component<CTransform>()->Ready_Transform();
	
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

	//m_pVIBuffer->Render_Buffer();
	m_pGraphicDev->SetStreamSource(0, m_pVB, 0, m_dwVtxSize);
	m_pGraphicDev->SetFVF(m_dwFVF);
	m_pGraphicDev->SetIndices(m_pIB);
	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, FALSE);
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	m_pGraphicDev->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, m_dwVtxCnt, 0, m_dwTriCnt);
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
		Get_Component<CTransform>()->Set_Pos({
			Get_Component<CTransform>()->Get_Pos().x,
			Get_Component<CTransform>()->Get_Pos().y,
			Get_Component<CTransform>()->Get_Pos().z + m_fMoveSpeed * fTimeDelta });
	}
	if (CInputMgr::Get_Instance()->Key_Down(DIK_S)) {
		Get_Component<CTransform>()->Set_Pos({
			Get_Component<CTransform>()->Get_Pos().x,
			Get_Component<CTransform>()->Get_Pos().y,
			Get_Component<CTransform>()->Get_Pos().z - m_fMoveSpeed * fTimeDelta });
	}
	if (CInputMgr::Get_Instance()->Key_Down(DIK_D)) {
		Get_Component<CTransform>()->Set_Pos({
			Get_Component<CTransform>()->Get_Pos().x + m_fMoveSpeed * fTimeDelta ,
			Get_Component<CTransform>()->Get_Pos().y,
			Get_Component<CTransform>()->Get_Pos().z });
	}
	if (CInputMgr::Get_Instance()->Key_Down(DIK_A)) {
		Get_Component<CTransform>()->Set_Pos({
			Get_Component<CTransform>()->Get_Pos().x - m_fMoveSpeed * fTimeDelta ,
			Get_Component<CTransform>()->Get_Pos().y,
			Get_Component<CTransform>()->Get_Pos().z});
	}
}