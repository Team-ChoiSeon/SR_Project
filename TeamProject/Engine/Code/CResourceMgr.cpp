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

	// 한 줄씩 읽으면서 파싱
	while (std::getline(in, line))//한줄씩
	{
		wistringstream iss(line);
		wstring token;
		iss >> token; //line 문자열을 띄어쓰기(공백) 기준으로 토큰 단위로 분리

		// 머티리얼 이름 저장
		if (token == L"newmtl") {
			iss >> matName;//newmtl이 나오면 그다음은 머티리얼 네임
		}
		// 디퓨즈 텍스처 경로 추출
		else if (token == L"map_Kd") {
			iss >> texturePath;//map_Kd이 나오면 다음은 텍스쳐

			// 경로 문자열에서 마지막 슬래시(/ 또는 \)의 위치를 찾음
			// 예: "C:/folder/texture.jpg" → 마지막 '/'는 index 11
			size_t pos = texturePath.find_last_of(L"/\\");

			// 찾았다면 (npos가 아니라면) → 파일명만 추출
			// 예: substr(12) → "texture.jpg"
			if (pos != wstring::npos)
				texturePath = texturePath.substr(pos + 1);
		}
	}

	// 텍스처 로드 시 경로 조합 (※ 실제 경로 구조에 따라 수정 가능)
	Load_Texture(texturePath);
	CTexture* tex = Get_Texture(texturePath);
	if (!tex) return E_FAIL;

	// 머티리얼 객체 생성 및 텍스처 설정
	CMaterial* mat = CMaterial::Create();
	mat->Set_Diffuse(tex);

	// 머티리얼 맵에 등록 (이름으로 저장)
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