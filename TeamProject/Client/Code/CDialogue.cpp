#include "pch.h"
#include "Engine_Define.h"
#include "CDialogue.h"
#include "CResourceMgr.h"
#include "CUiQuad.h"

constexpr int width = 220;
constexpr int height = 80;

CDialogue::CDialogue(LPDIRECT3DDEVICE9 pGraphicDev)
	:CGameObject(pGraphicDev)
{
}

CDialogue::~CDialogue()
{
}

CDialogue* CDialogue::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CDialogue* instance = new CDialogue(pGraphicDev);

	if (FAILED(instance->Ready_GameObject())) {
		Safe_Release(instance);
		instance = nullptr;
	}

	return instance;
}

HRESULT CDialogue::Ready_GameObject()
{
	m_pFont = CResourceMgr::Get_Instance()->Get_Font(L"나눔");

	m_pQuad = Add_Component<CUiQuad>(ID_DYNAMIC, m_pGraphicDev);
	m_pTransform = Add_Component<CTransform>(ID_DYNAMIC, m_pGraphicDev);
	m_tPanel.Set_Size({ width,height });

	m_pQuad->Set_Texture(L"UI/square_gradient.png");
	m_pQuad->Set_Shader(L"g_UIShader.fx");

	m_pGraphicDev->CreateTexture(
		width, height,
		1, D3DUSAGE_RENDERTARGET,
		D3DFMT_A8R8G8B8,
		D3DPOOL_DEFAULT,
		&m_pDialTex,
		NULL
	);
	m_sDialogue = L"여기에 오브젝트 설명이 주저리주저리 들어가게 됩니다.";
	return S_OK;
}

_int CDialogue::Update_GameObject(const _float& fTimeDelta)
{
	if (!m_bOpen) {
		m_tPanel.Set_Size({ 0,0 });
		return 0;
	}	m_tPanel.Set_Size({ width,height });


	m_pTransform->Set_Scale(m_tPanel.Get_WorldScale());
	m_pTransform->Set_Pos({ m_tPanel.Get_WorldPos(WINCX, WINCY) });
	CGameObject::Update_GameObject(fTimeDelta);

	// 서피스 가져오기
	LPDIRECT3DSURFACE9 newSurf = nullptr;
	m_pDialTex->GetSurfaceLevel(0, &newSurf);

	// 백버퍼 백업
	LPDIRECT3DSURFACE9 pOldSurf = nullptr;
	m_pGraphicDev->GetRenderTarget(0, &pOldSurf);

	// 텍스트 렌더링 준비
	m_pGraphicDev->SetRenderTarget(0, newSurf);
	m_pGraphicDev->Clear(0, nullptr, D3DCLEAR_TARGET, D3DCOLOR_ARGB(0, 0, 0, 0), 1.0f, 0);

	// 텍스트 출력
	RECT rc = { 0, 0, width,height };  // 텍스처 내 영역
	m_pFont->DrawTextW(nullptr, m_sDialogue.c_str(), - 1, &rc, 
		  DT_WORDBREAK,
		D3DCOLOR_ARGB(255, 255, 255, 255));

	// 원래대로 복원
	m_pGraphicDev->SetRenderTarget(0, pOldSurf);
	Safe_Release(pOldSurf);
	Safe_Release(newSurf);

	m_pQuad->Set_TextureHandle(m_pDialTex);

	return 0;
}

void CDialogue::LateUpdate_GameObject(const _float& fTimeDelta)
{
	if (!m_bOpen) return;
	CGameObject::LateUpdate_GameObject(fTimeDelta);
}

void CDialogue::Set_Pos(_vec2 pos)
{
	m_tPanel.Set_Anchor(UIPanel::Anchor::Left, pos);
	m_tPanel.Set_Anchor(UIPanel::Anchor::Bottom, pos);
}

void CDialogue::Set_Object(CGameObject* Obj)
{
}

void CDialogue::Free()
{
	CGameObject::Free();
}
