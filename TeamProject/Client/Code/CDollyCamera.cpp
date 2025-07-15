#include "pch.h"
#include "CDollyCamera.h"
#include "CTransform.h"
#include "CCamera.h"
#include "CFactory.h"

CDollyCamera::CDollyCamera(LPDIRECT3DDEVICE9 pGraphicDev)
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

	CFactory::Save_Prefab(this, "CCinematicCamera");
}

CDollyCamera::CDollyCamera(const CDollyCamera& rhs)
	: CCameraObject(rhs)
{
}

CDollyCamera::~CDollyCamera()
{
	Free();
}

HRESULT CDollyCamera::Ready_GameObject()
{
	return S_OK;
}

int CDollyCamera::Update_GameObject(const _float& fTimeDelta)
{
	auto Lerp = [](float a, float b, float t)
		{
			return a + (b - a) * t;
		};

	if (m_bDolly)
	{
		m_fTimer += fTimeDelta;
		_float timeRatio = min(m_fTimer / m_fDuration, 1.f);
		timeRatio = timeRatio * timeRatio * (3.f - 2.f * timeRatio);

		if (m_CurPhase == 0)
		{
			_vec3 lerpPos = m_StartPos + (m_MiddlePos - m_StartPos) * timeRatio;
			m_pTransform->Set_Pos(lerpPos);

			if (m_fDollyFovDelta != 0.f)
			{
				_float newFov = Lerp(m_OriginFov, m_OriginFov - m_fDollyFovDelta, timeRatio);
				m_pCamera->Set_Fov(newFov);
			}

			m_pCamera->Set_Eye(lerpPos);
			m_pCamera->Set_At(m_vTargetPos);

			if (timeRatio >= 1.f)
			{
				m_fTimer = 0.f;
				m_CurPhase = 1;
			}
		}
		else if (m_CurPhase == 1)
		{
			_vec3 lerpPos = m_MiddlePos + (m_EndPos - m_MiddlePos) * timeRatio;
			m_pTransform->Set_Pos(lerpPos);

			if (m_fDollyFovDelta != 0.f)
			{
				_float newFov = Lerp(m_OriginFov - m_fDollyFovDelta, m_OriginFov + m_fDollyFovDelta, timeRatio);
				m_pCamera->Set_Fov(newFov);
			}

			m_pCamera->Set_Eye(lerpPos);
			m_pCamera->Set_At(m_vTargetPos);

			if (timeRatio >= 1.f)
			{
				End_Dolly();
			}
		}
	}

	m_pCamera->AngleClamping();
	CGameObject::Update_GameObject(fTimeDelta);
	return 0;
}

void CDollyCamera::LateUpdate_GameObject(const _float& fTimeDelta)
{
	CGameObject::LateUpdate_GameObject(fTimeDelta);
}

void CDollyCamera::Start_Dolly(CGameObject* pTarget, const _vec3& moveOffset, _float fovDelta, _float duration)
{
	m_pTargetTransform = pTarget->Get_Component<CTransform>();
	m_vTargetPos = m_pTargetTransform->Get_Pos();

	m_StartPos = m_pTransform->Get_Pos();
	m_MoveOffset = moveOffset;

	m_MiddlePos = m_StartPos + m_MoveOffset;
	m_EndPos = m_StartPos;

	m_CurPhase = 0;
	m_OriginFov = D3DX_PI * 0.25f;
	m_fDollyFovDelta = fovDelta;

	m_fDuration = duration * 0.5f;
	m_fTimer = 0.f;
	m_bDolly = true;

	m_pCamera->Set_Fov(m_OriginFov);
}

void CDollyCamera::End_Dolly()
{
	m_bDolly = false;
	m_pCamera->Set_Fov(D3DX_PI * 0.25f);
}

void CDollyCamera::Set_Pos(const _vec3& pos)
{
	m_pTransform->Set_Pos(pos);
}

void CDollyCamera::Set_Angle(const _vec3& angle)
{
	m_pTransform->Set_Angle(angle);
}

CDollyCamera* CDollyCamera::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	auto pCam = new CDollyCamera(pGraphicDev);
	if (FAILED(pCam->Ready_GameObject()))
	{
		Safe_Release(pCam);
		return nullptr;
	}
	return pCam;
}

void CDollyCamera::Free()
{
	Safe_Release(m_pCamera);
	Safe_Release(m_pTransform);
}

REGISTER_GAMEOBJECT(CDollyCamera)

