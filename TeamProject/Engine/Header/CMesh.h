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
	virtual HRESULT		Ready_Buffer();
	virtual void		Render_Buffer();

public:
	HRESULT LoadOBJ(LPDIRECT3DDEVICE9 pDevice, const wstring& path);
	virtual LPDIRECT3DVERTEXBUFFER9 Get_VertexBuffer() const { return m_pVB; };
	virtual LPDIRECT3DINDEXBUFFER9 Get_IndexBuffer() const { return m_pIB; };
	virtual const wstring& Get_Key() { return m_wKey; }
public:
	virtual void Free();

protected:
	LPDIRECT3DVERTEXBUFFER9		m_pVB;
	LPDIRECT3DINDEXBUFFER9		m_pIB;

	_ulong						m_dwVtxSize;	// ���ؽ�ũ��
	_ulong						m_dwVtxCnt;		// ���ý�����
	_ulong						m_dwFVF;		// FVF

	_ulong						m_dwIdxSize;	// �ε��� ũ��
	_ulong						m_dwTriCnt;		// �ﰢ�� ����
	D3DFORMAT					m_IdxFmt;		// �ε��� �Ӽ�

	wstring m_wKey;
};
END
