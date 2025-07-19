#include "pch.h"
#include "CSlotCube_Auto.h"
#include "CTransform.h"
#include "CRigidBody.h"
#include "CCollider.h"
#include "CSlotSensor.h"
#include "CFactory.h"

CSlotCube_Auto::CSlotCube_Auto(LPDIRECT3DDEVICE9 pGraphicDev)
	: CCube(pGraphicDev)
{
}

CSlotCube_Auto::CSlotCube_Auto(const CSlotCube_Auto& rhs)
	: CCube(rhs.m_pGraphicDev)
{
}

CSlotCube_Auto::~CSlotCube_Auto()
{
}

HRESULT CSlotCube_Auto::Ready_GameObject()
{
	Add_Component<CTransform>(ID_DYNAMIC, m_pGraphicDev);
	m_pTransform = Get_Component<CTransform>();
	m_pTransform->Ready_Component();
	m_pTransform->Set_Look({ 0.f, 0.f, 1.f });
	m_pTransform->Set_Scale({ 1.f, 1.f, 1.f });

	Add_Component<CModel>(ID_DYNAMIC, m_pGraphicDev);
	m_pModel = Get_Component<CModel>();

	Add_Component<CRigidBody>(ID_DYNAMIC, m_pGraphicDev, m_pTransform);
	m_pRigid = Get_Component<CRigidBody>();
	m_pRigid->Set_Friction(0.7f);
	m_pRigid->Set_Mass(1.f);
	m_pRigid->Set_Bounce(0.3f);
	m_pRigid->Set_UseGravity(true);

	Add_Component<CCollider>(ID_DYNAMIC, m_pGraphicDev, m_pRigid);
	m_pCollider = Get_Component<CCollider>();
	m_pCollider->Set_ColTag(ColliderTag::NONE);
	m_pCollider->Set_ColType(ColliderType::ACTIVE);
	m_pCollider->Set_BoundType(BoundingType::AABB);

	m_FitSlot = nullptr;

	CFactory::Save_Prefab(this, "CSlotCube_Auto");

	return S_OK;
}

_int CSlotCube_Auto::Update_GameObject(const _float& fTimeDelta)
{
	if (Check_Overlap())
	{
		Fit(fTimeDelta);
	}
	else if (m_FitSlot != nullptr)
	{
		m_FitSlot->Set_SlottedCube(nullptr);
	}

	CGameObject::Update_GameObject(fTimeDelta);
	return 0;
}

void CSlotCube_Auto::LateUpdate_GameObject(const _float& fTimeDelta)
{
	CGameObject::LateUpdate_GameObject(fTimeDelta);
}

CSlotCube_Auto* CSlotCube_Auto::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CSlotCube_Auto* pInstance = new CSlotCube_Auto(pGraphicDev);
	if (FAILED(pInstance->Ready_GameObject()))
	{
		Safe_Release(pInstance);
		MSG_BOX("CSlotCube_Auto Create Failed");
		return nullptr;
	}
	return pInstance;
}

void CSlotCube_Auto::Free()
{
	Safe_Release(m_pTransform);
	Safe_Release(m_pModel);
	Safe_Release(m_pRigid);
	Safe_Release(m_pCollider);
	Safe_Release(m_pGraphicDev);
}

void CSlotCube_Auto::Set_Info(CMainPlayer* player, const _int puzzleID, const _int slotID)
{
	m_pPlayer = player;
	m_iPuzzleID = puzzleID;
	m_iSlotID = slotID;
}

void CSlotCube_Auto::Insert_Overlap(CSlotSensor* sensor, _float dist)
{
	m_vecDetected_Slot.insert(make_pair(sensor, dist));
}

_bool CSlotCube_Auto::Check_Overlap()
{
	if (m_vecDetected_Slot.empty())
		return false;

	auto Slotiter = std::min_element(
		m_vecDetected_Slot.begin(), m_vecDetected_Slot.end(),
		[](auto const& a, auto const& b) {
			return a.second < b.second;
		});

	m_FitSlot = Slotiter->first;
	m_bSlotted = true;
	return true;
}

void CSlotCube_Auto::Fit(const _float& fTimeDelta)
{
	m_FitSlot->Set_SlottedCube_Auto(this);

	m_pTransform->Set_Pos(m_FitSlot->Get_Component<CTransform>()->Get_Pos());
	m_pRigid->Set_UseGravity(false);
	m_pRigid->Set_Velocity({ 0.f, 0.f, 0.f });

	// 축 정렬 (회전)
	_float AllignSpeed = 3.f;
	_vec3 SensorLook = m_FitSlot->Get_Look();
	_vec3 CubeLook = m_pTransform->Get_Info(INFO_LOOK);

	_vec3 axis;
	D3DXVec3Cross(&axis, &CubeLook, &SensorLook);

	if (D3DXVec3LengthSq(&axis) < 1e-6f)
		return;

	D3DXVec3Normalize(&axis, &axis);

	float cosA = D3DXVec3Dot(&CubeLook, &SensorLook);
	cosA = cosf(max(-1.f, min(1.f, cosA)));
	float fullAngle = acosf(cosA);

	float step = AllignSpeed * fTimeDelta;
	if (step > fullAngle)
		step = fullAngle;

	m_pTransform->Rotate_Axis(axis, step);
}

REGISTER_GAMEOBJECT(CSlotCube_Auto)
