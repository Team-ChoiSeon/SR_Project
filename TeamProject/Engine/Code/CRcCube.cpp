#include "CRcCube.h"

CRcCube::CRcCube()
{
}

CRcCube::CRcCube(LPDIRECT3DDEVICE9 pGraphicDev)
	: CVIBuffer(pGraphicDev)
{
}

CRcCube::CRcCube(const CRcCube& rhs)
	: CVIBuffer(rhs)
{
}

CRcCube::~CRcCube()
{
}

HRESULT CRcCube::Ready_Buffer()
{
	m_dwTriCnt = 12;
	m_dwVtxCnt = 8;
	m_dwVtxSize = sizeof(VTXCOL);
	m_dwFVF = FVF_CUBE;

	m_dwIdxSize = sizeof(INDEX32);
	m_IdxFmt = D3DFMT_INDEX32;

	if (FAILED(CVIBuffer::Ready_Buffer()))
		return E_FAIL;

	VTXCOL* pVertex = NULL;

	// ���ؽ� ���ۿ� ������ ���� �� ù ��° ������ �ּҸ� ������ �Ű� ����
	m_pVB->Lock(0, 0, (void**)&pVertex, 0);

	// ����
	pVertex[0].vPosition = { -1.f, 1.f, -1.f };
	pVertex[0].dwColor = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);

	pVertex[1].vPosition = { 1.f, 1.f, -1.f };
	pVertex[1].dwColor = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);

	pVertex[2].vPosition = { 1.f, -1.f, -1.f };
	pVertex[2].dwColor = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);

	pVertex[3].vPosition = { -1.f, -1.f, -1.f };
	pVertex[3].dwColor = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);

	// �ĸ�
	pVertex[4].vPosition = { -1.f, 1.f, 1.f };
	pVertex[4].dwColor = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);

	pVertex[5].vPosition = { 1.f, 1.f, 1.f };
	pVertex[5].dwColor = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);

	pVertex[6].vPosition = { 1.f, -1.f, 1.f };
	pVertex[6].dwColor = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);

	pVertex[7].vPosition = { -1.f, -1.f, 1.f };
	pVertex[7].dwColor = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);

	m_pVB->Unlock();

	INDEX32* pIndex = NULL;

	m_pIB->Lock(0, 0, (void**)&pIndex, 0);

	// X+
	// ������ ��
	pIndex[0] = { 1,5,6 };
	// ���� �Ʒ�
	pIndex[1] = { 1,6,2 };

	// X-
	// ������ ��
	pIndex[2] = { 4,0,3 };
	// ���� �Ʒ�
	pIndex[3] = { 4,3,7 };

	// Y+
	// ������ ��
	pIndex[4] = { 4,5,1 };
	// ���� �Ʒ�
	pIndex[5] = { 4,1,0 };

	// Y-
	// ������ ��
	pIndex[6] = { 3,2,6 };
	// ���� �Ʒ�
	pIndex[7] = { 3,6,7 };

	// Z+
	// ������ ��
	pIndex[8] = { 7,6,5 };
	// ���� �Ʒ�
	pIndex[9] = { 7,5,4 };

	// Z-
	// ������ ��
	pIndex[10] = { 0,1,2 };
	// ���� �Ʒ�
	pIndex[11] = { 0,2,3 };

	m_pIB->Unlock();


	return S_OK;
}

void CRcCube::Render_Buffer()
{
	CVIBuffer::Render_Buffer();
}

CRcCube* CRcCube::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CRcCube* pRcCube = new CRcCube(pGraphicDev);

	if (FAILED(pRcCube->Ready_Buffer()))
	{
		Safe_Release(pRcCube);
		MSG_BOX("pRcCube Create Failed");
		return nullptr;
	}

	return pRcCube;
}

void CRcCube::Free()
{
	CVIBuffer::Free();
}
