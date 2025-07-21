#include "Engine_Define.h"
#include "CUiQuad.h"
#include "CRenderMgr.h"
#include "CTransform.h"
#include "CCameraMgr.h"

CUiQuad::CUiQuad(LPDIRECT3DDEVICE9 pGraphicDev)
 : CUI(pGraphicDev),m_pDevice(pGraphicDev)
{

}

CUiQuad::~CUiQuad()
{
}

CUiQuad* CUiQuad::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CUiQuad* instance = new CUiQuad(pGraphicDev);

	if (FAILED(instance->Ready_Component())) {
		Safe_Release(instance);
		instance = nullptr;
	}
	return instance;
}

HRESULT CUiQuad::Ready_Component()
{
	//tagVertexTexture VTXTEX 사용
	HRESULT hr = m_pDevice->CreateVertexBuffer(
		sizeof(VTXTEX) * 4,
		D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY,
		FVF_TEX,
		D3DPOOL_DEFAULT,
		&m_pVB,
		nullptr);

	if (FAILED(hr)) return E_FAIL;

	VTXTEX* pVertices = nullptr;

	m_pVB->Lock(0, 0, (void**)&pVertices, D3DLOCK_DISCARD);

	pVertices[0] = { { -1, -1, 0 }, {0.f, 1.f} };  // LB
	pVertices[1] = { { -1 , 1, 0},  {0.f, 0.f} };  // LT
	pVertices[2] = { { 1, 1, 0},  {1.f, 0.f} };  // RT
	pVertices[3] = { { 1, -1 , 0},  {1.f, 1.f} };  // RB

	m_pVB->Unlock();

	hr = m_pDevice->CreateIndexBuffer(6 * sizeof(INDEX32),
		0, D3DFMT_INDEX32, D3DPOOL_MANAGED,
		&m_pIB, 0);

	if (FAILED(hr)) return E_FAIL;

	void* ib;
	vector<DWORD> indices;
	indices.push_back(0);
	indices.push_back(1);
	indices.push_back(2);
	indices.push_back(0);
	indices.push_back(2);
	indices.push_back(3);

	m_pIB->Lock(0, 0, &ib, 0);
	memcpy(ib, indices.data(), indices.size() * sizeof(DWORD));
	m_pIB->Unlock();

	return hr;
}

void CUiQuad::Update_Component(const _float& fTimeDelta)
{
	if (!m_pTransform)
		m_pTransform = m_pOwner->Get_Component<CTransform>();

	CGameObject* mainCam = CCameraMgr::Get_Instance()->Get_MainCamera();
	CCamera* pCam = mainCam->Get_Component<CCamera>();
	
	_vec3 right = pCam->Get_Info(INFO_RIGHT); // 화면 기준 X축
	_vec3 up = pCam->Get_Info(INFO_UP);    // 화면 기준 Y축

	D3DXVec3Normalize(&right, &right);
	D3DXVec3Normalize(&up, &up);
 	_vec3 pos = m_pTransform->Get_Pos();
	
}


void CUiQuad::LateUpdate_Component(const _float& fTimeDelta)
{

	CRenderMgr::Get_Instance()->Add_UI(this);
}

void CUiQuad::Render(LPDIRECT3DDEVICE9 pDevice)
{
	if (!m_pTransform) return;

	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	_matrix viewNoRot;
	D3DXMatrixIdentity(&viewNoRot);
	
	pDevice->SetTransform(D3DTS_WORLD, &viewNoRot);

	_matrix camView = *(CCameraMgr::Get_Instance()->Get_MainViewMatrix());

	 viewNoRot = camView;
	//회전은 정점으로 주자.
	// 회전 성분 제거 → 단위 행렬로
	viewNoRot._11 = 1.f; viewNoRot._12 = 0.f; viewNoRot._13 = 0.f;
	viewNoRot._21 = 0.f; viewNoRot._22 = 1.f; viewNoRot._23 = 0.f;
	viewNoRot._31 = 0.f; viewNoRot._32 = 0.f; viewNoRot._33 = 1.f;

	pDevice->SetTransform(D3DTS_VIEW, &camView);
	// pDevice에 세팅


	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_ZENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);


	if(m_pTexture)
		pDevice->SetTexture(0, m_pTexture->Get_Texture());
	pDevice->SetStreamSource(0, m_pVB, 0, sizeof(VTXTEX));
	pDevice->SetFVF(FVF_TEX);
	pDevice->SetIndices(m_pIB);

	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 4, 0, 2);
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ZENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);

}

void CUiQuad::Set_Texture(const wstring& key)
{
	m_pTexture = CResourceMgr::Get_Instance()->Load_Texture(key);
}

void CUiQuad::Set_QuadPos(_vec3 pos, _vec2 scale)
{
	VTXTEX* pVertices = nullptr;

	if (FAILED(m_pVB->Lock(0, 0, (void**)&pVertices, D3DLOCK_DISCARD)))
		return;

	// 1. 카메라 방향 벡터 가져오기
	CCamera* pCam = CCameraMgr::Get_Instance()->Get_MainCamera()->Get_Component<CCamera>();
	const D3DXVECTOR3& camRight = pCam->Get_Right();  // x축
	const D3DXVECTOR3& camUp = pCam->Get_Up();     // y축

	// 2. 크기에 맞게 스케일링
	D3DXVECTOR3 vRight = camRight * (scale.x * 0.5f);
	D3DXVECTOR3 vUp = camUp * (scale.y * 0.5f);


	// 정점 순서: LT, LB, RB, RT (시계 or 반시계는 렌더 상태 맞춰서)
	pVertices[0] = { pos - vRight - vUp, {0.f, 1.f} }; // LB
	pVertices[1] = { pos - vRight + vUp, {0.f, 0.f} }; // LT
	pVertices[2] = { pos + vRight + vUp, {1.f, 0.f} }; // RT
	pVertices[3] = { pos + vRight - vUp, {1.f, 1.f} }; // RB

	m_pVB->Unlock();
}


void CUiQuad::Free()
{
	CUI::Free();
	Safe_Release(m_pIB);
	Safe_Release(m_pVB);
	Safe_Release(m_pGraphicDev);
}
