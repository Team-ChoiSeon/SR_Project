#include "CMaterial.h"

CMaterial::CMaterial()
{
	ZeroMemory(&m_tMaterial, sizeof(D3DMATERIAL9));
}

CMaterial::~CMaterial()
{
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

void CMaterial::Free()
{
    CResource::Free();
}
