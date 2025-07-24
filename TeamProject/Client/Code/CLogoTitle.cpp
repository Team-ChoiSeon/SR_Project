#include "pch.h"
#include "CLogoTitle.h"
#include "CUiQuad.h"
#include "CTransform.h"
CLogoTitle::CLogoTitle(LPDIRECT3DDEVICE9 pGraphicDev)
	:CGameObject(pGraphicDev)
{
}

CLogoTitle::~CLogoTitle()
{
}

CLogoTitle* CLogoTitle::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
    CLogoTitle* instance = new CLogoTitle(pGraphicDev);

    if (FAILED(instance->Ready_GameObject())) {
        Safe_Release(instance);
        instance = nullptr;
    }

    return instance;
}

HRESULT CLogoTitle::Ready_GameObject()
{
	m_pFont = CResourceMgr::Get_Instance()->Get_Font(L"여주");

	m_pQuad = Add_Component<CUiQuad>(ID_DYNAMIC, m_pGraphicDev);
	m_pTransform = Add_Component<CTransform>(ID_DYNAMIC, m_pGraphicDev);
	m_tPanel.Set_Size({ WINCX ,40 });
	m_tPanel.Set_Pos({ WINCX * 0.5f,WINCY * 0.5f });

	m_pQuad->Set_Texture(L"UI/square_gradient.png");
	m_pQuad->Set_Shader(L"g_UIShader.fx");
	m_pQuad->Set_Alpha(1.f);
	m_pQuad->Set_Post(true);

	m_pGraphicDev->CreateTexture(
		m_tPanel.Get_Size().x, m_tPanel.Get_Size().y,
		1, D3DUSAGE_RENDERTARGET,
		D3DFMT_A8R8G8B8,
		D3DPOOL_DEFAULT,
		&m_pHintTex,
		NULL
	);

	m_sTitle = L"TEAM_CHOISUN";
	return S_OK;
}

_int CLogoTitle::Update_GameObject(const _float& fTimeDelta)
{
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
	RECT rcText = { 0, 0, (LONG)m_tPanel.Get_Size().x, (LONG)m_tPanel.Get_Size().y };
	// 3. 실제 텍스트 그리기
	m_pFont->DrawTextW(nullptr, m_sTitle.c_str(), -1, &rcText,
		DT_WORDBREAK | DT_CENTER,
		D3DCOLOR_ARGB(255, 255, 255, 255));

	// 원래대로 복원
	m_pGraphicDev->SetRenderTarget(0, m_pOldSurf);
	m_pQuad->Set_TextureHandle(m_pHintTex);

	return 0;
}

void CLogoTitle::LateUpdate_GameObject(const _float& fTimeDelta)
{
	CGameObject::LateUpdate_GameObject(fTimeDelta);
}

void CLogoTitle::Free()
{
}
