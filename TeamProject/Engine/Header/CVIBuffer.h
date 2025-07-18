#pragma once
#include "CComponent.h"

BEGIN(Engine)

class ENGINE_DLL CVIBuffer : public CComponent
{
protected:
	explicit CVIBuffer();
	explicit CVIBuffer(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CVIBuffer(const CVIBuffer& rhs);
public:
	virtual ~CVIBuffer();

public:
	virtual HRESULT		Ready_Buffer();
	virtual void		Render_Buffer();

	virtual HRESULT	Ready_Component()override { return S_OK; };
	virtual void	Update_Component(const _float& fTimeDelta)override {}
	virtual void	LateUpdate_Component(const _float& fTimeDelta)override {}

public:
	virtual LPDIRECT3DVERTEXBUFFER9 Get_VertexBuffer() const { return m_pVB; };
	virtual LPDIRECT3DINDEXBUFFER9 Get_IndexBuffer() const { return m_pIB; };

protected:
	LPDIRECT3DVERTEXBUFFER9		m_pVB;
	LPDIRECT3DINDEXBUFFER9		m_pIB;

	_ulong						m_dwVtxSize;	// 버텍스크기
	_ulong						m_dwVtxCnt;		// 버택스개수
	_ulong						m_dwFVF;		// FVF

	_ulong						m_dwIdxSize;	// 인덱스 크기
	_ulong						m_dwTriCnt;		// 삼각형 개수
	D3DFORMAT					m_IdxFmt;		// 인덱스 속성



public:
	virtual void Free()override;
};

END