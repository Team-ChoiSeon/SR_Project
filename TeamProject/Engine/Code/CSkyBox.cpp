#include "Engine_Define.h"
#include "CSkyBox.h"
#include "CRenderMgr.h"
#include "CCameraMgr.h"
#include "CGameObject.h"
#include "CTransform.h"

CSkyBox::CSkyBox(LPDIRECT3DDEVICE9 pGraphicDev)
: CComponent(pGraphicDev)
{
}

CSkyBox::~CSkyBox()
{
}

CSkyBox* CSkyBox::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
    CSkyBox* instance = new CSkyBox(pGraphicDev);

    if (FAILED(instance->Ready_Component())) {
        Safe_Release(instance);
        instance = nullptr;
    }

    return instance;
}

HRESULT CSkyBox::Ready_Component()
{
    const DWORD numIndices = 36;
    const DWORD numVertices = 8;
    // 정점 버퍼 생성
    HRESULT hr = m_pGraphicDev->CreateVertexBuffer(sizeof(VTXCUBE) * numVertices,
        0, FVF_CUBE, D3DPOOL_MANAGED, &m_pVB, 0);
    if (FAILED(hr)) {
        return E_FAIL;
    }

    // 인덱스 버퍼 생성
    hr = m_pGraphicDev->CreateIndexBuffer(sizeof(DWORD) * numIndices,
        0, D3DFMT_INDEX32, D3DPOOL_MANAGED, &m_pIB, 0);
    if (FAILED(hr)) {
        return E_FAIL;
    }

    float s = 1.f;

    VTXCUBE cube[8] = {};

    // 전면
    cube[0].vPosition = { -1.f, 1.f, -1.f };
    cube[0].vTexUV = cube[0].vPosition;

    cube[1].vPosition = { 1.f, 1.f, -1.f };
    cube[1].vTexUV = cube[1].vPosition;

    cube[2].vPosition = { 1.f, -1.f, -1.f };
    cube[2].vTexUV = cube[2].vPosition;

    cube[3].vPosition = { -1.f, -1.f, -1.f };
    cube[3].vTexUV = cube[3].vPosition;

    cube[4].vPosition = { -1.f, 1.f, 1.f };
    cube[4].vTexUV = cube[4].vPosition;

    cube[5].vPosition = { 1.f, 1.f, 1.f };
    cube[5].vTexUV = cube[5].vPosition;

    cube[6].vPosition = { 1.f, -1.f, 1.f };
    cube[6].vTexUV = cube[6].vPosition;

    cube[7].vPosition = { -1.f, -1.f, 1.f };
    cube[7].vTexUV = cube[7].vPosition;

    INDEX32 indices[12] = {};
    // X+
    // 오른쪽 위                    // 왼쪽 아래
    indices[0]._0 = 1;        indices[1]._0 = 1;
    indices[0]._1 = 5;        indices[1]._1 = 6;;
    indices[0]._2 = 6;        indices[1]._2 = 2;

    // X-
    // 오른쪽 위                    // 왼쪽 아래
    indices[2]._0 = 4;       indices[3]._0 = 4;
    indices[2]._1 = 0;        indices[3]._1 = 3;
    indices[2]._2 = 3;       indices[3]._2 = 7;

    // Y+
    // 오른쪽 위                      // 왼쪽 아래
    indices[4]._0 = 4;       indices[5]._0 = 4;
    indices[4]._1 = 5;         indices[5]._1 = 1;
    indices[4]._2 = 1;         indices[5]._2 = 0;

    // Y-
    // 오른쪽 위                          // 왼쪽 아래
    indices[6]._0 = 3;            indices[7]._0 = 3;
    indices[6]._1 = 2;             indices[7]._1 = 6;
    indices[6]._2 = 6;            indices[7]._2 = 7;

    // Z+
    // 오른쪽 위                        // 왼쪽 아래
    indices[8]._0 = 7;          indices[9]._0 = 7;
    indices[8]._1 = 6;           indices[9]._1 = 5;
    indices[8]._2 = 5;          indices[9]._2 = 4;

    // Z-
    // 오른쪽 위                        // 왼쪽 아래
    indices[10]._0 = 0;            indices[11]._0 = 0;
    indices[10]._1 = 1;              indices[11]._1 = 2;
    indices[10]._2 = 2;            indices[11]._2 = 3;

    // 정점 정의
    void* pVertices = nullptr;
    m_pVB->Lock(0, 0, &pVertices, FVF_CUBE);
    memcpy(pVertices, cube, sizeof(cube));
    m_pVB->Unlock();

    // 인덱스 정의
    void* pIndices = nullptr;
    m_pIB->Lock(0, 0, &pIndices, D3DFMT_INDEX32);
    memcpy(pIndices, indices, sizeof(indices));
    m_pIB->Unlock();

    return S_OK;
}

void CSkyBox::Update_Component(const _float& fTimeDelta)
{
}

void CSkyBox::LateUpdate_Component(const _float& fTimeDelta)
{
    CRenderMgr::Get_Instance()->Add_SkyBox(this);
}

void CSkyBox::Render(LPDIRECT3DDEVICE9 pDevice)
{
    if (!m_pTransform) {
        m_pTransform= m_pOwner->Get_Component<CTransform>();
        return;
    }
    _matrix matCamView = *(CCameraMgr::Get_Instance()->Get_MainViewMatrix());
    _matrix matInvView;
    D3DXMatrixInverse(&matInvView, nullptr, &matCamView);

    D3DXVECTOR3 vCamPos(matInvView._41, matInvView._42, matInvView._43);

    _matrix matWorld;
    D3DXMatrixTranslation(&matWorld, vCamPos.x, vCamPos.y, vCamPos.z);
    pDevice->SetTransform(D3DTS_WORLD, &matWorld);

    pDevice->SetTransform(D3DTS_VIEW, &matCamView);


    pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
    pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
    pDevice->SetRenderState(D3DRS_SPECULARENABLE, FALSE);
    pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
    // 스카이박스 렌더링 전
    pDevice->SetRenderState(D3DRS_ZENABLE, FALSE);

    pDevice->SetFVF(FVF_CUBE);

    pDevice->SetStreamSource(0, m_pVB, 0, sizeof(VTXCUBE));
    pDevice->SetTexture(0, m_pCubeTexture);
    pDevice->SetIndices(m_pIB);
    pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 8, 0, 12);

    pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
    pDevice->SetRenderState(D3DRS_ZENABLE, TRUE);
    //pDevice->SetTransform(D3DTS_VIEW, &CamView);
    pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
    pDevice->SetRenderState(D3DRS_SPECULARENABLE, TRUE);
    pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);           // 기본 컬링 복구
}


void CSkyBox::Set_Texture(const wstring& key)
{
    wstring basePath = L"../Bin/Resource/Texture/Dds/"+key;
    if (FAILED(D3DXCreateCubeTextureFromFileW(m_pGraphicDev, basePath.c_str(), &m_pCubeTexture)))
    {
        MessageBoxW(nullptr, L"스카이박스 큐브맵 텍스처 로드 실패", L"Error", MB_OK);
    }
}

void CSkyBox::Free()
{
    Safe_Release(m_pIB);
    Safe_Release(m_pVB);
    Safe_Release(m_pGraphicDev);
    Safe_Release(m_pCubeTexture);
}
