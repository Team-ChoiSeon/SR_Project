#pragma once
#include "CComponent.h"
#include "CMesh.h"
#include "CMaterial.h"
#include "CTexture.h"
#include "CResourceMgr.h"
#include "Engine_Model.h"


BEGIN(Engine)

//class CMesh;
//class CTexture;
//class CMaterial;

class ENGINE_DLL CModel : public CComponent
{
public:
    explicit CModel(LPDIRECT3DDEVICE9 pDevice);
    virtual ~CModel();

public:
    static CModel* Create(LPDIRECT3DDEVICE9 pDevice);
    //static CModel* Create(LPDIRECT3DDEVICE9 pDevice, const DefaultTileModel& model);
    
    virtual HRESULT Ready_Component()override { return S_OK; }
    virtual void Update_Component(const _float& fTimeDelta)override {};
    virtual void LateUpdate_Component(const _float& fTimeDelta)override;
    
    void Render(LPDIRECT3DDEVICE9 pDevice);
    HRESULT Set_Model(const wstring& meshType, const wstring& matType);
    void Set_Alpha(float alpha) { m_fAlpha = alpha; };
    void Set_UVScale(_vec4 uvScale) { m_uvScale = uvScale; };

    //void Set_Mesh(CMesh* pMesh) { m_pMesh = pMesh; }
    //void Set_Material(CMaterial* pMaterial) { m_pMaterial = pMaterial; }

    CMesh* Get_Mesh() { return m_pMesh; }
    CMaterial* Get_Material() { return m_pMaterial; }
    float Get_Alpha() { return m_fAlpha; }
public:
    RENDER_PASS Get_RenderPass() { return m_ePass; };
    virtual void Free()override;
private:
    template<typename T>
    void Safe_Change(T& lhs, T rhs);
private:
    CMesh* m_pMesh = nullptr;
    CMaterial* m_pMaterial = nullptr; // material include texture
    _vec4 m_uvScale;
    _float m_fAlpha =1.f;
    RENDER_PASS m_ePass = RENDER_PASS::RP_OPAQUE;

};

END

template<typename T>
inline void CModel::Safe_Change(T& lhs, T rhs)
{
    if (lhs != nullptr) {
        Safe_Release(lhs);
    }

    lhs = rhs;

    if (rhs != nullptr) {
        rhs->AddRef();
    }
}
