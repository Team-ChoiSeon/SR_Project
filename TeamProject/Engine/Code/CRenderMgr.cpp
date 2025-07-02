#include "CRenderMgr.h"

IMPLEMENT_SINGLETON(CRenderMgr)

CRenderMgr::CRenderMgr()
{
}

CRenderMgr::~CRenderMgr()
{
	Free();
}

HRESULT CRenderMgr::Ready_RenderMgr(LPDIRECT3DDEVICE9 pDevice)
{
	return S_OK;
}

void CRenderMgr::Render(LPDIRECT3DDEVICE9 pDevice)
{
	//렌더 스테이트 설정
	pDevice->SetRenderState(D3DRS_ZENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	//pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	pDevice->SetRenderState(D3DRS_NORMALIZENORMALS, TRUE);		// 정규화된 노멀 사용
	pDevice->SetRenderState(D3DRS_SPECULARENABLE, TRUE);			// 스페큘러 하이라이트

	//텍스쳐 설정
	pDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	pDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	pDevice->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);

	pDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
	pDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);
	//pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);

	for (auto& renderer : m_Modellist[RENDER_PASS::RP_SHADOW])
		renderer->Render(pDevice);

	for (auto& renderer : m_Modellist[RENDER_PASS::RP_OPAQUE])
		renderer->Render(pDevice);

	for (auto& renderer : m_Modellist[RENDER_PASS::RP_STENCIL])
		renderer->Render(pDevice);

	for (auto& renderer : m_Modellist[RENDER_PASS::RP_TRANSPARENT])
		renderer->Render(pDevice);

	for (auto& renderer : m_Modellist[RENDER_PASS::RP_UI])
		renderer->Render(pDevice);

	for (auto& renderer : m_Modellist[RENDER_PASS::RP_POSTPROCESS])
		renderer->Render(pDevice);

	Clear();
}

void CRenderMgr::Add_Model(CModel* model)
{
	RENDER_PASS pass = model->Get_RenderPass();

	auto iter = find_if(m_Modellist[pass].begin(), m_Modellist[pass].end(),
		[&model](CModel* data)->bool {
			return data == model;
		});

	if (iter == m_Modellist[pass].end())
		m_Modellist[pass].push_back(model);
}

void CRenderMgr::Remove_Model(CModel* model)
{
	RENDER_PASS pass = model->Get_RenderPass();

	auto iter = remove_if(m_Modellist[pass].begin(), m_Modellist[pass].end(),
		[&model](CModel* data)->bool {
			return data == model;
		});

	if (iter != m_Modellist[pass].end()) {
		m_Modellist[pass].erase(iter, m_Modellist[pass].end());
	}
}

void CRenderMgr::Clear()
{
	m_Modellist.clear();
}

void CRenderMgr::Free()
{
}
