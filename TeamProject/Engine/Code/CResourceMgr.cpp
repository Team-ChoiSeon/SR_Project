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

void CResourceMgr::Register(const wstring& key, CResource* pResource)
{
	if (!pResource) return;

	if (m_umResource.contains(key))
		Safe_Release(m_umResource[key]);

	m_umResource[key] = pResource;
}

CResource* CResourceMgr::Get(const wstring& key)
{
	auto it = m_umResource.find(key);
	if (it == m_umResource.end())
		return nullptr;

	return it->second;
}

HRESULT CResourceMgr::Load_Texture(const wstring& filePath, TEXTUREID eType, _uint iCnt)
{
	std::wstring key = Get_FileName(filePath);
	if (m_umResource.contains(key))
		return E_FAIL;

	CTexture* pTexture = CTexture::Create(m_pGraphicDev, filePath, eType, iCnt);
	if (pTexture == nullptr)
		return E_FAIL;

	Register(key, pTexture);

	return S_OK;
}

HRESULT CResourceMgr::Load_Material(const D3DMATERIAL9& mtrl)
{
	return E_NOTIMPL;
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
	for (auto& [_, pRes] : m_umResource)
		Safe_Release(pRes);
	m_umResource.clear();
}