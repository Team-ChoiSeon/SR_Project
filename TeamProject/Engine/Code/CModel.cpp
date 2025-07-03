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

void CModel::LateUpdate_Component()
{
	CRenderMgr::Get_Instance()->Add_Model(this);
}

void CModel::Render(LPDIRECT3DDEVICE9 m_pDevice)
{
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
