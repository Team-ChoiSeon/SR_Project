#include "pch.h"
#include "CDirectionalCube.h"
#include "CTransform.h"
#include "CPickTarget.h"
#include "CRigidBody.h"
#include "CFactory.h"

#include "CFactory.h"
CDirectionalCube::CDirectionalCube(LPDIRECT3DDEVICE9 pGraphicDev)
	: CCube(pGraphicDev)
{
}

CDirectionalCube::CDirectionalCube(const CDirectionalCube& rhs)
	: CCube(rhs.m_pGraphicDev)
{
}

CDirectionalCube::~CDirectionalCube()
{
}

HRESULT CDirectionalCube::Ready_GameObject()
{
	Add_Component<CTransform>(ID_DYNAMIC, m_pGraphicDev);
	Add_Component<CModel>(ID_DYNAMIC, m_pGraphicDev);
	Add_Component<CPickTarget>(ID_DYNAMIC, m_pGraphicDev, RAY_AABB);
	Add_Component<CRigidBody>(ID_DYNAMIC, m_pGraphicDev, Get_Component<CTransform>());
	Add_Component<CCollider>(ID_DYNAMIC, m_pGraphicDev, Get_Component<CRigidBody>());


	m_pTransform = Get_Component<CTransform>();
	m_pTransform->Set_Scale({ 1.f, 1.f, 0.2f });
	m_pTransform->Set_Pos({ 0.f, 0.f, 0.f });
	m_pTransform->Set_Look({ 0.f, 0.f, 1.f });

	m_pRigid = Get_Component<CRigidBody>();
	m_pModel = Get_Component<CModel>();
	m_pPick = Get_Component<CPickTarget>();
	m_pCollider = Get_Component<CCollider>();
	m_pCollider->Set_ColTag(ColliderTag::NONE);
	m_pCollider->Set_ColType(ColliderType::ACTIVE);

	CFactory::Save_Prefab(this, "CDirectionalCube");

	return S_OK;
}

_int CDirectionalCube::Update_GameObject(const _float& fTimeDelta)
{
	Move();
	for (auto& pComponent : m_umComponent[ID_DYNAMIC])
		pComponent.second->Update_Component(fTimeDelta);
	return S_OK;
}

void CDirectionalCube::LateUpdate_GameObject(const _float& fTimeDelta)
{
	for (auto& pComponent : m_umComponent[ID_DYNAMIC])
		pComponent.second->LateUpdate_Component(fTimeDelta);
}

CDirectionalCube* CDirectionalCube::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CDirectionalCube* pDirCube = new CDirectionalCube(pGraphicDev);
	if (FAILED(pDirCube->Ready_GameObject()))
	{
		Safe_Release(pDirCube);
		MSG_BOX("DirCube Create Failed");
		return nullptr;
	}
	return pDirCube;
}

void CDirectionalCube::Free()
{
	Safe_Release(m_pPick);
	Safe_Release(m_pModel);
	Safe_Release(m_pTransform);
	Safe_Release(m_pRigid);
	Safe_Release(m_pCollider);
	Safe_Release(m_pGraphicDev);
}

void CDirectionalCube::Set_Info(const _vec3& startpos, const _vec3& axis, const _float& mindistance, const _float& maxdistance)
{
	m_vStartPos = startpos;
	m_pTransform->Set_Pos(m_vStartPos);
	D3DXVec3Normalize(&m_vDefaultAxis, &axis);
	m_fMinDistance = mindistance; 
	m_fMaxDistance = maxdistance;
	m_vMoveDelta = { 0.f, 0.f, 0.f };
	m_vCursorDelta = { 0.f, 0.f, 0.f };
	m_bOneway = false;
	ComputeMinMaxPos();

	CFactory::Save_Prefab(this, "CDirectionalCube");
}

void CDirectionalCube::Set_Info(const _vec3& startpos, const _vec3& axis, const _float& maxdistance)
{
	m_vStartPos = startpos;
	m_pTransform->Set_Pos(m_vStartPos);
	D3DXVec3Normalize(&m_vDefaultAxis, &axis);
	m_fMaxDistance = maxdistance;
	m_vMoveDelta = { 0.f, 0.f, 0.f };
	m_vCursorDelta = { 0.f, 0.f, 0.f };
	m_bOneway = true;
	ComputeEndPos();

}


void CDirectionalCube::ComputeMinMaxPos()
{
	m_vMinPos = m_vStartPos + m_fMinDistance * m_vDefaultAxis;
	m_vMaxPos = m_vStartPos + m_fMaxDistance * m_vDefaultAxis;
}

void CDirectionalCube::ComputeEndPos()
{
	D3DXVec3Normalize(&m_vDefaultAxis, &m_vDefaultAxis);
	m_vMaxPos = m_vStartPos + m_vDefaultAxis * m_fMaxDistance;
}

void CDirectionalCube::Move()
{
	if (!m_bOneway)
	{
		if (m_bGrab)
		{
			ComputeMoveVecIntoAxisMoveVec();
			m_pTransform->Set_Pos(m_pTransform->Get_Pos() + m_vMoveDelta);
		}

		_vec3 NowPos = m_pTransform->Get_Pos();
		_vec3 MinGap = m_vMinPos - NowPos;
		_vec3 MaxGap = m_vMaxPos - NowPos;
		_float SumDistance = m_fMaxDistance - m_fMinDistance;
		if (D3DXVec3Length(&MaxGap) >= SumDistance)
		{
			m_pTransform->Set_Pos(m_vMinPos);
		}
		else if (D3DXVec3Length(&MinGap) >= SumDistance)
		{
			m_pTransform->Set_Pos(m_vMaxPos);
		}
	}
	else
	{
		if (m_bGrab)
		{
			ComputeMoveVecIntoAxisMoveVec();
			_float fDelta = D3DXVec3Dot(&m_vDefaultAxis, &m_vCursorDelta);

			if (fDelta > 0.f)
				m_pTransform->Set_Pos(m_pTransform->Get_Pos() + m_vMoveDelta);
		}
		_vec3 vNowPos = m_pTransform->Get_Pos();
		_vec3 vNowGap = vNowPos - m_vStartPos;
		_vec3 vEndGap = m_vMaxPos - m_vStartPos;
		_vec3 vStartGap = m_vStartPos - vNowPos;
		_vec3 vMaxNowGap = m_vMaxPos - vNowPos;

		if (D3DXVec3Length(&vNowGap) >= D3DXVec3Length(&vEndGap))
		{
			m_pTransform->Set_Pos(m_vMaxPos);
		}
		else if (D3DXVec3Length(&vMaxNowGap) >= D3DXVec3Length(&vEndGap))
		{
			m_pTransform->Set_Pos(m_vStartPos);
		}
	}

	m_pRigid->Set_Velocity({ 0.f, 0.f, 0.f });
}

void CDirectionalCube::ComputeMoveVecIntoAxisMoveVec()
{
	m_vMoveDelta = m_vDefaultAxis * (D3DXVec3Dot(&m_vDefaultAxis, &m_vCursorDelta));
}


REGISTER_GAMEOBJECT(CDirectionalCube)