#include "pch.h"
#include "CCinematicCamera.h"
#include "CTransform.h"
#include "CCamera.h"
#include "CFactory.h"

CCinematicCamera::CCinematicCamera(LPDIRECT3DDEVICE9 pGraphicDev)
	: CCameraObject(pGraphicDev)
{
	m_pTransform = Get_Component<CTransform>();
	m_pCamera = Get_Component<CCamera>();

	m_pTransform->Set_Pos({ 0.f, 0.f, 0.f });
	m_pTransform->Set_Angle({ 0.f, 0.f, 0.f });
	m_pTransform->Set_Scale({ 1.f, 1.f, 1.f });
	m_pTransform->Set_Look({ 0.f, 0.f, 1.f });
	m_pTransform->Set_Up({ 0.f, 1.f, 0.f });
	m_pTransform->Set_Right({ 1.f, 0.f, 0.f });

	m_pCamera->Set_Eye(m_pTransform->Get_Pos());
	m_pCamera->Set_At(m_pTransform->Get_Pos() + m_pTransform->Get_Info(INFO_LOOK));
	m_pCamera->Set_Up(m_pTransform->Get_Info(INFO_UP));
	m_pCamera->Set_Look(m_pTransform->Get_Info(INFO_LOOK));
	m_pCamera->Set_Right(m_pTransform->Get_Info(INFO_RIGHT));
	m_pCamera->Set_Fov(D3DX_PI * 0.25f);
	m_pCamera->Set_Aspect(WINCX / (WINCY * 1.f));
	m_pCamera->Set_Near(0.1f);
	m_pCamera->Set_Far(1000.f);

	Set_Target(this);

	CFactory::Save_Prefab(this, "CCinematicCamera");
}

CCinematicCamera::CCinematicCamera(const CCinematicCamera& rhs)
	: CCameraObject(rhs)
{
}

CCinematicCamera::~CCinematicCamera()
{
	Free();
}

HRESULT CCinematicCamera::Ready_GameObject()
{
	return S_OK;
}

int CCinematicCamera::Update_GameObject(const _float& fTimeDelta)
{

	if (m_bCinematic)
	{
		m_fTimer += fTimeDelta;
		_float timeRatio = min(m_fTimer / m_fDuration, 1.f);

		_vec3 lerpPos = m_StartPos + (m_EndPos - m_StartPos) * timeRatio;
		m_pTransform->Set_Pos(lerpPos);

		_vec3 lerpLook = m_StartLookDir + (m_EndLookDir - m_StartLookDir) * timeRatio;
		D3DXVec3Normalize(&lerpLook, &lerpLook);
		m_pTransform->Set_Look(lerpLook);

		_float originFov = D3DX_PI * 0.25f * (timeRatio);
		_float lerpedFov = (1.f - timeRatio) * originFov + timeRatio * m_fCinematicFov;
		m_pCamera->Set_Fov(originFov);

		m_pCamera->Set_Eye(lerpPos);
		m_pCamera->Set_At(lerpPos + lerpLook);

		if (timeRatio >= 1.f)
		{
			m_bCinematic = false;
		}
	}



	m_pCamera->AngleClamping();

	CGameObject::Update_GameObject(fTimeDelta);

	return 0;
}

void CCinematicCamera::LateUpdate_GameObject(const _float& fTimeDelta)
{
	CGameObject::LateUpdate_GameObject(fTimeDelta);
}

void CCinematicCamera::Start_Cinematic(const _vec3& startPos, const _vec3& endPos, const _vec3& endLook, _float fov, _float duration)
{
	m_StartPos = startPos;
	m_pTransform->Set_Pos(startPos);
	m_EndPos = endPos;

	_vec3 startLook = m_EndPos - m_StartPos;
	D3DXVec3Normalize(&startLook, &startLook);

	m_pTransform->Set_Look(startLook);
	m_StartLookDir = startLook;
	m_EndLookDir = endLook;

	m_fCinematicFov = fov;
	m_fDuration = duration;
	m_fTimer = 0.f;
	m_bCinematic = true;

	m_pCamera->Set_Eye(m_StartPos);
	m_pCamera->Set_At(m_StartPos + startLook);


}

void CCinematicCamera::End_Cinematic()
{
	m_bCinematic = false;
	m_pCamera->Set_Fov(D3DX_PI * 0.25f);
}

void CCinematicCamera::Set_Pos(const _vec3& pos)
{
	m_pTransform->Set_Pos(pos);
}

void CCinematicCamera::Set_Angle(const _vec3& angle)
{
	m_pTransform->Set_Angle(angle);
}

CCinematicCamera* CCinematicCamera::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	auto pCam = new CCinematicCamera(pGraphicDev);
	if (FAILED(pCam->Ready_GameObject()))
	{
		Safe_Release(pCam);
		return nullptr;
	}
	return pCam;
}

void CCinematicCamera::Free()
{
	Safe_Release(m_pCamera);
	Safe_Release(m_pTransform);
}

REGISTER_GAMEOBJECT(CCinematicCamera)
