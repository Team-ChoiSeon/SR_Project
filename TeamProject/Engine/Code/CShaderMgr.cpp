#include "Engine_Define.h"
#include "CShaderMgr.h"
#include "CGraphicDev.h"
namespace fs = std::filesystem;

IMPLEMENT_SINGLETON(CShaderMgr)

CShaderMgr::CShaderMgr()
	:m_pDevice(nullptr)
{
}

CShaderMgr::~CShaderMgr()
{
	Free();
}

HRESULT CShaderMgr::Ready_Shader(LPDIRECT3DDEVICE9 pDevice)
{
	m_pDevice = CGraphicDev::Get_Instance()->Get_GraphicDev();
	if (!m_pDevice)
	{
		MessageBoxA(nullptr, "디바이스 초기화 실패", "에러", MB_OK);
		return E_FAIL;
	}

	Load_AllShaders("../../Shader/");
	return S_OK;
}

void CShaderMgr::Load_AllShaders(const string& folderPath)
{
	for (const auto& entry : fs::directory_iterator(folderPath))
	{
		if (entry.is_regular_file())
		{
			const string& fileName = entry.path().filename().string();
			if (entry.path().extension() == ".fx")
			{
				GetShader(fileName);
			}
		}
	}
}

LPD3DXEFFECT CShaderMgr::GetShader(const string& path)
{
	auto it = m_mapShader.find(path);

	if (it != m_mapShader.end())
		return it->second;

	wstring fullPath = ToWString("../../Shader/" + path); // 경로 조합

	LPD3DXEFFECT pEffect = nullptr;
	LPD3DXBUFFER pError = nullptr;
	HRESULT hr = D3DXCreateEffectFromFileW(
		m_pDevice,
		fullPath.c_str(),
		nullptr,
		nullptr,
		D3DXSHADER_DEBUG,
		nullptr,
		&pEffect,
		&pError // 여기서 컴파일 실패 시 상세 원인이 들어감
	);


	if (FAILED(hr)) {
		if (pError) {
			OutputDebugStringA((char*)pError->GetBufferPointer()); // 디버그 출력
			MessageBox(nullptr, (char*)pError->GetBufferPointer(), "Shader Compile Error", MB_OK); // 창으로 표시
			pError->Release();
		}
		else {
			MessageBoxW(nullptr, (L"Failed to load: " + fullPath).c_str(), L"Shader Error", MB_OK);
		}
		return nullptr;
	}

	m_mapShader[path] = pEffect;
	return pEffect;
}

HRESULT CShaderMgr::ReloadShader(const string& path)
{
	auto iter = m_mapShader.find(path);
	if (iter != m_mapShader.end()) {
		if (iter->second)
			Safe_Release(iter->second);
		m_mapShader.erase(iter);
	}

	LPD3DXEFFECT shader = GetShader(path);

	if (shader)
		return S_OK;

	return E_FAIL;
}

void CShaderMgr::Free()
{
	for (auto& pair : m_mapShader) {
		Safe_Release(pair.second);
	}
	m_mapShader.clear();
}

string CShaderMgr::ToString(const wstring& wstr)
{
	if (wstr.empty()) return {};
	int size_needed = WideCharToMultiByte(CP_UTF8, 0, wstr.data(), (int)wstr.size(), nullptr, 0, nullptr, nullptr);
	std::string result(size_needed, 0);
	WideCharToMultiByte(CP_UTF8, 0, wstr.data(), (int)wstr.size(), &result[0], size_needed, nullptr, nullptr);
	return result;
}

wstring CShaderMgr::ToWString(const string& str)
{
	if (str.empty()) return {};
	int size_needed = MultiByteToWideChar(CP_UTF8, 0, str.data(), (int)str.size(), nullptr, 0);
	std::wstring result(size_needed, 0);
	MultiByteToWideChar(CP_UTF8, 0, str.data(), (int)str.size(), &result[0], size_needed);
	return result;
}