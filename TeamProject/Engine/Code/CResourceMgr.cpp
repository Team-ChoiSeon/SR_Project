#include "CResourceMgr.h"
#include "CGraphicDev.h"


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
	// Already exist
	auto iter = m_umMaterial.find(mtlPath);
	if (iter != m_umMaterial.end())
		return E_FAIL;

	// Set Path
	wstring BasePath = L"../Bin/Resource/Material/";
	wstring filePath = BasePath + mtlPath;

	// Open .mtl file 
	wifstream in(filePath);
	if (!in.is_open()) {
		MSG_BOX("ResourceMgr::File Open Failed");
		return E_FAIL;
	}

	// Ready .mtl parsing
	wstring line;
	wstring matName;
	wstring texturePath;

	// �� �پ� �����鼭 �Ľ�
	while (std::getline(in, line))//���پ�
	{
		wistringstream iss(line);
		wstring token;
		iss >> token; //line ���ڿ��� ����(����) �������� ��ū ������ �и�

		// ��Ƽ���� �̸� ����
		if (token == L"newmtl") {
			iss >> matName;//newmtl�� ������ �״����� ��Ƽ���� ����
		}
		// ��ǻ�� �ؽ�ó ��� ����
		else if (token == L"map_Kd") {
			iss >> texturePath;//map_Kd�� ������ ������ �ؽ���

			// ��� ���ڿ����� ������ ������(/ �Ǵ� \)�� ��ġ�� ã��
			// ��: "C:/folder/texture.jpg" �� ������ '/'�� index 11
			size_t pos = texturePath.find_last_of(L"/\\");

			// ã�Ҵٸ� (npos�� �ƴ϶��) �� ���ϸ� ����
			// ��: substr(12) �� "texture.jpg"
			if (pos != wstring::npos)
				texturePath = texturePath.substr(pos + 1);
		}
	}

	// �ؽ�ó �ε� �� ��� ���� (�� ���� ��� ������ ���� ���� ����)
	Load_Texture(texturePath);
	CTexture* tex = Get_Texture(texturePath);
	if (!tex) return E_FAIL;

	// ��Ƽ���� ��ü ���� �� �ؽ�ó ����
	CMaterial* mat = CMaterial::Create();
	mat->Set_Diffuse(tex);

	// ��Ƽ���� �ʿ� ��� (�̸����� ����)
	m_umMaterial[mtlPath] = mat;

	return S_OK;
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