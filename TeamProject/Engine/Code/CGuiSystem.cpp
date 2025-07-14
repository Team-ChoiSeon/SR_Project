#include "Engine_Define.h"
#include "Engine_GUI.h"
#include "CGuiSystem.h"
#include "CGraphicDev.h"

IMPLEMENT_SINGLETON(CGuiSystem)
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

CGuiSystem::CGuiSystem()
{
}

CGuiSystem::~CGuiSystem()
{
	Free();
}

HRESULT CGuiSystem::Ready_GUI(HWND hwnd)
{
	m_pDevice = CGraphicDev::Get_Instance()->Get_GraphicDev();

	IMGUI_CHECKVERSION();

	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.DisplaySize = ImVec2(static_cast<float>(WINCX), static_cast<float>(WINCY));

	ImFont* bigFont = io.Fonts->AddFontFromFileTTF("../../ThirdParty/NanumSquareNeo-cBd.ttf", 
		24.0f, nullptr, io.Fonts->GetGlyphRangesKorean()); // 큰 폰트

	ImFont* regular = io.Fonts->AddFontFromFileTTF("../../ThirdParty/NanumSquareNeo-bRg.ttf", 
		18.0f, nullptr, io.Fonts->GetGlyphRangesKorean()); //큰 폰트

	io.FontDefault = regular;
	m_fontContainer.insert({ "Bold",bigFont });
	m_fontContainer.insert({ "Regular",regular });

	ImGui::StyleColorsDark();  // 또는 Light, Classic
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls

	if (!ImGui_ImplWin32_Init(hwnd))
		return E_FAIL;
	if (!ImGui_ImplDX9_Init(m_pDevice))
		return E_FAIL;

	return S_OK;
}

void CGuiSystem::Render_GUI()
{
	ImGui_ImplDX9_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	for (auto& pair : m_PanelContainer)
	{
		const string& name = pair.first;
		PANEL& panel = pair.second;
	
		if (!panel.bOpen)
			continue;
	
		if (panel.callback)
			panel.callback();
	}
	// 간단한 GUI 창 하나 출력
	
	ImGui::Render();
	ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());

	m_PanelContainer.clear();
}

ImFont* CGuiSystem::Get_Font(const string& tag)
{
	auto iter = m_fontContainer.find(tag);
	if (iter != m_fontContainer.end()) {
		return iter->second;
	}
	return nullptr;
}

bool CGuiSystem::Set_Input(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	return ImGui_ImplWin32_WndProcHandler(hWnd, message, wParam, lParam);
}

void CGuiSystem::RegisterPanel(const string& name, function<void()> callback)
{
	m_PanelContainer.emplace(name, PANEL(true, callback));
}

void CGuiSystem::RemovePanel(const string& name)
{
	auto iter = m_PanelContainer.find(name);
	if (iter != m_PanelContainer.end()) {
		m_PanelContainer.erase(iter);
	}
}

// 종료: 백엔드 및 컨텍스트 해제
void CGuiSystem::Free()
{
	ImGui_ImplDX9_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}

