#pragma once
#include "pch.h"
#include "CProjectile.h"

#include "CModel.h"
#include "CTransform.h"
#include "CRigidBody.h"
#include "CCollider.h"
#include "CParticle.h"
#include "CPickTarget.h"

#include "CPickingMgr.h"
#include "CInputMgr.h"
#include "CSceneMgr.h"
#include "CCameraMgr.h"
#include "CMainPlayer.h"

#include "CFactory.h"

CProjectile::CProjectile(LPDIRECT3DDEVICE9 pGraphicDev)
	: CGameObject(pGraphicDev)
{
}

CProjectile::CProjectile(const CProjectile& rhs)
	: CGameObject(rhs.m_pGraphicDev)
{
}

CProjectile::~CProjectile()
{
}

HRESULT CProjectile::Ready_GameObject()
{
	Add_Component<CModel>(ID_DYNAMIC, m_pGraphicDev);
	m_pModel = Get_Component<CModel>();

	Add_Component<CTransform>(ID_DYNAMIC, m_pGraphicDev);
	m_pTransform = Get_Component<CTransform>();
	m_pTransform->Set_Scale({ 1.f, 1.f, 1.f });

	Add_Component<CRigidBody>(ID_DYNAMIC, m_pGraphicDev, m_pTransform);
	m_pRigid = Get_Component<CRigidBody>();
	m_pRigid->Set_OnGround(false);
	m_pRigid->Set_UseGravity(true);
	m_pRigid->Set_Bounce(0);
	m_pRigid->Set_Friction(1);
	m_pRigid->Set_Mass(1.f);

	Add_Component<CCollider>(ID_DYNAMIC, m_pGraphicDev, m_pRigid);
	m_pCol = Get_Component<CCollider>();
	m_pCol->Set_ColTag(ColliderTag::MONSTER);
	m_pCol->Set_ColType(ColliderType::ACTIVE);
	m_pCol->Set_BoundType(BoundingType::AABB);


	Add_Component<CParticle>(ID_DYNAMIC, m_pGraphicDev);
	m_pParticle = Get_Component<CParticle>();
	m_pParticle->Set_Texture(L"projectile.png");
	m_pParticle->PreSet_Fire(300, 0.03f, 1.f);
	m_pParticle->Set_Speed(2.f);
	m_pParticle->Set_Size(3.f);

	Add_Component<CPickTarget>(ID_DYNAMIC, m_pGraphicDev, RAY_AABB);
	m_pPickTarget = Get_Component<CPickTarget>();
	m_pPickTarget->Set_Active(false);

	CFactory::Save_Prefab(this, "CProjectile");
	return CGameObject::Ready_GameObject();
}

_int CProjectile::Update_GameObject(const _float& fTimeDelta)
{
	switch (m_eState)
	{
	case EProjectileState::MSHOT:
	case EProjectileState::PSHOT:
		m_fLifeTime -= fTimeDelta;
		if (m_fLifeTime <= 0.f)
			return 1;

		if (m_pCol->Get_ColState() == ColliderState::ENTER || m_pCol->Get_ColState() == ColliderState::STAY)
		{
			CCollider* pOther = m_pCol->Get_Other();
			if (pOther && pOther->Get_ColTag() == ColliderTag::GROUND)
			{
				m_eState = EProjectileState::GROUND;
				m_pRigid->Stop_Motion();
				m_pRigid->Set_UseGravity(false);
				m_pPickTarget->Set_Active(true);
			}
		}
		break;

	case EProjectileState::GROUND:
	{
		if (m_bGrab)
		{
			m_eState = EProjectileState::HOLD;
			m_pPickTarget->Set_Active(false);
		}
	}
		break;

	case EProjectileState::HOLD:
	{
		auto pInputMgr = CInputMgr::Get_Instance();
		auto pCameraMgr = CCameraMgr::Get_Instance();
		if (!pInputMgr || !pCameraMgr) break;

		m_pRigid->Set_Velocity({ 0.f, 0.f, 0.f });
		m_pRigid->Set_UseGravity(false);
		m_pCol->Set_ColType(ColliderType::PASSIVE);

		if (m_bGrab && m_pPickerTransform)
		{
			const _vec3& vPlayerPos = m_pPickerTransform->Get_Pos();
			const _vec3& vPlayerLook = m_pPickerTransform->Get_Info(INFO_LOOK);
			_vec3 vNewPos = vPlayerPos + (vPlayerLook * 1.5f) + _vec3(0.f, 0.5f, 0.f);
			m_pTransform->Set_Pos(vNewPos);
			m_pTransform->Set_Look(vPlayerLook);
		}

		if (!m_bGrab && pInputMgr->Mouse_Away(DIM_LB))
		{
			CGameObject* pMainCamObj = pCameraMgr->Get_MainCamera();
			if (pMainCamObj)
			{
				CTransform* pCamTransform = pMainCamObj->Get_Component<CTransform>();
				if (pCamTransform)
				{
					_vec3 vLook = pCamTransform->Get_Info(INFO_LOOK);
					D3DXVec3Normalize(&vLook, &vLook);
					Throw(vLook, 50.f);
				}
			}
		}
	}
	break;
	}

	CGameObject::Update_GameObject(fTimeDelta);
	return 0;
}

