#include "CModel.h"
#include "CRenderMgr.h"

CModel::CModel(LPDIRECT3DDEVICE9 pDevice)
	: m_pDevice(pDevice)
{
	if (m_pDevice)
		m_pDevice->AddRef();
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
	// SetTransform
	
	//if (m_pMaterial)
	//	m_pMaterial->Apply(m_pDevice); 

	//if (m_pTexture)
	//	m_pTexture->Bind(m_pDevice); 

	//if (m_pMesh)
	//	m_pMesh->Render(m_pDevice);
}

void CModel::Free()
{
	Safe_Release(m_pDevice);
	Safe_Release(m_pMesh);
	Safe_Release(m_pMaterial);
	Safe_Release(m_pTexture);
}
