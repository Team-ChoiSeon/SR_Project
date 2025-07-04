#include "CResourceMgr.h"
#include "CGraphicDev.h"
#include "CCubeTex.h"


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
	Load_Mesh <CCubeTex>(m_pGraphicDev, L"CCubeTex");
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

	m_umTexture[texturePath] = tex;
	return S_OK;
}

HRESULT CResourceMgr::Load_Material(const wstring& mtlPath)
{
	// �̹� �ε�Ǿ� ������ ����
	if (m_umMaterial.find(mtlPath) != m_umMaterial.end())
		return S_OK;

	// ��� ����
	wstring basePath = L"../Bin/Resource/Material/";
	wstring fullPath = basePath + mtlPath;

	// UTF-8 ���̳ʸ��� �б�
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
		wstring wline = ToWString(utf8Line); // UTF-8 �� wide string
		wistringstream iss(wline);
		wstring token;
		iss >> token;

		if (token == L"newmtl") {
			iss >> matName;
		}
		else if (token == L"map_Kd") {
			iss >> texturePath;

			// ������ �߶󳻱�
			size_t pos = texturePath.find_last_of(L"/\\");
			if (pos != wstring::npos)
				texturePath = texturePath.substr(pos + 1);
		}
	}

	// �ؽ�ó �ε�
	if (FAILED(Load_Texture(texturePath)))
		return E_FAIL;

	CTexture* tex = Get_Texture(texturePath);
	if (!tex) return E_FAIL;

	// ��Ƽ���� ���� �� ���
	CMaterial* mat = CMaterial::Create();
	mat->Set_Diffuse(tex);
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