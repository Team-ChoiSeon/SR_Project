#pragma once
#include "CBase.h"

#include "CMesh.h"

#include "CTexture.h"
#include "CMaterial.h"

BEGIN(Engine)

class CResource;

class ENGINE_DLL CResourceMgr : public CBase
{
	DECLARE_SINGLETON(CResourceMgr)

private:
	explicit CResourceMgr();
	virtual ~CResourceMgr();

public:
	// [Initialize]
	// example: CResourceMgr::Get_Instance()->Ready_Resource();
	HRESULT Ready_Resource();

	// [Register]
	void Register(const wstring& key, CResource* pResource);

	// [Get]
	// example: CTexture* pTex = dynamic_cast<CTexture*>(CResourceMgr::Get_Instance()->Get(L"SkyTex"));
	CResource* Get(const wstring& key);

	// [Get - Template]
	// example: CTexture* pTex = CResourceMgr::Get_Instance()->GetAs<CTexture>(L"SkyTex");
	template <typename T>
	T* GetAs(const wstring& key);

	// [Mesh Load & Auto Register]
	// example: CResourceMgr::Get_Instance()->Load_Mesh<CRcCube>(pDevice, L"CubeMesh");
	template <typename MeshT>
	HRESULT Load_Mesh(LPDIRECT3DDEVICE9 pDevice, const wstring& key);

	// [Texture Load & Auto Register]
	// example: CResourceMgr::Get_Instance()->Load_Texture(L"../Textures/brick.jpg", TEX_NORMAL, 1);
	HRESULT Load_Texture(const wstring& filePath, TEXTUREID eType = TEX_CUBE, _uint iCnt = 6);

	// [TODO : ]
	HRESULT Load_Material(const D3DMATERIAL9& mtrl);

private:
	// Extract FileName From FilePath
	wstring Get_FileName(const wstring& filePath);

private:
	virtual void Free();

private:
	unordered_map<wstring, CResource*> m_umResource;
	LPDIRECT3DDEVICE9 m_pGraphicDev = nullptr;
};

END

template<typename T>
inline T* CResourceMgr::GetAs(const wstring& key)
{
	return dynamic_cast<T*>(Get(key));
}

template<typename MeshType>
inline HRESULT CResourceMgr::Load_Mesh(LPDIRECT3DDEVICE9 pDevice, const wstring& key)
{
	if (!std::is_base_of<CMesh, MeshType>::value)
	{
		//"MeshT must be derived from CMesh"
		return E_FAIL;
	}

	if (m_umResource.find(key) != m_umResource.end())
		return S_OK; // Already loaded

	MeshType* pMesh = MeshType::Create(pDevice);
	if (FAILED(pMesh->Ready_Buffer()))
	{
		Safe_Release(pMesh);
		return E_FAIL;
	}

	Register(key, pMesh);
	return S_OK;
}
