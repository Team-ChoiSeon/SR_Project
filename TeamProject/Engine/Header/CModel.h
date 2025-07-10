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
    virtual void LateUpdate_Component()override;
    void Render(LPDIRECT3DDEVICE9 pDevice);
    HRESULT Set_Model(const wstring& meshType, const wstring& matType);

    //void Set_Mesh(CMesh* pMesh) { m_pMesh = pMesh; }
    //void Set_Material(CMaterial* pMaterial) { m_pMaterial = pMaterial; }

    CMesh* Get_Mesh() { return m_pMesh; }
    CMaterial* Get_Material() { return m_pMaterial; }
    void Set_UVScale(_vec4 _uvScale) { uvScale = _uvScale; }
public:
    RENDER_PASS Get_RenderPass() { return RENDER_PASS::RP_OPAQUE; };
    virtual void Free()override;

private:
    template<typename T>
    void Safe_Change(T& lhs, T rhs);
private:
    CMesh* m_pMesh = nullptr;
    CMaterial* m_pMaterial = nullptr; // material include texture
    _vec4 uvScale;

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
