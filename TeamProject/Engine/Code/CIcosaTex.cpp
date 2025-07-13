#include "CIcosaTex.h"

CIcosaTex::CIcosaTex() {}

CIcosaTex::CIcosaTex(LPDIRECT3DDEVICE9 pGraphicDev)
    : CMesh(pGraphicDev)
{
}

CIcosaTex::CIcosaTex(const CIcosaTex& rhs)
    : CMesh(rhs)
{
}

CIcosaTex::~CIcosaTex()
{
}

HRESULT CIcosaTex::Ready_Buffer()
{
    const float phi = (1.f + sqrtf(5.f)) * 0.5f;
    const float scale = 1.f / sqrtf(1.f + phi * phi); // 정규화 길이 조정
    const float X = scale;
    const float Z = phi * scale;

    // 12 정점
    VTXCUBE vertices[12] =
    {
        {{-X,  0,  Z}, {-X,  0,  Z}},
        {{ X,  0,  Z}, { X,  0,  Z}},
        {{-X,  0, -Z}, {-X,  0, -Z}},
        {{ X,  0, -Z}, { X,  0, -Z}},
        {{ 0,  Z,  X}, { 0,  Z,  X}},
        {{ 0,  Z, -X}, { 0,  Z, -X}},
        {{ 0, -Z,  X}, { 0, -Z,  X}},
        {{ 0, -Z, -X}, { 0, -Z, -X}},
        {{ Z,  X,  0}, { Z,  X,  0}},
        {{-Z,  X,  0}, {-Z,  X,  0}},
        {{ Z, -X,  0}, { Z, -X,  0}},
        {{-Z, -X,  0}, {-Z, -X,  0}},
    };

    // 20면 (삼각형) ? 인덱스
    INDEX32 indices[20] =
    {
        {0, 4, 1},  {0, 9, 4},   {9, 5, 4},   {4, 5, 8},   {4, 8, 1},
        {8, 10, 1}, {8, 3, 10},  {5, 3, 8},   {5, 2, 3},   {2, 7, 3},
        {7, 10, 3}, {7, 6, 10},  {7, 11, 6},  {11, 0, 6},  {0, 1, 6},
        {6, 1, 10}, {9, 0, 11},  {9, 11, 2},  {9, 2, 5},   {7, 2, 11},
    };

    m_dwVtxCnt = 12;
    m_dwVtxSize = sizeof(VTXCUBE);
    m_dwFVF = FVF_CUBE;

    m_dwTriCnt = 20;
    m_dwIdxSize = sizeof(INDEX32);
    m_IdxFmt = D3DFMT_INDEX32;

    if (FAILED(CMesh::Ready_Buffer()))
        return E_FAIL;

    // 버텍스 입력
    VTXCUBE* pVertex = nullptr;
    m_pVB->Lock(0, 0, (void**)&pVertex, 0);
    memcpy(pVertex, vertices, sizeof(vertices));
    m_pVB->Unlock();

    // 인덱스 입력
    INDEX32* pIndex = nullptr;
    m_pIB->Lock(0, 0, (void**)&pIndex, 0);
    memcpy(pIndex, indices, sizeof(indices));
    m_pIB->Unlock();

    return S_OK;
}

void CIcosaTex::Render_Buffer()
{
    CMesh::Render_Buffer();
}

CIcosaTex* CIcosaTex::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
    CIcosaTex* pMesh = new CIcosaTex(pGraphicDev);

    if (FAILED(pMesh->Ready_Buffer()))
    {
        Safe_Release(pMesh);
        MSG_BOX("Icosahedron Create Failed");
        return nullptr;
    }

    return pMesh;
}

void CIcosaTex::Free()
{
    CMesh::Free();
}
