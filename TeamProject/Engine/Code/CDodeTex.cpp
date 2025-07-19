#include "CDodeTex.h"

CDodeTex::CDodeTex() {}

CDodeTex::CDodeTex(LPDIRECT3DDEVICE9 pGraphicDev)
    : CMesh(pGraphicDev)
{
}

CDodeTex::CDodeTex(const CDodeTex& rhs)
    : CMesh(rhs)
{
}

CDodeTex::~CDodeTex()
{
}

HRESULT CDodeTex::Ready_Buffer()
{
    const float phi = (1.f + sqrtf(5.f)) * 0.5f;
    const float a = 1.f;
    const float b = 1.f / phi;

    // 정점 정의 (정십이면체의 20개 꼭짓점)
    VTXCUBE vertices[20] =
    {
        {{-a, -a, -a}, {-a, -a, -a}},
        {{-a, -a,  a}, {-a, -a,  a}},
        {{-a,  a, -a}, {-a,  a, -a}},
        {{-a,  a,  a}, {-a,  a,  a}},
        {{ a, -a, -a}, { a, -a, -a}},
        {{ a, -a,  a}, { a, -a,  a}},
        {{ a,  a, -a}, { a,  a, -a}},
        {{ a,  a,  a}, { a,  a,  a}},
        {{ 0, -b, -phi}, { 0, -b, -phi}},
        {{ 0, -b,  phi}, { 0, -b,  phi}},
        {{ 0,  b, -phi}, { 0,  b, -phi}},
        {{ 0,  b,  phi}, { 0,  b,  phi}},
        {{-b, -phi, 0}, {-b, -phi, 0}},
        {{-b,  phi, 0}, {-b,  phi, 0}},
        {{ b, -phi, 0}, { b, -phi, 0}},
        {{ b,  phi, 0}, { b,  phi, 0}},
        {{-phi, 0, -b}, {-phi, 0, -b}},
        {{ phi, 0, -b}, { phi, 0, -b}},
        {{-phi, 0,  b}, {-phi, 0,  b}},
        {{ phi, 0,  b}, { phi, 0,  b}},
    };

    m_dwVtxCnt = 20;
    m_dwVtxSize = sizeof(VTXCUBE);
    m_dwFVF = FVF_CUBE;

    m_dwIdxSize = sizeof(INDEX32);
    m_IdxFmt = D3DFMT_INDEX32;

    // 예시 삼각형 2개만 정의 (12면 전체는 추후 구현 가능)
    m_dwTriCnt = 2;

    if (FAILED(CMesh::Ready_Buffer()))
        return E_FAIL;

    VTXCUBE* pVertex = nullptr;
    m_pVB->Lock(0, 0, (void**)&pVertex, 0);
    memcpy(pVertex, vertices, sizeof(vertices));
    m_pVB->Unlock();

    INDEX32* pIndex = nullptr;
    m_pIB->Lock(0, 0, (void**)&pIndex, 0);

    // 샘플로 삼각형 2개만
    pIndex[0] = { 0, 8, 4 };
    pIndex[1] = { 0, 4, 16 };

    m_pIB->Unlock();

    return S_OK;
}

void CDodeTex::Render_Buffer()
{
    CMesh::Render_Buffer();
}

CDodeTex* CDodeTex::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
    CDodeTex* pMesh = new CDodeTex(pGraphicDev);

    if (FAILED(pMesh->Ready_Buffer()))
    {
        Safe_Release(pMesh);
        MSG_BOX("Dodecahedron Create Failed");
        return nullptr;
    }

    return pMesh;
}

void CDodeTex::Free()
{
    CMesh::Free();
}
