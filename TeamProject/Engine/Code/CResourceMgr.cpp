#include "CResourceMgr.h"
#include "CGraphicDev.h"
#include "CCubeTex.h"
#include "CFactory.h"
#include "CSceneMgr.h"


IMPLEMENT_SINGLETON(CResourceMgr)

CResourceMgr::CResourceMgr()
{

}

CResourceMgr::~CResourceMgr()
{
	Free();
}

HRESULT CResourceMgr::Ready_Resource()
{
	m_pGraphicDev = CGraphicDev::Get_Instance()->Get_GraphicDev();

	if (!m_pGraphicDev)
		return E_FAIL;

	m_pGraphicDev->AddRef();
}

CTexture* CResourceMgr::Load_Texture(const wstring& texturePath)
{
	auto it = m_umTexture.find(texturePath);
	if (it != m_umTexture.end())
		return it->second; // 이미 로드된 텍스처 반환

	wstring BasePath = L"../Bin/Resource/Texture/Diffuse/";
	wstring filePath = BasePath + texturePath;

	if (texturePath.empty())
		filePath = BasePath + L"T_SciFi_Greebles_Vol01_Panels_01_BC.PNG";
	else
		filePath = BasePath + texturePath;

	CTexture* tex = CTexture::Create();
	if (!tex) return nullptr;

	if (FAILED(tex->Load(m_pGraphicDev, filePath))) {
		Safe_Release(tex);
		return nullptr;
	}

	tex->Set_Key(texturePath);
	m_umTexture[texturePath] = tex;

	return tex; // 성공 시 텍스처 반환
}

CMesh* CResourceMgr::Load_Mesh(LPDIRECT3DDEVICE9 pDevice, const wstring& key)
{
	auto iter = m_umMesh.find(key);
	if (iter != m_umMesh.end()) {
		return iter->second;
	}

	CMesh* mesh = CMesh::Create();
	wstring meshKey = key;
	if (FAILED(mesh->LoadOBJ(pDevice, meshKey))) {
		Safe_Release(mesh);

		auto iter = m_umMesh.find(meshKey);
		if (iter != m_umMesh.end()) {
			return iter->second;
		}
		else {
			return nullptr;
		}
	}

	mesh->Set_Key(meshKey);
	m_umMesh[meshKey] = mesh;
	return mesh;
}

CMaterial* CResourceMgr::Load_Material(const wstring& mtlPath)
{
	// 이미 로드된 머티리얼이 있다면 재사용 (중복 방지)
	auto iter = m_umMaterial.find(mtlPath);
	if (iter != m_umMaterial.end())
		return iter->second;

	// 경로 조합
	wstring mtlKey = mtlPath;
	wstring basePath = L"../Bin/Resource/Material/";
	wstring fullPath = basePath + mtlPath;

	ifstream in;

	in.open(fullPath);

	if (!in.is_open()) {
		fullPath = basePath + L"Tile_A.mtl";
		mtlKey = L"Tile_A.mtl";
		// 스트림 상태 초기화
		in.clear();
		in.open(fullPath);
		if (!in.is_open()) {
			MessageBoxW(0, L"Mtl Load Err", L"Err", MB_OK);
		}
		else {
			auto iter = m_umMaterial.find(mtlKey);
			if (iter != m_umMaterial.end())
				return iter->second;
		}
	}

	string utf8Line;
	wstring matName;
	wstring texturePath;

	while (getline(in, utf8Line))
	{
		wstring wline = ToWString(utf8Line); // UTF-8 → wide string
		wistringstream iss(wline);
		wstring token;
		iss >> token;

		if (token == L"newmtl") {
			iss >> matName;
		}
		else if (token == L"map_Kd") {
			iss >> texturePath;

			// 슬래시 잘라내기
			size_t pos = texturePath.find_last_of(L"/\\");
			if (pos != wstring::npos)
				texturePath = texturePath.substr(pos + 1);
		}
	}

	if (texturePath.empty()) {
		MessageBoxW(0, L"MTL 파일에 텍스처 경로가 없습니다.", L"Error", MB_OK);
		return nullptr;
	}

	// 텍스처 로드
	CTexture* tex = Load_Texture(texturePath);
	if (!tex) return nullptr;

	// 머티리얼 생성 및 등록
	CMaterial* mat = CMaterial::Create();
	if (!mat) return nullptr;

	mat->Set_Diffuse(tex);
	mat->Set_MaterialKey(mtlKey);
	m_umMaterial[mtlKey] = mat;

	return mat; 
}


string CResourceMgr::ToString(const wstring& wstr)
{
	if (wstr.empty()) return {};
	int size_needed = WideCharToMultiByte(CP_UTF8, 0, wstr.data(), (int)wstr.size(), nullptr, 0, nullptr, nullptr);
	std::string result(size_needed, 0);
	WideCharToMultiByte(CP_UTF8, 0, wstr.data(), (int)wstr.size(), &result[0], size_needed, nullptr, nullptr);
	return result;
}

wstring CResourceMgr::ToWString(const string& str)
{
	if (str.empty()) return {};
	int size_needed = MultiByteToWideChar(CP_UTF8, 0, str.data(), (int)str.size(), nullptr, 0);
	std::wstring result(size_needed, 0);
	MultiByteToWideChar(CP_UTF8, 0, str.data(), (int)str.size(), &result[0], size_needed);
	return result;
}


void CResourceMgr::Free()
{
	for (auto& [_, pRes] : m_umMesh)
		Safe_Release(pRes);
	m_umMesh.clear();

	for (auto& [_, pRes] : m_umMaterial)

		Safe_Release(pRes);
	m_umMaterial.clear();

	for (auto& [_, pRes] : m_umTexture)
		Safe_Release(pRes);
	m_umTexture.clear();
}