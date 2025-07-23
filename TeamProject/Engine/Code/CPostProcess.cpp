#include "Engine_Define.h"
#include "CPostProcess.h"


CPostProcess::CPostProcess(LPDIRECT3DDEVICE9 pDevice)
	:m_pDevice(pDevice)
{
	m_pDevice->AddRef();
}

CPostProcess::~CPostProcess()
{
}

CPostProcess* CPostProcess::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CPostProcess* instance = new CPostProcess(pDevice);

	if (FAILED(instance->Ready_Process())) {
		Safe_Release(instance);
		instance = nullptr;
	}

	return instance;
}

HRESULT CPostProcess::Ready_Process()
{
	m_pDevice->CreateTexture(
		WINCX, WINCY,
		1, D3DUSAGE_RENDERTARGET,
		D3DFMT_A8R8G8B8,
		D3DPOOL_DEFAULT,
		&m_pRenderTexture,
		NULL
	);


	m_pRenderTexture->GetSurfaceLevel(0, &m_pRenderSurf);

	HRESULT hr = m_pDevice->CreateVertexBuffer(
		sizeof(VTXTEX) * 4,
		D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY,
		FVF_TEX,
		D3DPOOL_DEFAULT,
		&m_pVB,
		nullptr);


	VTXTEX* pVertices = nullptr;

	m_pVB->Lock(0, 0, (void**)&pVertices, D3DLOCK_DISCARD);

	pVertices[0] = { { -1, -1, 0 }, {0.f, 1.f} };  // LB
	pVertices[1] = { { -1 , 1, 0},  {0.f, 0.f} };  // LT
	pVertices[2] = { { 1, 1, 0},  {1.f, 0.f} };  // RT
	pVertices[3] = { { 1, -1 , 0},  {1.f, 1.f} };  // RB

	m_pVB->Unlock();
	return S_OK;
}

void CPostProcess::BeginScene()
{
	m_pDevice->GetRenderTarget(0, &m_pBackBuffer); // 백버퍼 저장
	m_pDevice->SetRenderTarget(0, m_pRenderSurf);
	m_pDevice->Clear(0, nullptr, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_ARGB(0, 0, 0, 0), 1.0f, 0);
}

void CPostProcess::EndScene()
{
	// 백버퍼로 다시 전환
	m_pDevice->SetRenderTarget(0, m_pBackBuffer);

	// 후처리 셰이더에 씬 텍스처 전달
	m_pEffect->SetTexture("g_SceneTex", m_pRenderTexture);
	m_pEffect->SetInt("g_EffectType", m_iEffectType);

	// 풀스크린 쿼드로 출력
	m_pDevice->SetFVF(FVF_TEX);
	m_pDevice->SetStreamSource(0, m_pVB, 0, sizeof(VTXTEX));

	UINT passCount = 0;
	m_pEffect->Begin(&passCount, 0);
	for (UINT i = 0; i < passCount; ++i)
	{
		m_pEffect->BeginPass(i);
		m_pDevice->DrawPrimitive(D3DPT_TRIANGLEFAN, 0, 2);
		m_pEffect->EndPass();
	}
	m_pEffect->End();
}

void CPostProcess::Free()
{
	Safe_Release(m_pRenderSurf);
	Safe_Release(m_pBackBuffer);
	Safe_Release(m_pRenderTexture);
	Safe_Release(m_pVB);
	Safe_Release(m_pDevice);
}