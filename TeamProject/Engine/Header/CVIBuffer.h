#pragma once
#include "CComponent.h"

BEGIN(Engine)

class ENGINE_DLL CVIBuffer : public CComponent
{
protected:
	explicit CVIBuffer();
	explicit CVIBuffer(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CVIBuffer(const CVIBuffer& rhs);
	virtual ~CVIBuffer();

public:
	virtual HRESULT		Ready_Buffer();
	virtual void		Render_Buffer();

protected:
	LPDIRECT3DVERTEXBUFFER9		m_pVB;
	LPDIRECT3DINDEXBUFFER9		m_pIB;

	_ulong						m_dwVtxSize;	// ���ؽ�ũ��
	_ulong						m_dwVtxCnt;		// ���ý�����
	_ulong						m_dwFVF;		// FVF

	_ulong						m_dwIdxSize;	// �ε��� ũ��
	_ulong						m_dwTriCnt;		// �ﰢ�� ����
	D3DFORMAT					m_IdxFmt;		// �ε��� �Ӽ�



public:
	virtual void Free();
};

END

