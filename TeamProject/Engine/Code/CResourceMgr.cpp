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

	Load_Material(L"DirtObj.mtl");
	Load_Material(L"BrickRoad.mtl");
	Load_Mesh <CCubeTex>(m_pGraphicDev, L"CCubeTex");
	Load_Mesh <CCubeTex>(m_pGraphicDev,L"DirtObj.obj");
	Load_Mesh <CCubeTex>(m_pGraphicDev, L"BrickRoad.obj");
	m_pGraphicDev->AddRef();
}

HRESULT CResourceMgr::Load_Texture(const wstring& texturePath)
{
	auto it = m_umTexture.find(texturePath);
	if (it != m_umTexture.end())
		return E_FAIL;

	wstring BasePath = L"../Bin/Resource/Texture/";
	wstring filePath = BasePath + texturePath;

	CTexture* tex = CTexture::Create();

	if (FAILED(tex->Load(m_pGraphicDev, filePath))) {
		Safe_Release(tex);
		return E_FAIL;
	}
	tex->Set_Key(texturePath);
	m_umTexture[texturePath] = tex;
	return S_OK;
}

HRESULT CResourceMgr::Load_GameObject(const wstring& filePath)
{
	//HANDLE hFile = ::CreateFileW(
	//	path.c_str(), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	//if (hFile == INVALID_HANDLE_VALUE) {
	//	MessageBoxW(nullptr, L"파일 열기 실패", L"Error", MB_OK);
	//	return;
	//}

	//DWORD fileSize = GetFileSize(hFile, NULL);
	//std::string jsonText(fileSize, '\0');
	//DWORD bytesRead = 0;
	//ReadFile(hFile, jsonText.data(), fileSize, &bytesRead, NULL);
	//CloseHandle(hFile);

	//json jScene = json::parse(jsonText);

	//for (const auto& jObj : jScene["objects"])
	//{
	//	string className = jObj["class"];
	//	string instanceName = jObj["name"];
	//	string layerName = jObj["Layer"];

	//	CGameObject* pObj = CFactory::Create(ToWString(className), m_pGraphicDev);
	//	if (!pObj) continue;

	//	pObj->Deserialize(jObj);

	//	CScene* pScene = CSceneMgr::Get_Instance()->Get_Scene();
	//	// 예: Layer 등록 -> d
	//	LAYERID id = ToLayer(ToWString(jObj["Layer"]));
	//	pScene->Get_Layer(id)->Add_GameObject(ToWString(instanceName), pObj);
	//}

	return S_OK;
}

HRESULT CResourceMgr::Load_Material(const wstring& mtlPath)
{
	// 이미 로드되어 있으면 종료
	if (m_umMaterial.find(mtlPath) != m_umMaterial.end())
		return S_OK;

	// 경로 조합
	wstring basePath = L"../Bin/Resource/Material/";
	wstring fullPath = basePath + mtlPath;

	// UTF-8 바이너리로 읽기
	ifstream in(fullPath, ios::binary);
	if (!in.is_open()) {
		MSG_BOX("ResourceMgr::File Open Failed");
		return E_FAIL;
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

	// 텍스처 로드
	if (FAILED(Load_Texture(texturePath)))
		return E_FAIL;

	CTexture* tex = Get_Texture(texturePath);
	if (!tex) return E_FAIL;

	// 머티리얼 생성 및 등록
	CMaterial* mat = CMaterial::Create();
	mat->Set_Diffuse(tex);
	mat->Set_MaterialKey(mtlPath);
	m_umMaterial[mtlPath] = mat;

	return S_OK;
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

// enum LAYERID { LAYER_TILE, LAYER_OBJECT, LAYER_PLAYER, LAYER_CAMERA, LAYER_END };
LAYERID CResourceMgr::ToLayer(const wstring& wstr)
{
	if (wstr == L"CAMERA")  return LAYERID::LAYER_CAMERA;
	if (wstr == L"PLAYER")  return LAYERID::LAYER_PLAYER;
	if (wstr == L"OBJECT")  return LAYERID::LAYER_OBJECT;
	if (wstr == L"TILE")    return LAYERID::LAYER_TILE;

	return LAYERID::LAYER_END; // 또는 예외 처리용 L_INVALID이 있으면 더 좋음
}

wstring CResourceMgr::Get_FileName(const wstring& filePath)
{
	for (int i = static_cast<int>(filePath.size()) - 1; i >= 0; --i)
	{
		if (filePath[i] == L'\\' || filePath[i] == L'/')
			return filePath.substr(i + 1);
	}
	return filePath;
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