#include "CModel.h"

CModel::CModel(LPDIRECT3DDEVICE9 pDevice)
	: m_pDevice(pDevice)
{
	if (m_pDevice)
		m_pDevice->AddRef();
}

CModel::~CModel()
{
}

void CModel::Render()
{
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
