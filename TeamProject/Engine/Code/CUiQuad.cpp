#include "Engine_Define.h"
#include "CUiQuad.h"
#include "CRenderMgr.h"
#include "CTransform.h"

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

	constexpr int s = 1.f;
	
	vector<VTXTEX> vertices;
	vertices.push_back({ {-s,-s,0} ,{0,1} }); //0
	vertices.push_back({ {-s,+s,0} ,{0,0} }); //1
	vertices.push_back({ {+s,+s,0} ,{1,0} }); //2
	vertices.push_back({ {+s,-s,0} ,{1,1} }); //3

	//정점 버퍼 만들기
	void* pb;

	m_pVB->Lock(0, 0, &pb, 0);
	memcpy(pb, vertices.data(), vertices.size()*sizeof(VTXTEX));
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
	if (!m_pTransform) {
		m_pTransform = m_pOwner->Get_Component<CTransform>();
	}
}

void CUiQuad::LateUpdate_Component(const _float& fTimeDelta)
{
	CRenderMgr::Get_Instance()->Add_UI(this);
}

void CUiQuad::Render(LPDIRECT3DDEVICE9 pDevice)
{
	if (!m_pTransform) return;
	
	pDevice->SetTransform(D3DTS_WORLD, m_pTransform->Get_WorldMatrix());

	if(m_pTexture)
		pDevice->SetTexture(0, m_pTexture->Get_Texture());
	pDevice->SetStreamSource(0, m_pVB, 0, sizeof(VTXTEX)*4);
	pDevice->SetFVF(FVF_TEX);
	pDevice->SetIndices(m_pIB);
	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 4, 0, 6);
}

void CUiQuad::Free()
{
	CUI::Free();
}
