#include "CRenderMgr.h"
#include "CCollisionMgr.h"

IMPLEMENT_SINGLETON(CRenderMgr)

CRenderMgr::CRenderMgr()
{
}

CRenderMgr::~CRenderMgr()
{
	Free();
}

HRESULT CRenderMgr::Ready_RenderMgr()
{
	m_vModellist.resize(static_cast<int>(RENDER_PASS::RP_END));
	Clear();
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
	
	for (auto& renderer : m_vModellist[static_cast<int>(RENDER_PASS::RP_SHADOW)])
		renderer->Render(pDevice);

	for (auto& renderer : m_vModellist[static_cast<int>(RENDER_PASS::RP_OPAQUE)])
	{
		renderer->Render(pDevice);
	}
		
	for (auto& renderer : m_vModellist[static_cast<int>(RENDER_PASS::RP_STENCIL)])
		renderer->Render(pDevice);

	for (auto& renderer : m_vModellist[static_cast<int>(RENDER_PASS::RP_TRANSPARENT)])
		renderer->Render(pDevice);

	for (auto& renderer : m_vModellist[static_cast<int>(RENDER_PASS::RP_UI)])
		renderer->Render(pDevice);

	for (auto& renderer : m_vUI)
		renderer->Render(pDevice);

	for (auto& renderer : m_vModellist[static_cast<int>(RENDER_PASS::RP_POSTPROCESS)])
		renderer->Render(pDevice);
	
	for (auto& renderer : m_vCol)
		renderer->Render(pDevice);


	Clear();
}

void CRenderMgr::Add_Model(CModel* model)
{
	OutputDebugString("[RenderMgr] Add_Model 호출됨\n");
	auto pass = static_cast<int>(model->Get_RenderPass());

	auto iter = find_if(m_vModellist[pass].begin(), m_vModellist[pass].end(),
		[&model](CModel* data)->bool {
			return data == model;
		});

	if (iter == m_vModellist[pass].end())
		m_vModellist[pass].push_back(model);
	OutputDebugString("[CRenderMgr] 모델 추가됨\n");
}

void CRenderMgr::Remove_Model(CModel* model)
{
	auto pass = static_cast<int>(model->Get_RenderPass());

	auto iter = remove_if(m_vModellist[pass].begin(), m_vModellist[pass].end(),
		[&model](CModel* data)->bool {
			return data == model;
		});

	if (iter != m_vModellist[pass].end()) {
		m_vModellist[pass].erase(iter, m_vModellist[pass].end());
	}
}


void CRenderMgr::Add_Collider(CCollider* collider)
{
	auto iter = find_if(m_vCol.begin(), m_vCol.end(),
		[&collider](CCollider* data)->bool {
			return data == collider;
		});

	if (iter == m_vCol.end())
		m_vCol.push_back(collider);
}

void CRenderMgr::Remove_Collider(CCollider* collider)
{
	auto iter = remove_if(m_vCol.begin(), m_vCol.end(),
		[&collider](CCollider* data)->bool {
			return data == collider;
		});

	if (iter != m_vCol.end()) {
		m_vCol.erase(iter, m_vCol.end());
	}
}

void CRenderMgr::Add_UI(CUI* ui)
{
	auto iter = find_if(m_vUI.begin(), m_vUI.end(),
		[&ui](CUI* data)->bool {
			return data == ui;
		});

	if (iter == m_vUI.end())
		m_vUI.push_back(ui);
}

void CRenderMgr::Remove_UI(CUI* ui)
{
	auto iter = remove_if(m_vUI.begin(), m_vUI.end(),
		[&ui](CUI* data)->bool {
			return data == ui;
		});

	if (iter != m_vUI.end()) {
		m_vUI.erase(iter, m_vUI.end());
	}
}


void CRenderMgr::Clear()
{
	for (auto& list : m_vModellist)
		list.clear();

	m_vCol.clear();
}



void CRenderMgr::Free()
{
}
