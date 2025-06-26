#include "pch.h"
#include "Player.h"
#include "CInputMgr.h"
#include "CVIBuffer.h"
#include "CCubeTex.h"

Player::Player(LPDIRECT3DDEVICE9 pGraphicDev) : CGameObject(pGraphicDev), m_pVB(nullptr), m_pIB(nullptr)
{
	m_vScale = { 1.f, 1.f, 1.f };
	m_vRotation = { 0.f, 0.f, 0.f };
	m_vTranslation = { 0.f, 0.f, 0.f };
	m_fMoveSpeed = 5.f;

	D3DXMatrixIdentity(&m_mScale);
	D3DXMatrixIdentity(&m_mRotation);
	D3DXMatrixIdentity(&m_mTranslation);
}

Player::~Player()
{
	Safe_Release(m_pVB);
	Safe_Release(m_pIB);
}

HRESULT Player::Ready_GameObject()
{
	m_dwVtxCnt = 8;
	m_dwVtxSize = sizeof(VTXCOL);
	m_dwFVF = FVF_COL;
	m_dwTriCnt = 12;
	m_dwIdxSize = sizeof(INDEX32);
	m_IdxFmt = D3DFMT_INDEX32;

	if (FAILED(m_pGraphicDev->CreateVertexBuffer(m_dwVtxCnt * m_dwVtxSize, // 버텍스 버퍼 크기
		0,      // 버퍼 속성(0인 경우 정적 버퍼)
		m_dwFVF,    // 버텍스 속성
		D3DPOOL_MANAGED, // 정적 버퍼일 경우 MANAGED
		&m_pVB, // 생성한 버텍스 버퍼를 정보를 저장할 객체 주소
		NULL)))
	{
		return E_FAIL;
	}

	if (FAILED(m_pGraphicDev->CreateIndexBuffer(m_dwTriCnt * m_dwIdxSize, // 인덱스 버퍼 크기
		0,      // 버퍼 속성(0인 경우 정적 버퍼)
		m_IdxFmt,    // 인덱스 속성
		D3DPOOL_MANAGED, // 정적 버퍼일 경우 MANAGED
		&m_pIB, // 생성한 인덱스 버퍼를 정보를 저장할 객체 주소
		NULL)))
	{
		return E_FAIL;
	}


	m_pVB->Lock(0, 0, (void**)&vertecies, 0);

	vertecies[0].vPosition = { -0.5f, 0.5f, -0.5f };
	vertecies[0].dwColor = D3DCOLOR_RGBA(127, 127, 127, 255);

	vertecies[1].vPosition = { 0.5f, 0.5f, -0.5f };
	vertecies[1].dwColor = D3DCOLOR_RGBA(127, 127, 127, 255);

	vertecies[2].vPosition = { 0.5f, -0.5f, -0.5f };
	vertecies[2].dwColor = D3DCOLOR_RGBA(127, 127, 127, 255);

	vertecies[3].vPosition = { -0.5f, -0.5f, -0.5f };
	vertecies[3].dwColor = D3DCOLOR_RGBA(127, 127, 127, 255);

	vertecies[4].vPosition = { -0.5f, 0.5f, 0.5f };
	vertecies[4].dwColor = D3DCOLOR_RGBA(127, 127, 127, 255);

	vertecies[5].vPosition = { 0.5f, 0.5f, 0.5f };
	vertecies[5].dwColor = D3DCOLOR_RGBA(127, 127, 127, 255);

	vertecies[6].vPosition = { 0.5f, -0.5f, 0.5f };
	vertecies[6].dwColor = D3DCOLOR_RGBA(127, 127, 127, 255);

	vertecies[7].vPosition = { -0.5f, -0.5f, 0.5f };
	vertecies[7].dwColor = D3DCOLOR_RGBA(127, 127, 127, 255);

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


	D3DXMATRIX matView, matProj;
	_vec3 eye = { 0.f, 20.f, 0.f };
	_vec3 at = { 0.f, 0.f, 0.f };
	_vec3 up = { 0.f, 0.f, 1.f };

	float fov = (D3DX_PI * 0.25f);
	float aspect = WINCX / (WINCY * 1.f);
	float zn = 0.1f;
	float zf = 1000.f;
	D3DXMatrixLookAtLH(&matView, &eye, &at, &up);
	m_pGraphicDev->SetTransform(D3DTS_VIEW, &matView);
	D3DXMatrixPerspectiveFovLH(&matProj, fov, aspect, zn, zf);
	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &matProj);

	return S_OK;
}

int Player::Update_GameObject(const _float& fTimeDelta)
{
	KeyInput(fTimeDelta);
	return int();
}

void Player::LateUpdate_GameObject(const _float& fTimeDelta)
{
}

void Player::Render_GameObject()
{
	ComputeWorldMatirx();
	m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_mWorld);

	m_pGraphicDev->SetStreamSource(0, m_pVB, 0, m_dwVtxSize);
	m_pGraphicDev->SetFVF(m_dwFVF);
	m_pGraphicDev->SetIndices(m_pIB);
	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, FALSE);
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	m_pGraphicDev->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, m_dwVtxCnt, 0, m_dwTriCnt);


}

void Player::ComputeWorldMatirx()
{
	D3DXMatrixScaling(&m_mScale, m_vScale.x, m_vScale.y, m_vScale.z);
	D3DXMatrixRotationYawPitchRoll(&m_mRotation, m_vRotation.y, m_vRotation.x, m_vRotation.z);
	D3DXMatrixTranslation(&m_mTranslation, m_vTranslation.x, m_vTranslation.y, m_vTranslation.z);

	m_mWorld = m_mScale * m_mRotation * m_mTranslation;
}

void Player::KeyInput(const _float& fTimeDelta)
{
	if (CInputMgr::GetInstance()->Key_Hold(DIK_LSHIFT)) {
		m_fMoveSpeed = 50.f;
	}
	if (CInputMgr::GetInstance()->Key_Away(DIK_LSHIFT))
	{
		m_fMoveSpeed = 5.f;
	}
	if (CInputMgr::GetInstance()->Key_Down(DIK_W)) {
		m_vTranslation.z += m_fMoveSpeed * fTimeDelta;
	}
	if (CInputMgr::GetInstance()->Key_Down(DIK_S)) {
		m_vTranslation.z -= m_fMoveSpeed * fTimeDelta;
	}
	if (CInputMgr::GetInstance()->Key_Down(DIK_D)) {
		m_vTranslation.x += m_fMoveSpeed * fTimeDelta;
	}
	if (CInputMgr::GetInstance()->Key_Down(DIK_A)) {
		m_vTranslation.x -= m_fMoveSpeed * fTimeDelta;
	}
}

//이거는 좀 자연스러운 현상임
//왜냐면 일단 우리가 객체들이 동적할당 받을 것이 없어 원래라면은//그래서 그냥 자동으로 프리되면 컴포넌트들이 사라지게 햇는데
//지금 상황이 컴포넌트로 못하니까 직접 할당을 받았자나? 그래서 지금 해제가 안되고 있었던 거야.

//이건 내일 내가 성빈이형이랑 수정을 좀 해둘게 free_comp 로  만들고 순수가상으로 돌려둘게