#include "pch.h"
#include "CImpulseCube.h"
#include "CCollider.h"
#include "CTransform.h"
#include "CRigidbody.h"

CImpulseCube::CImpulseCube(LPDIRECT3DDEVICE9 pGraphicDev)
	: CCube(pGraphicDev)
{
}

CImpulseCube::CImpulseCube(const CImpulseCube& rhs)
	: CCube(rhs.m_pGraphicDev)
{
}

CImpulseCube::~CImpulseCube()
{
}

HRESULT CImpulseCube::Ready_GameObject()
{
	DefaultCubeModel tModel;
	Add_Component<CTransform>(ID_DYNAMIC, m_pGraphicDev);
	Add_Component<CCollider>(ID_DYNAMIC, m_pGraphicDev);
	Add_Component<CModel>(ID_DYNAMIC, m_pGraphicDev, tModel);
	Add_Component<CRigidbody>(ID_DYNAMIC, m_pGraphicDev, Get_Component<CTransform>());

	m_pTransform = Get_Component<CTransform>();
	m_pModel = Get_Component<CModel>();
	m_pCollider = Get_Component<CCollider>();
	m_pRigid = Get_Component<CRigidbody>();
	m_pCollider->Set_ColTag(ColliderTag::NONE);
	m_pCollider->Set_ColType(ColliderType::ACTIVE);

	return S_OK;
}

_int CImpulseCube::Update_GameObject(const _float& fTimeDelta)
{
	for (auto& pComponent : m_umComponent[ID_DYNAMIC])
		pComponent.second->Update_Component(fTimeDelta);
	return S_OK;
}

void CImpulseCube::LateUpdate_GameObject(const _float& fTimeDelta)
{
	for (auto& pComponent : m_umComponent[ID_DYNAMIC])
		pComponent.second->LateUpdate_Component();
}

CImpulseCube* CImpulseCube::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CImpulseCube* pImpulseCube = new CImpulseCube(pGraphicDev);
	if (FAILED(pImpulseCube->Ready_GameObject()))
	{
		Safe_Release(pImpulseCube);
		MSG_BOX("FCube Create Failed");
		return nullptr;
	}
	return pImpulseCube;
}

void CImpulseCube::Free()
{
	Safe_Release(m_pTransform);
	Safe_Release(m_pModel);
	Safe_Release(m_pCollider);
	Safe_Release(m_pGraphicDev);
	Safe_Release(m_pRigid);
}

void CImpulseCube::Set_Info(const _vec3& StartPos)
{
	m_pTransform->Set_Pos(StartPos);
}
