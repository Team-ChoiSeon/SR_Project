#include "CMaterial.h"

CMaterial::CMaterial()
{
	ZeroMemory(&m_tMaterial, sizeof(D3DMATERIAL9));
}

CMaterial::~CMaterial()
{

}

void CMaterial::Apply(LPDIRECT3DDEVICE9 pDevice)
{
    if (m_pDiffuse)
        m_pDiffuse->Bind(pDevice, 0);
}

HRESULT CMaterial::Ready_Material()
{
    ZeroMemory(&m_tMaterial, sizeof(D3DMATERIAL9));

    m_tMaterial.Diffuse = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);  // Èò»ö
    m_tMaterial.Ambient = m_tMaterial.Diffuse;
    m_tMaterial.Specular = D3DXCOLOR(0.3f, 0.3f, 0.3f, 1.f);
    m_tMaterial.Emissive = D3DXCOLOR(0.f, 0.f, 0.f, 1.f);
    m_tMaterial.Power = 10.f;

    return S_OK;
}

CMaterial* CMaterial::Create()
{
    CMaterial* pTex = new CMaterial();
    if (FAILED(pTex->Ready_Material()))
    {
        Safe_Release(pTex);
        return nullptr;
    }
    return pTex;
}

void CMaterial::Free()
{
    CResource::Free();
}