void CProjectile::LateUpdate_GameObject(const _float& fTimeDelta)
{
	CGameObject::LateUpdate_GameObject(fTimeDelta);
}

CProjectile* CProjectile::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	auto pPro = new CProjectile(pGraphicDev);
	if (FAILED(pPro->Ready_GameObject())) {
		Safe_Release(pPro);
		return nullptr;
	}
	return pPro;
}

void CProjectile::Free()
{
	Safe_Release(m_pModel);
	Safe_Release(m_pTransform);
	Safe_Release(m_pRigid);
	Safe_Release(m_pCol);
	Safe_Release(m_pParticle);
	Safe_Release(m_pPickTarget);
	CGameObject::Free();
}

void CProjectile::Pick(CTransform* pTransform)
{
	if (m_eState != EProjectileState::GROUND)
		return;

	m_eState = EProjectileState::HOLD;
	m_pPickerTransform = pTransform;
	m_pPickTarget->Set_Active(false);
	//m_pParticle->Set_Active(false);
}

void CProjectile::Throw(const _vec3& vDir, const _float& fSpeed)
{
	if (m_eState != EProjectileState::HOLD)
		return;

	m_eState = EProjectileState::PSHOT;
	m_pPickerTransform = nullptr;

	m_pRigid->Set_UseGravity(true);
	m_pCol->Set_ColType(ColliderType::ACTIVE);
	m_pCol->Set_ColTag(ColliderTag::ATTACK);
	m_pRigid->Set_Velocity(vDir * fSpeed);

	m_fLifeTime = 15.f;
	//m_pParticle->Set_Active(true);
}

bool CProjectile::Check_Ray(_float& fDist)
{
	auto pPickingMgr = CPickingMgr::Get_Instance();
	if (!pPickingMgr || !m_pCol) return false;

	Ray* pRay = pPickingMgr->Get_Ray();
	if (!pRay) return false;

	const AABB& worldAABB = m_pCol->Get_AABBW();
	const _vec3& rayPos = pRay->_position;
	const _vec3& rayDir = pRay->_direction;

	// 레이-AABB 교차 테스트 (Slab Test)
	float tMin = 0.0f;
	float tMax = FLT_MAX;

	for (int i = 0; i < 3; ++i)
	{
		// C-style 포인터 연산 대신 배열 인덱싱처럼 각 축에 접근
		float ray_pos_axis = *(&rayPos.x + i);
		float ray_dir_axis = *(&rayDir.x + i);
		float box_min_axis = *(&worldAABB.vMin.x + i);
		float box_max_axis = *(&worldAABB.vMax.x + i);

		// 레이가 현재 축에 평행한 경우 (분모가 0이 되는 것을 방지)
		if (fabs(ray_dir_axis) < 1e-6f)
		{
			// 레이의 시작점이 AABB 슬랩(Slab) 밖에 있다면 교차하지 않음
			if (ray_pos_axis < box_min_axis || ray_pos_axis > box_max_axis)
			{
				return false; // 교차 실패
			}
		}
		else
		{
			// 현재 축의 슬랩과 교차하는 두 점의 t값 계산
			float t1 = (box_min_axis - ray_pos_axis) / ray_dir_axis;
			float t2 = (box_max_axis - ray_pos_axis) / ray_dir_axis;

			// t1이 항상 t2보다 작도록 정렬 (t1: 가까운 쪽, t2: 먼 쪽)
			if (t1 > t2) std::swap(t1, t2);

			// 모든 축에서 공통으로 교차하는 구간을 찾음
			// tMin은 가장 먼 '진입' 지점, tMax는 가장 가까운 '탈출' 지점
			tMin = max(tMin, t1);
			tMax = min(tMax, t2);

			// 교차 구간이 존재하지 않으면 충돌하지 않음
			if (tMin > tMax)
			{
				return false; // 교차 실패
			}
		}
	}

	// tMax < 0 이면 AABB가 완전히 레이의 시작점 뒤에 있다는 의미
	if (tMax < 0.0f)
	{
		return false;
	}

	// 교차 지점까지의 거리를 계산하여 반환합니다.
	// 만약 tMin > 0 이면, 레이 시작점이 박스 밖에 있습니다. 거리는 tMin 입니다.
	// 만약 tMin <= 0 이면, 레이 시작점이 박스 안에 있습니다. 거리는 0으로 간주하는 것이 직관적입니다.
	fDist = (tMin > 0.f) ? tMin : 0.f;

	return true;
}

void CProjectile::Set_Grab(bool bGrab)
{
	m_bGrab = bGrab;


	if (bGrab)
	{
		if (m_eState == EProjectileState::GROUND)
		{
			auto pPlayer = static_cast<CMainPlayer*>(CSceneMgr::Get_Instance()->Get_Player());
			if (pPlayer)
				m_pPickerTransform = pPlayer->Get_Component<CTransform>();
		}
	}
	else
	{
		m_pPickerTransform = nullptr;
	}
}

REGISTER_GAMEOBJECT(CProjectile);