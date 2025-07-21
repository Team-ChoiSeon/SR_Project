#include "pch.h"
#include "CHealthPanel.h"
#include "CHealthBar.h"
#include "CCameraMgr.h"
#include "CSceneMgr.h"
#include "CUiQuad.h"
#include "CGuiSystem.h"

CHealthPanel::CHealthPanel(LPDIRECT3DDEVICE9 pGraphicDev)
	:CGameObject(pGraphicDev)
{
}

CHealthPanel::~CHealthPanel()
{
}

CHealthPanel* CHealthPanel::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CHealthPanel* instance = new CHealthPanel(pGraphicDev);

	if (FAILED(instance->Ready_GameObject())) {
		Safe_Release(instance);
		instance = nullptr;
	}

	return instance;
}

HRESULT CHealthPanel::Ready_GameObject()
{
	m_pHealthBar = CHealthBar::Create(m_pGraphicDev);
	m_pTransform = Add_Component<CTransform>(ID_DYNAMIC, m_pGraphicDev);
	m_pTransform->Rotate_Axis({0,1,0}, D3DXToRadian(45));
	m_pQuad = Add_Component<CUiQuad>(ID_DYNAMIC, m_pGraphicDev);
	m_pQuad->Set_Texture(L"UI/Vinjette.png");
	m_vScreen = {WINCX/2,WINCY/2,1};
	return S_OK;
}

_int CHealthPanel::Update_GameObject(const _float& fTimeDelta)
{
	CGameObject::Update_GameObject(fTimeDelta);
	CGameObject* mainCamera = CCameraMgr::Get_Instance()->Get_MainCamera();
	if (!mainCamera)
		return 0;
	m_pMainCam = mainCamera->Get_Component<CCamera>();

	//m_pTransform->Set_Pos(m_pMainCam->Get_Eye() + m_pMainCam->Get_Look() *4);
	Compute_ScreenToWorld();

	m_pQuad->Set_QuadPos(m_vWorldPos, { 10,10});

	//m_pHealthBar->Set_Pos(m_vWorldPos, { -0.8,-0.8,0});
	m_pHealthBar->Update_GameObject(fTimeDelta);
	CGuiSystem::Get_Instance()->RegisterPanel("HealthPanel", [this]() {
		if (ImGui::Begin("Health Panel Debug", nullptr, ImGuiWindowFlags_AlwaysAutoResize))
		{
	
			ImGui::InputFloat3("Position", (float*)&m_vWorldPos, "%.2f");
			ImGui::InputFloat2("Screen", (float*)&m_vTest, "%.2f");
		}
		ImGui::End();
		});

	return 0;
}

void CHealthPanel::LateUpdate_GameObject(const _float& fTimeDelta)
{
	CGameObject::LateUpdate_GameObject(fTimeDelta);
	m_pHealthBar->LateUpdate_GameObject(fTimeDelta);
}

void CHealthPanel::Compute_ScreenToWorld()
{
	D3DVIEWPORT9		ViewPort;
	ZeroMemory(&ViewPort, sizeof(D3DVIEWPORT9)); //dx 뷰포트 가져오기
	m_pGraphicDev->GetViewport(&ViewPort);
	POINT pt = { m_vScreen.x ,m_vScreen.y};
	//ScreenToClient(g_hWnd,&pt);
	m_vTest.x = pt.x;
	m_vTest.y = pt.y;
	_vec3 ndc;
	ndc.x = ((float)pt.x / (float)ViewPort.Width) * 2.0f - 1.0f;
	ndc.y = 1.0f - ((float)pt.y / (float)ViewPort.Height) * 2.0f;	

	ndc.z = .9f;

	// 투영 -> 뷰 스페이스
	D3DXMATRIX	matProj;
	matProj = *(CCameraMgr::Get_Instance()->Get_MainProjectionMatrix());
	D3DXMatrixInverse(&matProj, 0, &matProj); //역행렬 돌려줌.
	D3DXVec3TransformCoord(&ndc, &ndc, &matProj); //그 역행렬을 기준으로 ndc를 계산함.

	// 뷰 스페이스 -> 월드
	D3DXMATRIX	matView;
	matView = *(CCameraMgr::Get_Instance()->Get_MainViewMatrix());
	D3DXMatrixInverse(&matView, 0, &matView);

	D3DXVec3TransformCoord(&ndc, &ndc, &matView); //레이포즈를 월드 위치 벡터로 변환

	m_vWorldPos = ndc;
}

void CHealthPanel::Free()
{
	CGameObject::Free();
	Safe_Release(m_pHealthBar);
	Safe_Release(m_pGraphicDev);
}
