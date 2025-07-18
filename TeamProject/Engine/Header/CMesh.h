#pragma once
#include "CResource.h"
BEGIN(Engine)
class ENGINE_DLL CMesh : public CResource
{
protected:
	explicit CMesh();
	explicit CMesh(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CMesh(const CMesh& rhs);


public:
	virtual ~CMesh();

public:
	static CMesh* Create();

	virtual HRESULT		Ready_Buffer();
	virtual void		Render_Buffer();

public:
	HRESULT Ready_Mesh();

	HRESULT LoadOBJ(LPDIRECT3DDEVICE9 pDevice,  wstring& path);
	virtual LPDIRECT3DVERTEXBUFFER9 Get_VertexBuffer() const { return m_pVB; };
	virtual LPDIRECT3DINDEXBUFFER9 Get_IndexBuffer() const { return m_pIB; };
	virtual const wstring& Get_Key() { return m_wKey; }
	virtual void Set_Key(const wstring& key) {  m_wKey = key; }
public:
	virtual void Free();

protected:
	LPDIRECT3DVERTEXBUFFER9		m_pVB;
	LPDIRECT3DINDEXBUFFER9		m_pIB;

	_ulong						m_dwVtxSize;	// 버텍스크기
	_ulong						m_dwVtxCnt;		// 버택스개수
	_ulong						m_dwFVF;		// FVF

	_ulong						m_dwIdxSize;	// 인덱스 크기
	_ulong						m_dwTriCnt;		// 삼각형 개수
	D3DFORMAT					m_IdxFmt;		// 인덱스 속성

	wstring m_wKey;
};
END
