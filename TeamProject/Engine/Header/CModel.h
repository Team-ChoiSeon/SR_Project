#pragma once
#include "CComponent.h"
#include "CMesh.h"
#include "CMaterial.h"
#include "CTexture.h"
#include "CResourceMgr.h"


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
    template <typename ModelT, typename... Args>
    static CModel* Create(LPDIRECT3DDEVICE9 pDevice, Args&&... args);

    virtual void LateUpdate_Component()override;
    void Render(LPDIRECT3DDEVICE9 pDevice);

    void Set_Mesh(CMesh* pMesh) { m_pMesh = pMesh; }
    // void Set_Texture(CTexture* pTexture) { m_pTexture = pTexture; }
    void Set_Material(CMaterial* pMaterial) { m_pMaterial = pMaterial; }

public:
    RENDER_PASS Get_RenderPass() { return RENDER_PASS::RP_OPAQUE; };
    virtual void Free()override;

private:
    LPDIRECT3DDEVICE9 m_pDevice = nullptr;

    CMesh* m_pMesh = nullptr;
    // CTexture* m_pTexture = nullptr;
    CMaterial* m_pMaterial = nullptr; // material include texture

};


template <typename ModelT, typename... Args>
static CModel* CModel::Create(LPDIRECT3DDEVICE9 pDevice, Args&&... args)
{
    ModelT model;
    // 인자들을 하나씩 처리    
    (ApplyArg(model, std::forward<Args>(args)), ...);
    auto pMesh = CResourceMgr::Get_Instance()->Get_Mesh(model.meshKey);
    // auto pTexture = CResourceMgr::Get_Instance()->Get_Texture(model.textureKey);
    auto pMaterial = CResourceMgr::Get_Instance()->Get_Material(model.materialKey);
    

    if (!pMesh || !pMaterial) {
        MSG_BOX("CModel::Create - Resource Missing");
        return nullptr;
    }

    auto pModel = new CModel(pDevice);
    pModel->Set_Mesh(pMesh);
    // pModel->Set_Texture(pTexture);
    pModel->Set_Material(pMaterial);

    return pModel;
};

END