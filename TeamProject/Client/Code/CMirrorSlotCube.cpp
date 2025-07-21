#include "pch.h"
#include "CMirrorSlotCube.h"
#include "CSlotCube_Auto.h"
#include "CMainPlayer.h"
#include "CTransform.h"
#include "CFactory.h"

CMirrorSlotCube::CMirrorSlotCube(LPDIRECT3DDEVICE9 pGraphicDev)
	: CSlotCube_Auto(pGraphicDev)
{
}

CMirrorSlotCube::CMirrorSlotCube(const CMirrorSlotCube& rhs)
	: CSlotCube_Auto(rhs)
{
}

CMirrorSlotCube::~CMirrorSlotCube()
{
}

HRESULT CMirrorSlotCube::Ready_GameObject()
{
	CSlotCube_Auto::Ready_GameObject();

	CFactory::Save_Prefab(this, "CMirrorSlotCube");
	return S_OK;
}

_int CMirrorSlotCube::Update_GameObject(const _float& fTimeDelta)
{
	CSlotCube_Auto::Update_GameObject(fTimeDelta);

	if (m_bFollow && m_pPlayer)
		MirrorFollow(fTimeDelta);

	return 0;
}

void CMirrorSlotCube::LateUpdate_GameObject(const _float& fTimeDelta)
{
	CSlotCube_Auto::LateUpdate_GameObject(fTimeDelta);
}

void CMirrorSlotCube::MirrorFollow(const _float& fTimeDelta)
{
	if (!m_pPlayer)
		return;

	CTransform* pPlayerTrans = m_pPlayer->Get_Component<CTransform>();
	if (!pPlayerTrans)
		return;

	_vec3 playerPos = pPlayerTrans->Get_Pos();
	_vec3 myPos = m_pTransform->Get_Pos();

	_vec3 targetPos = myPos;

	if (m_bInvertX)
		targetPos.x = myPos.x - (playerPos.x - myPos.x);
	else
		targetPos.x = playerPos.x;

	if (m_bInvertZ)
		targetPos.z = myPos.z - (playerPos.z - myPos.z);
	else
		targetPos.z = playerPos.z;

	_float followSpeed = 5.f;
	_vec3 newPos;
	D3DXVec3Lerp(&newPos, &myPos, &targetPos, followSpeed * fTimeDelta);

	m_pTransform->Set_Pos(newPos);
}

CMirrorSlotCube* CMirrorSlotCube::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CMirrorSlotCube* pInstance = new CMirrorSlotCube(pGraphicDev);
	if (FAILED(pInstance->Ready_GameObject()))
	{
		Safe_Release(pInstance);
		MSG_BOX("CMirrorSlotCube Create Failed");
		return nullptr;
	}
	return pInstance;
}

void CMirrorSlotCube::Free()
{
	CSlotCube_Auto::Free();
}
