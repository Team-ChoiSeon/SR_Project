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

	// [Mesh Register]
	// 
	template <typename MeshT>
	CMesh* Load_Mesh(LPDIRECT3DDEVICE9 pDevice, const wstring& key);
	CMesh* Load_Mesh(LPDIRECT3DDEVICE9 pDevice, const wstring& key);

	// [Material Register]
	// 
	CMaterial* Load_Material(const wstring& mtlPath);

	// [Texture Register]
	// 
	CTexture* Load_Texture(const wstring& filePath);

	HRESULT Load_GameObject(const wstring& filePath);



	// 이름 기반 조회
	CMesh* Get_Mesh(const wstring& key)
	{
		auto iter = m_umMesh.find(key);
		if (iter != m_umMesh.end()) {
			return iter->second;
		}
		return nullptr;
	}

	CMaterial* Get_Material(const wstring& key)
	{
		auto iter = m_umMaterial.find(key);
		if (iter != m_umMaterial.end()) {
			return iter->second;
		}
		return nullptr;
	}
	CTexture* Get_Texture(const wstring& key)
	{
		auto iter = m_umTexture.find(key);
		if (iter != m_umTexture.end()) {
			return iter->second;
		}
		return nullptr;
	}
	

	string ToString(const wstring& wstr);
	wstring ToWString(const string& str);
	LAYERID ToLayer(const wstring& wstr);

private:
	// Extract FileName From FilePath
	wstring Get_FileName(const wstring& filePath);


private:
	virtual void Free();

private:
	// 사용 X
	// unordered_map<wstring, CResource*> m_umResource;
	unordered_map<wstring, CMesh*> m_umMesh;
	unordered_map<wstring, CMaterial*> m_umMaterial;
	unordered_map<wstring, CTexture*> m_umTexture;

	LPDIRECT3DDEVICE9 m_pGraphicDev = nullptr;
};

END

template<typename MeshType>
inline CMesh* CResourceMgr::Load_Mesh(LPDIRECT3DDEVICE9 pDevice, const wstring& key)
{
	static_assert(std::is_base_of<CMesh, MeshType>::value, "MeshType must derive from CMesh");

	auto it = m_umMesh.find(key);
	if (it != m_umMesh.end())
		return it->second; // Already loaded

	MeshType* pMesh = MeshType::Create(pDevice);
	if (!pMesh) return nullptr;

	if (!pMesh->LoadOBJ(pDevice, key)) // 로딩 실패시 nullptr 반환
	{
		Safe_Release(pMesh);
		return nullptr;
	}

	m_umMesh[key] = pMesh;
	return pMesh;
}
