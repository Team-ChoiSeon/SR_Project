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
		return it->second; // �̹� �ε�� �ؽ�ó ��ȯ

	wstring BasePath = L"../Bin/Resource/Texture/";
	wstring filePath = BasePath + texturePath;

	CTexture* tex = CTexture::Create();
	if (!tex) return nullptr;

	if (FAILED(tex->Load(m_pGraphicDev, filePath))) {
		Safe_Release(tex);
		return nullptr;
	}

	tex->Set_Key(texturePath);
	m_umTexture[texturePath] = tex;

	return tex; // ���� �� �ؽ�ó ��ȯ
}

CMesh* CResourceMgr::Load_Mesh(LPDIRECT3DDEVICE9 pDevice, const wstring& key)
{
	auto iter = m_umMesh.find(key);
	if (iter != m_umMesh.end()) {
		return iter->second;
	}

	wstring BasePath = L"../Bin/Resource/Obj/";
	wstring filePath = BasePath + key;

	CMesh* mesh = CMesh::Create();
	if (FAILED(mesh->LoadOBJ(pDevice, filePath))) {
		Safe_Release(mesh);
		return nullptr;
	}
	mesh->Set_Key(key);
	m_umMesh[key] = mesh;
	return mesh;
}

CMaterial* CResourceMgr::Load_Material(const wstring& mtlPath)
{
	// �̹� �ε�� ��Ƽ������ �ִٸ� ���� (�ߺ� ����)
	auto iter = m_umMaterial.find(mtlPath);
	if (iter != m_umMaterial.end())
		return iter->second;

	// ��� ����
	wstring basePath = L"../Bin/Resource/Material/";
	wstring fullPath = basePath + mtlPath;

	ifstream in(fullPath, ios::binary);
	if (!in.is_open()) {
		MSG_BOX("ResourceMgr::File Open Failed");
		return nullptr;
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
	CTexture* tex = Load_Texture(texturePath);
	if (!tex) return nullptr;

	// ��Ƽ���� ���� �� ���
	CMaterial* mat = CMaterial::Create();
	if (!mat) return nullptr;

	mat->Set_Diffuse(tex);
	mat->Set_MaterialKey(mtlPath);
	m_umMaterial[mtlPath] = mat;

	return mat; //  �ùٸ� ��ȯ
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