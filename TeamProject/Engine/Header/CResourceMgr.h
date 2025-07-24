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

	CMesh* Load_Mesh(LPDIRECT3DDEVICE9 pDevice, const wstring& key);
	
	CMaterial* Load_Material(const wstring& mtlPath);
	CTexture* Load_Texture(const wstring& filePath);

	ID3DXFont* Get_Font(const wstring& key);
	void PreLoad_Font();
	void Free_Font();

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


private:
	ID3DXFont* Load_Font(const wstring& key, const wstring& path, _float size);

	virtual void Free();

private:
	// 사용 X
	// unordered_map<wstring, CResource*> m_umResource;
	unordered_map<wstring, CMesh*> m_umMesh;
	unordered_map<wstring, CMaterial*> m_umMaterial;
	unordered_map<wstring, CTexture*> m_umTexture;
	unordered_map<wstring, ID3DXFont*> m_umFont;

	LPDIRECT3DDEVICE9 m_pGraphicDev = nullptr;
};

END

//emplate<typename MeshType>
//nline CMesh* CResourceMgr::Load_Mesh(LPDIRECT3DDEVICE9 pDevice, const wstring& key)
//
//	static_assert(std::is_base_of<CMesh, MeshType>::value, "MeshType must derive from CMesh");
//
//	auto it = m_umMesh.find(key);
//	if (it != m_umMesh.end())
//		return it->second; // Already loaded
//
//	MeshType* pMesh = MeshType::Create(pDevice);
//	if (!pMesh) return nullptr;
//
//	if (!pMesh->LoadOBJ(pDevice, key)) // 로딩 실패시 nullptr 반환
//	{
//		Safe_Release(pMesh);
//		return nullptr;
//	}
//
//	m_umMesh[key] = pMesh;
//	return pMesh;
//
