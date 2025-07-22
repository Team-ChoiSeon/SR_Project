#include "Engine_Define.h"
#include "CUiQuad.h"
#include "CRenderMgr.h"
#include "CTransform.h"
#include "CCameraMgr.h"
#include "CShaderMgr.h"
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
}


void CUiQuad::LateUpdate_Component(const _float& fTimeDelta)
{

	CRenderMgr::Get_Instance()->Add_UI(this);
}

void CUiQuad::Render(LPDIRECT3DDEVICE9 pDevice)
{
	if (!m_pTransform) return;

	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_ZENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	_matrix worldMat = *m_pTransform->Get_WorldMatrix();

	_matrix viewNoRot;
	D3DXMatrixIdentity(&viewNoRot);
	_matrix projMat;
	D3DXMatrixOrthoLH(&projMat, WINCX, WINCY, 0, 1);

	if (m_pEffect) {
		D3DXHANDLE hWorld = m_pEffect->GetParameterByName(nullptr, "g_matWorld");
		D3DXHANDLE hView = m_pEffect->GetParameterByName(nullptr, "g_matView");
		D3DXHANDLE hProj = m_pEffect->GetParameterByName(nullptr, "g_matProj");
		D3DXHANDLE hDiffuse = m_pEffect->GetParameterByName(nullptr, "g_DiffuseTex");
		D3DXHANDLE hAlpha = m_pEffect->GetParameterByName(nullptr, "g_Alpha");
		D3DXHANDLE hRatio = m_pEffect->GetParameterByName(nullptr, "g_Ratio");

		m_pEffect->SetMatrix(hWorld, &worldMat);
		m_pEffect->SetMatrix(hView, &viewNoRot);
		m_pEffect->SetMatrix(hProj, &projMat);
		_vec4 ClipRatio = { m_fRatio.x,m_fRatio.y,1.f,1.f };
		m_pEffect->SetVector(hRatio, &ClipRatio);

		if (m_pTexture)
			m_pEffect->SetTexture(hDiffuse, m_pTexture->Get_Texture());

		m_pEffect->SetFloat(hAlpha, m_fAlpha); // 혹은 UI 알파값
		m_pEffect->Begin(0, 0);
		m_pEffect->BeginPass(0);
			DrawQuad(pDevice);
		m_pEffect->EndPass();
		m_pEffect->End();
	}
	else {
		pDevice->SetTransform(D3DTS_WORLD, &worldMat);
		pDevice->SetTransform(D3DTS_VIEW, &viewNoRot);
		pDevice->SetTransform(D3DTS_PROJECTION, &projMat);

		if (m_pTexHandle)
			pDevice->SetTexture(0, m_pTexHandle);
		else if(m_pTexture)
			pDevice->SetTexture(0, m_pTexture->Get_Texture());

		DrawQuad(pDevice);

		_matrix matCamView = *(CCameraMgr::Get_Instance()->Get_MainViewMatrix());
		_matrix matCamProj = *(CCameraMgr::Get_Instance()->Get_MainProjectionMatrix());
		pDevice->SetTransform(D3DTS_VIEW, &matCamView);
		pDevice->SetTransform(D3DTS_PROJECTION, &matCamProj);
	}

	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ZENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
}

void CUiQuad::DrawQuad(LPDIRECT3DDEVICE9 pDevice)
{
	pDevice->SetStreamSource(0, m_pVB, 0, sizeof(VTXTEX));
	pDevice->SetFVF(FVF_TEX);
	pDevice->SetIndices(m_pIB);
	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 4, 0, 2);
}

void CUiQuad::Set_Texture(const wstring& key)
{
	m_pTexture = CResourceMgr::Get_Instance()->Load_Texture(key);
}

void CUiQuad::Set_Shader(const wstring& key)
{
	m_pEffect = CShaderMgr::Get_Instance()->GetShader(key);
}

void CUiQuad::Set_TextureHandle(LPDIRECT3DBASETEXTURE9 tex)
{
	m_pTexHandle = tex;
}


void CUiQuad::Free()
{
	CUI::Free();
	Safe_Release(m_pIB);
	Safe_Release(m_pVB);
	Safe_Release(m_pGraphicDev);
}
