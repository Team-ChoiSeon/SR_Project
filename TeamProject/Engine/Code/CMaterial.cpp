#include "CMaterial.h"
#include "CShaderMgr.h"

CMaterial::CMaterial()
{
	ZeroMemory(&m_tMaterial, sizeof(D3DMATERIAL9));
}

CMaterial::~CMaterial()
{

}

void CMaterial::Apply(LPDIRECT3DDEVICE9 pDevice)
{

    if (m_pEffect)
    {
        // 셰이더 파라미터 바인딩
        if (m_pDiffuse)  m_pEffect->SetTexture("g_DiffuseTex", m_pDiffuse->Get_Texture());
        if (m_pNormal)   m_pEffect->SetTexture("g_NormalTex", m_pNormal->Get_Texture());
        if (m_pRoughness)m_pEffect->SetTexture("g_RoughnessTex", m_pRoughness->Get_Texture());
        return;
    }
    if (m_pDiffuse)
        m_pDiffuse->Bind(pDevice, 0);
}

HRESULT CMaterial::Ready_Material()
{
    ZeroMemory(&m_tMaterial, sizeof(D3DMATERIAL9));

    m_tMaterial.Diffuse = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);  // 흰색
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

void CMaterial::Set_Shader(const string& path)
{
    m_pEffect = CShaderMgr::Get_Instance()->GetShader(path);
    if (m_pEffect)
        m_strShaderPath = path;
    else
        m_strShaderPath.clear(); // 로딩 실패 시 키 제거
}

void CMaterial::Free()
{
    CResource::Free();
}
