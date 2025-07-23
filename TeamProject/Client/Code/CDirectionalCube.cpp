#include "pch.h"
#include "CDirectionalCube.h"
#include "CTransform.h"
#include "CPickTarget.h"
#include "CRigidBody.h"
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
	Add_Component<CModel>(ID_DYNAMIC, m_pGraphicDev);
	m_pModel = Get_Component<CModel>();

	Add_Component<CTransform>(ID_DYNAMIC, m_pGraphicDev);
	m_pTransform = Get_Component<CTransform>();
	m_pTransform->Ready_Component();
	m_pTransform->Set_Look({ 0.f, 0.f, 1.f });

	Add_Component<CRigidBody>(ID_DYNAMIC, m_pGraphicDev, m_pTransform);
	m_pRigid = Get_Component<CRigidBody>();
	m_pRigid->Set_Friction(0.f);
	m_pRigid->Set_Mass(1.f);
	m_pRigid->Set_Bounce(0.1f);
	m_pRigid->Set_OnGround(true);
	m_pRigid->Set_UseGravity(false);

	Add_Component<CCollider>(ID_DYNAMIC, m_pGraphicDev, m_pRigid);
	m_pCollider = Get_Component<CCollider>();
	m_pCollider->Set_ColTag(ColliderTag::GROUND);
	m_pCollider->Set_ColType(ColliderType::PASSIVE);
	m_pCollider->Set_BoundType(BoundingType::AABB);

	Add_Component<CPickTarget>(ID_DYNAMIC, m_pGraphicDev, RAY_AABB);
	m_pPick = Get_Component<CPickTarget>();

	CFactory::Save_Prefab(this, "CDirectionalCube");

	return S_OK;
}

_int CDirectionalCube::Update_GameObject(const _float& fTimeDelta)
{
	Move();
	CGameObject::Update_GameObject(fTimeDelta);
	return S_OK;
}

void CDirectionalCube::LateUpdate_GameObject(const _float& fTimeDelta)
{
	CGameObject::LateUpdate_GameObject(fTimeDelta);
	_vec3 pos = m_pTransform->Get_Pos();

	if (m_bLockX) pos.x = m_vStartPos.x;
	if (m_bLockY) pos.y = m_vStartPos.y;
	if (m_bLockZ) pos.z = m_vStartPos.z;

	m_pTransform->Set_Pos(pos);

	// velocity도 차단
	_vec3 velocity = m_pRigid->Get_Velocity();
	if (m_bLockX) velocity.x = 0.f;
	if (m_bLockY) velocity.y = 0.f;
	if (m_bLockZ) velocity.z = 0.f;
	m_pRigid->Set_Velocity(velocity);
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
}

void CDirectionalCube::Set_Info(const _vec3& axis, const _float& mindistance, const _float& maxdistance)
{
	m_vStartPos = m_pTransform->Get_Pos();
	D3DXVec3Normalize(&m_vDefaultAxis, &axis);
	m_fMinDistance = mindistance; 
	m_fMaxDistance = maxdistance;
	m_vMoveDelta = { 0.f, 0.f, 0.f };
	m_vCursorDelta = { 0.f, 0.f, 0.f };
	m_bOneway = false;
	ComputeMinMaxPos();

	// 축 고정
	m_bLockX = (fabs(m_vDefaultAxis.x) < 0.0001f);
	m_bLockY = (fabs(m_vDefaultAxis.y) < 0.0001f);
	m_bLockZ = (fabs(m_vDefaultAxis.z) < 0.0001f);
}

void CDirectionalCube::Set_Info( const _vec3& axis, const _float& maxdistance)
{
	m_vStartPos = m_pTransform->Get_Pos();
	D3DXVec3Normalize(&m_vDefaultAxis, &axis);
	m_fMaxDistance = maxdistance;
	m_vMoveDelta = { 0.f, 0.f, 0.f };
	m_vCursorDelta = { 0.f, 0.f, 0.f };
	m_bOneway = true;
	ComputeEndPos();

	// 축 고정
	m_bLockX = (fabs(m_vDefaultAxis.x) < 0.0001f);
	m_bLockY = (fabs(m_vDefaultAxis.y) < 0.0001f);
	m_bLockZ = (fabs(m_vDefaultAxis.z) < 0.0001f);
}

