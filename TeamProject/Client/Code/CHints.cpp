#include "pch.h"
#include "Engine_Define.h"
#include "CHints.h"
#include "CUiQuad.h"
#include "CTransform.h"

CHints::CHints(LPDIRECT3DDEVICE9 pGraphicDev)
	:CGameObject(pGraphicDev)
{
}

CHints::~CHints()
{
}

CHints* CHints::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CHints* instance = new CHints(pGraphicDev);

	if (FAILED(instance->Ready_GameObject())) {
		Safe_Release(instance);
		instance = nullptr;
	}

	return instance;
}

HRESULT CHints::Ready_GameObject()
{
	m_pFont = CResourceMgr::Get_Instance()->Get_Font(L"나눔큰");

	m_pQuad = Add_Component<CUiQuad>(ID_DYNAMIC, m_pGraphicDev);
	m_pTransform = Add_Component<CTransform>(ID_DYNAMIC, m_pGraphicDev);
	m_tPanel.Set_Size({ WINCX,50 });
	m_tPanel.Set_Pos({ WINCX * 0.5f,WINCY * 0.5f });

	m_pQuad->Set_Texture(L"UI/square_gradient.png");
	m_pQuad->Set_Shader(L"g_UIShader.fx");
	m_pQuad->Set_Alpha(1.f);

	m_pGraphicDev->CreateTexture(
		m_tPanel.Get_Size().x, m_tPanel.Get_Size().y,
		1, D3DUSAGE_RENDERTARGET,
		D3DFMT_A8R8G8B8,
		D3DPOOL_DEFAULT,
		&m_pHintTex,
		NULL
	);

	m_sHint = L"여기에 힌트가 들어가게 됩니다.";
	m_fLifeTime = 3.5f;
	 m_fSpeed = 50.5f;
	return S_OK;
}

_int CHints::Update_GameObject(const _float& fTimeDelta)
{
	if (!m_bActive) {
		return 0;
	}	

	if (m_fAge >= m_fLifeTime) {
		m_fAge = 0.f;
		m_bActive = false;
	}

	m_fAge += fTimeDelta;
	m_tPanel.Add_PosY(-fTimeDelta * m_fSpeed);
	m_pQuad->Add_Alpha(-fTimeDelta*0.5f);

	m_pTransform->Set_Scale(m_tPanel.Get_WorldScale());
	m_pTransform->Set_Pos({ m_tPanel.Get_WorldPos(WINCX, WINCY) });
	CGameObject::Update_GameObject(fTimeDelta);

	// 서피스 가져오기
	m_pHintTex->GetSurfaceLevel(0, &m_pNewSurf);
	// 백버퍼 백업
	m_pGraphicDev->GetRenderTarget(0, &m_pOldSurf);

	// 텍스트 렌더링 준비
	m_pGraphicDev->SetRenderTarget(0, m_pNewSurf);
	m_pGraphicDev->Clear(0, nullptr, D3DCLEAR_TARGET, D3DCOLOR_ARGB(0, 0, 0, 0), 1.0f, 0);

	// 텍스트 출력
	RECT rc = { 0, 0, (LONG)m_tPanel.Get_Size().x, (LONG)m_tPanel.Get_Size().y };

	m_pFont->DrawTextW(nullptr, m_sHint.c_str(), -1, &rc,
		DT_CENTER|DT_WORDBREAK,
		D3DCOLOR_ARGB(255, 255, 255, 255));

	// 원래대로 복원
	m_pGraphicDev->SetRenderTarget(0, m_pOldSurf);
	m_pQuad->Set_TextureHandle(m_pHintTex);

	return 0;
}

void CHints::LateUpdate_GameObject(const _float& fTimeDelta)
{
	CGameObject::LateUpdate_GameObject(fTimeDelta);
}

void CHints::Set_Active(const wstring& Hint)
{
	m_fAge = 0.f;
	m_bActive = true;
	m_sHint = Hint;
	m_pQuad->Set_Alpha(1.f);
	m_tPanel.Set_Pos({ WINCX * 0.5f,WINCY * 0.5f -100});
}

void CHints::Free()
{
	CGameObject::Free();
	Safe_Release(m_pHintTex);
	Safe_Release(m_pNewSurf);
	Safe_Release(m_pOldSurf);
}
