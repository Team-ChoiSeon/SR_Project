#include "pch.h"
#include "Engine_Define.h"
#include "CObjectInfo.h"
#include "CUiQuad.h"
#include "CTransform.h"
#include "CMainPlayer.h"
#include "CSceneMgr.h"
#include "CInfoSlot.h"
#include "CObjCapture.h"
#include "CDialogue.h"
#include "CPickingMgr.h"

CObjectInfo::CObjectInfo(LPDIRECT3DDEVICE9 pGraphicDev)
	:CGameObject(pGraphicDev)
{
}

CObjectInfo::~CObjectInfo()
{
}

CObjectInfo* CObjectInfo::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CObjectInfo* instance = new CObjectInfo(pGraphicDev);

	if (FAILED(instance->Ready_GameObject())) {
		Safe_Release(instance);
		instance = nullptr;
	}

	return instance;
}

HRESULT CObjectInfo::Ready_GameObject()
{
	m_pQuad = Add_Component<CUiQuad>(ID_DYNAMIC, m_pGraphicDev);
	m_pTransform = Add_Component<CTransform>(ID_DYNAMIC, m_pGraphicDev);

	m_tPanel.Set_Size({ 0,0 });

	m_pQuad->Set_Texture(L"UI/CinematicBar.png");
	m_pQuad->Set_Shader(L"g_UIShader.fx");
	m_pQuad->Set_Alpha(0.6);

	m_pSlot = CInfoSlot::Create(m_pGraphicDev);
	m_pCapture = CObjCapture::Create(m_pGraphicDev);
	m_pDialogue = CDialogue::Create(m_pGraphicDev);
	return S_OK;
}

_int CObjectInfo::Update_GameObject(const _float& fTimeDelta)
{

	if (!m_pPlayer) {
		if (CGameObject* player = CSceneMgr::Get_Instance()->Get_Player()) {
			m_pPlayer = static_cast<CMainPlayer*>(player);
		}
		else {

			return 0;
		}
	}
	m_bOpen = m_pPlayer->Get_PickObj();

	m_pPicked = m_pPlayer->Get_PickObj();
	if (!m_pPicked) {
		m_pPicked = CPickingMgr::Get_Instance()->Get_HitNearObject(40.f);
	}

	OpenPanel(fTimeDelta);
	CGameObject::Update_GameObject(fTimeDelta);
	m_pTransform->Set_Scale(m_tPanel.Get_WorldScale());
	m_pTransform->Set_Pos({ m_tPanel.Get_WorldPos(WINCX, WINCY) });

	m_pSlot->Update_GameObject(fTimeDelta);
	m_pCapture->Update_GameObject(fTimeDelta);
	m_pDialogue->Update_GameObject(fTimeDelta);
	return 0;
}

void CObjectInfo::LateUpdate_GameObject(const _float& fTimeDelta)
{
	CGameObject::LateUpdate_GameObject(fTimeDelta);
	m_pSlot->LateUpdate_GameObject(fTimeDelta);
	m_pCapture->LateUpdate_GameObject(fTimeDelta);
	m_pDialogue->LateUpdate_GameObject(fTimeDelta);
}

void CObjectInfo::OpenPanel(const _float& fTimeDelta)
{
	_vec2 offset = { 5.f ,5.f };

	UIPanel& slotPanel = m_pSlot->Get_Panel();
	UIPanel& capturePanel = m_pCapture->Get_Panel();
	_vec2 iconPos = { m_tPanel.LT() + slotPanel.Get_hSize() + offset };
	_vec2 SlotPos = { m_tPanel.LT() + slotPanel.Get_Size() + offset};

	if (m_bOpen) {
		if (m_tPanel.Open_While({ 150,200 }, fTimeDelta * 750)) {
			m_pCapture->Set_Open(true);
			m_pCapture->Set_Pos(SlotPos);
			m_pCapture->Set_Object(m_pPicked);
			m_pDialogue->Set_Open(true);
			m_pDialogue->Set_Pos({ m_tPanel.LB().x + 5, m_tPanel.LB().y });
			m_pDialogue->Set_Object(m_pPicked);
		}
	}
	else {
		m_tPanel.Close_While({ 30,30 }, fTimeDelta * 550);
		m_pCapture->Set_Open(false);
		m_pCapture->Set_Object(nullptr);
		m_pDialogue->Set_Open(false);
	}

	m_pSlot->Set_Pos(iconPos);
	m_tPanel.Set_Anchor(UIPanel::Anchor::Right, { WINCX - 10,0 });
	m_tPanel.Set_Anchor(UIPanel::Anchor::Top, { 0, 10 });
}

void CObjectInfo::Free()
{
	CGameObject::Free();
	Safe_Release(m_pSlot);
	Safe_Release(m_pCapture);
	Safe_Release(m_pDialogue);
}