void CDirectionalCube::Set_Info(const _vec3& start, const _vec3& axis, const _float& mindistance, const _float& maxdistance)
{
	m_vStartPos = start;
	m_pTransform->Set_Pos(m_vStartPos);
	D3DXVec3Normalize(&m_vDefaultAxis, &axis);
	m_fMinDistance = mindistance;
	m_fMaxDistance = maxdistance;
	m_vMoveDelta = { 0.f, 0.f, 0.f };
	m_vCursorDelta = { 0.f, 0.f, 0.f };
	m_bOneway = false;
	ComputeMinMaxPos();

	// 축 고정
	m_bLockX = (fabs(m_vDefaultAxis.x) < 0.0001f);
	m_bLockY = (fabs(m_vDefaultAxis.y) < 0.0001f);
	m_bLockZ = (fabs(m_vDefaultAxis.z) < 0.0001f);
}

void CDirectionalCube::Set_Info(const _vec3& start, const _vec3& axis, const _float& maxdistance)
{
	m_vStartPos = start;
	m_pTransform->Set_Pos(m_vStartPos);
	D3DXVec3Normalize(&m_vDefaultAxis, &axis);
	m_fMaxDistance = maxdistance;
	m_vMoveDelta = { 0.f, 0.f, 0.f };
	m_vCursorDelta = { 0.f, 0.f, 0.f };
	m_bOneway = true;
	ComputeEndPos();

	// 축 고정
	m_bLockX = (fabs(m_vDefaultAxis.x) < 0.0001f);
	m_bLockY = (fabs(m_vDefaultAxis.y) < 0.0001f);
	m_bLockZ = (fabs(m_vDefaultAxis.z) < 0.0001f);
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
		if (m_bCurGrab)
		{
			ComputeMoveVecIntoAxisMoveVec();
			m_pTransform->Set_Pos(m_pTransform->Get_Pos() + m_vMoveDelta);
			if (!m_bSoundPlayed)
			{
				CSoundMgr::Get_Instance()->Play("CubeDrag", "SFX", true);
				m_bSoundPlayed = true;
			}
		}
		else
		{
			if (m_bSoundPlayed)
			{
				CSoundMgr::Get_Instance()->Stop("CubeDrag");
				m_bSoundPlayed = false;
			}
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
		if (m_bCurGrab)
		{
			ComputeMoveVecIntoAxisMoveVec();
			_float fDelta = D3DXVec3Dot(&m_vDefaultAxis, &m_vCursorDelta);

			if (fDelta > 0.f)
				m_pTransform->Set_Pos(m_pTransform->Get_Pos() + m_vMoveDelta);
		
			if (!m_bSoundPlayed)
			{
				CSoundMgr::Get_Instance()->Play("CubeDrag", "SFX", true);
				m_bSoundPlayed = true;
			}
		}
		else
		{
			if (m_bSoundPlayed)
			{
				CSoundMgr::Get_Instance()->Stop("CubeDrag");
				m_bSoundPlayed = false;
			}
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

	//m_pRigid->Set_Velocity({ 0.f, 0.f, 0.f });

	_vec3 velocity = m_pRigid->Get_Velocity();
	if (m_bLockX) velocity.x = 0.f;
	if (m_bLockY) velocity.y = 0.f;
	if (m_bLockZ) velocity.z = 0.f;

	m_pRigid->Set_Velocity(velocity);
}

void CDirectionalCube::ComputeMoveVecIntoAxisMoveVec()
{
	m_vMoveDelta = m_vDefaultAxis * (D3DXVec3Dot(&m_vDefaultAxis, &m_vCursorDelta));
}


REGISTER_GAMEOBJECT(CDirectionalCube)