#include "CGameObject.h"

CGameObject::CGameObject(LPDIRECT3DDEVICE9 pGraphicDev)
    : m_pGraphicDev(pGraphicDev)
{
    m_pGraphicDev->AddRef();
}

CGameObject::CGameObject(const CGameObject& rhs)
    : m_pGraphicDev(rhs.m_pGraphicDev)
{
    m_pGraphicDev->AddRef();
}

CGameObject::~CGameObject()
{
}



HRESULT CGameObject::Ready_GameObject()
{
    return S_OK;
}

_int CGameObject::Update_GameObject(const _float& fTimeDelta)
{
    for (auto& pComponent : m_umComponent[ID_DYNAMIC])
        pComponent.second->Update_Component(fTimeDelta);

    return 0;
}

void CGameObject::LateUpdate_GameObject(const _float& fTimeDelta)
{
    for (auto& pComponent : m_umComponent[ID_DYNAMIC])
    {
        pComponent.second->LateUpdate_Component();
    }
}

//void CGameObject::Deserialize(const json& jObj)
//{
//	//Transform
//	if (auto transform = pObj->Get_Component<CTransform>()) {
//		auto pos = jObj["position"];
//		auto rot = jObj["rotation"];
//		auto scale = jObj["scale"];
//		auto pivot = jObj["pivot"];
//		auto orbit = jObj["orbit"];
//
//		transform->Set_Pos({ pos[0], pos[1], pos[2] });
//		transform->Set_Rotate({ rot[0], rot[1], rot[2] });
//		transform->Set_Scale({ scale[0], scale[1], scale[2] });
//		transform->Set_Pivot({ pivot[0], pivot[1], pivot[2] });
//		transform->Set_Orbit({ orbit[0], orbit[1], orbit[2] });
//		// �θ�� ���߿� ó�� (��ü �Ľ� �� ����)
//	}
//
//	// Model
//	if (auto model = pObj->Get_Component<CModel>()) {
//		// Model Ű ���� ���� Ȯ��
//		if (jObj.contains("mesh") && jObj.contains("matKey")) {
//			string meshKey = jObj["mesh"];
//			string materialKey = jObj["matKey"];
//
//			if (!meshKey.empty() && !materialKey.empty()) {
//				model->Set_Model(meshKey, materialKey);
//
//				if (auto mat = model->Get_Material()) {
//					if (jObj.contains("material")) {
//						auto& jMat = jObj["material"];
//
//						mat->Set_Diffuse(CResourceMgr::GetInstance()->LoadTexture(jMat.value("diffuse", "")));
//						mat->Set_Normal(CResourceMgr::GetInstance()->LoadTexture(jMat.value("normal", "")));
//						mat->Set_Roughness(CResourceMgr::GetInstance()->LoadTexture(jMat.value("roughness", "")));
//						mat->Set_Specular(CResourceMgr::GetInstance()->LoadTexture(jMat.value("specular", "")));
//						mat->Set_Emissive(CResourceMgr::GetInstance()->LoadTexture(jMat.value("emissive", "")));
//					}
//				}
//			}
//		}
//
//	}
//}

void CGameObject::Free()
{
    for (_uint i = 0; i < ID_END; ++i)
    {
        for (auto& [tag, comp] : m_umComponent[i])
            Safe_Release(comp);
        m_umComponent[i].clear();
    }
    

    Safe_Release(m_pGraphicDev);
}
