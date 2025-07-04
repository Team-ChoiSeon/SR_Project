#include "CModel.h"
#include "CRenderMgr.h"
#include "CTransform.h"

CModel::CModel(LPDIRECT3DDEVICE9 pDevice)
	: CComponent(pDevice)
{
	if (m_pGraphicDev)
		m_pGraphicDev->AddRef();
}

CModel::~CModel()
{
}

CModel* CModel::Create(LPDIRECT3DDEVICE9 pDevice, const DefaultCubeModel& model)
{
	auto pMesh = CResourceMgr::Get_Instance()->Get_Mesh(model.meshKey);
	auto pMaterial = CResourceMgr::Get_Instance()->Get_Material(model.materialKey);

	if (!pMesh || !pMaterial) {
		MSG_BOX("CModel::Create - Resource Missing");
		return nullptr;
	}
	auto pModel = new CModel(pDevice);
	pModel->Set_Mesh(pMesh);
	pModel->Set_Material(pMaterial);
	OutputDebugString("[CModel] Create 호출됨\n");
	return pModel;
}

void CModel::LateUpdate_Component()
{
	OutputDebugString("[CModel] LateUpdate_Component 호출됨\n");
	CRenderMgr::Get_Instance()->Add_Model(this);
	OutputDebugString("[CModel] 등록됨\n");
}

void CModel::Render(LPDIRECT3DDEVICE9 m_pDevice)
{
	if (!m_pMesh)
	{
		OutputDebugString("[CModel] m_pMesh is nullptr\n");
		return;
	}
	if (!m_pMaterial)
	{
		OutputDebugString("[CModel] m_pMaterial is nullptr\n");
		return;
	}

	CTransform* pTransform = m_pOwner->Get_Component<CTransform>();
	if (pTransform == nullptr)
	{
		MSG_BOX("CModel::Render : pTransform is nullptr");
		return;
	}
	m_pDevice->SetTransform(D3DTS_WORLD, pTransform->Get_WorldMatrix());

	if (m_pMaterial)
		m_pMaterial->Apply(m_pDevice); // 내부에서 텍스처를 Bind

	if (m_pMesh)
		m_pMesh->Render_Buffer();
}

void CModel::Free()
{
	Safe_Release(m_pGraphicDev);
	Safe_Release(m_pMesh);
	Safe_Release(m_pMaterial);
	// Safe_Release(m_pTexture);
}
