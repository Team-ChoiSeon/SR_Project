#include "Engine_Define.h"
#include "CPostProcess.h"
#include "CShaderMgr.h"
#include "CTimeMgr.h"
#include "CSoundMgr.h"

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

	VTXTEX* pV = nullptr;

	float w = float(WINCX);
	float h = float(WINCY);

	m_pVB->Lock(0, 0, (void**)&pV, D3DLOCK_DISCARD);

	pV[0] = { {-1.f,    -1.f,     0.0f},   {0.0f, 1.0f } };          // LB
	pV[1] = { { -1.f,    1.f,   0.0f},  { 0.0f, 0.0f} };          // LT
	pV[2] = { { 1.f,   -1.f,     0.0f },{  1.0f, 1.0f } };          // RB
	pV[3] = { {1.f,   1.f,   0.0f},{ 1.0f, 0.0f} };          // RT

	m_pVB->Unlock();

	m_pEffect = CShaderMgr::Get_Instance()->GetShader(L"g_PostShader.fx");

	CSoundMgr::Get_Instance()->Load_Sound("glitch", "../Bin/Resource/Sound/camera.wav");

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
	// 백버퍼로 
	m_pDevice->SetRenderTarget(0, m_pBackBuffer);
	m_pEffect->SetTexture("g_SceneTex", m_pRenderTexture);
	m_pEffect->SetInt("g_EffectType", m_iEffectType);

	float dt = CTimeMgr::Get_Instance()->Get_TimeDelta(L"Timer_FPS");
	if (m_bEffect) {
		m_pEffect->SetFloat("g_Time", dt);
		m_pEffect->SetFloat("g_EffectTime", m_fDuration);
		m_pEffect->SetFloat("g_TotalTime", m_fTotal);
		m_fDuration -= dt * 1.5;
	}
	if (!m_bLoop) {
		if (m_fDuration < 0) {
			m_fDuration = 0;
			m_iEffectType = 0;
			m_bEffect = false;
		}
	}

	_matrix identity;
	D3DXMatrixIdentity(&identity);
	m_pEffect->SetMatrix("g_matIdentity", &identity);

	// 풀스크린 쿼드로 출력


	m_pDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_POINT);
	m_pDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_POINT);
	m_pDevice->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_NONE);

	m_pDevice->SetRenderState(D3DRS_ZENABLE, FALSE);
	m_pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
	m_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	m_pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	m_pDevice->SetFVF(FVF_TEX);
	m_pDevice->SetStreamSource(0, m_pVB, 0, sizeof(VTXTEX));

	UINT passCount = 0;
	m_pEffect->Begin(&passCount, 0);
	m_pEffect->BeginPass(m_iEffectType);
	m_pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
	m_pEffect->EndPass();
	m_pEffect->End();

	m_pDevice->SetRenderState(D3DRS_ZENABLE, TRUE);
	m_pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	m_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
}

void CPostProcess::Start_Glitch(_float duration)
{
	m_fDuration = duration;
	m_bEffect = true;
	m_iEffectType = 0;
	CSoundMgr::Get_Instance()->Play("glitch");
}

void CPostProcess::Start_Dead(_float duration)
{
	m_fDuration = duration;
	m_bEffect = true;
	m_iEffectType = 1;
	m_fTotal = duration;
}

void CPostProcess::Start_Alive(_float duration)
{
	m_fDuration = duration;
	m_bEffect = true;
	m_iEffectType = 2;
	m_fTotal = duration;
}

void CPostProcess::Do_Assemble(_bool Assemble)
{
	if (Assemble) {
		m_iEffectType = 4;
		m_bEffect = true;
		m_bLoop = true;
	}
	else {
		m_iEffectType = 4;
		m_fDuration = 1.f;
		m_fTotal = 1.f;
		m_bLoop = false;
	}
}

void CPostProcess::Set_Assemble()
{
	m_iEffectType = 3;
	m_fDuration = 8.f;
	m_fTotal = 8.f;
	m_bLoop = false;
	m_bEffect = true;
}

void CPostProcess::Free()
{
	Safe_Release(m_pRenderSurf);
	Safe_Release(m_pBackBuffer);
	Safe_Release(m_pRenderTexture);
	Safe_Release(m_pVB);
	Safe_Release(m_pDevice);
}