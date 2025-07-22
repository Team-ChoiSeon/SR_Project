#include "pch.h"
#include "CMirrorSlotCube.h"
#include "CSlotCube_Auto.h"
#include "CMainPlayer.h"
#include "CTransform.h"
#include "CInputMgr.h"
#include "CCameraMgr.h"

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
	m_pTransform->Set_Scale({ .8f, 2.f, .8f });
	m_pTransform->Set_Pos({ 0.f, 0.f, 0.f });
	m_pTransform->Set_Look({ 0.f, 0.f, 1.f });
	m_pTransform->Set_Up({ 0.f, 1.f, 0.f });
	m_pTransform->Set_Right({ 1.f, 0.f, 0.f });

	m_pRigid->Set_Mass(6.f);
	m_pRigid->Set_Friction(10.f);
	m_pRigid->Set_Gravity(5.f);

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


void CMirrorSlotCube::Set_MirrorPlane(const _vec3& vPlanePos, const _vec3& vPlaneNormal)
{
	m_vPlanePos = vPlanePos;
	m_vPlaneNorm = vPlaneNormal;
	D3DXVec3Normalize(&m_vPlaneNorm, &m_vPlaneNorm);
}

void CMirrorSlotCube::MirrorFollow(const _float& fTimeDelta)
{
	if (!m_pPlayer)
		return;

	CTransform* pPlayerTrans = m_pPlayer->Get_Component<CTransform>();
	_vec3 playerPos = pPlayerTrans->Get_Pos();
	_vec3 toPlane = playerPos - m_vPlanePos;
	
	float projLen = D3DXVec3Dot(&toPlane, &m_vPlaneNorm);
	_vec3 projVec = m_vPlaneNorm * projLen;
	_vec3 mirrorPos = playerPos - projVec * 2.f;
	
	m_pTransform->Set_Pos(mirrorPos);
	m_pTransform->Set_PosY(playerPos.y);
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

REGISTER_GAMEOBJECT(CMirrorSlotCube)


//사용방법
// cMirrorSlotCube->Set_Info(2, 2); -> 슬롯 세팅
// cMirrorSlotCube->Set_Player(pPlayer); -> 플레이어 설정
// cMirrorSlotCube->Set_Follow(true); -> 플레이어위치기반 반전을 할것인지 안할것인지
// cMirrorSlotCube->Set_MirrorPlane(_vec3(-15.f, 0.f, 0.f), _vec3(1.f, 0.f, 0.f)); 
					//첫번째는 반사 평면위치 플레이어 위치 기준으로 - 는 왼쪽 +는 오른쪽 앞뒤도 동일
					//두번째는 반사 할 방향 _vec3(1.f, 0.f, 0.f)); X축 기반, _vec3(0.f, 0.f, 1.f)); Z축 기반