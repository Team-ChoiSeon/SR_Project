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
	HRESULT Ready_Resource();

	void Register(const wstring& key, CResource* pResource);

	CResource* Get(const wstring& key);

	template <typename T>
	T* GetAs(const wstring& key);

	// �ؽ�ó �ε� (���ϸ� ��� ���)
	HRESULT Load_Texture(const wstring& filePath, TEXTUREID eType = TEX_CUBE, _uint iCnt = 6);

	// TODO: ���� ���͸��� �ε� �߰� ����
	HRESULT Load_Material(const D3DMATERIAL9& mtrl);

private:
	// ��ο��� ���� �̸� ����
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
