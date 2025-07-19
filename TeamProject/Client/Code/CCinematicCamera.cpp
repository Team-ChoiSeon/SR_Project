#include "pch.h"
#include "CCinematicCamera.h"
#include "CTransform.h"
#include "CCamera.h"
#include "CFactory.h"
#include "CInputMgr.h"

CCinematicCamera::CCinematicCamera(LPDIRECT3DDEVICE9 pGraphicDev)
	: CCameraObject(pGraphicDev)
{

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

	CResourceMgr::Get_Instance()->Ready_Resource();
	CResourceMgr::Get_Instance()->Load_Texture(L"UI/CinematicBar.png");

	m_pBlackBarTop = CImageUIObject::Create(m_pGraphicDev);
	m_pBlackBarTop->Set_Texture(CResourceMgr::Get_Instance()->Get_Texture(L"UI/CinematicBar.png"));
	m_pBlackBarTop->Set_RenderSize(820, 100.f);
	//m_pBlackBarTop->Set_Position({ -220.f, -30.f });
	m_pBlackBarTop->Set_Position({ -220.f, -130.f});

	m_pBlackBarBottom = CImageUIObject::Create(m_pGraphicDev);
	m_pBlackBarBottom->Set_Texture(CResourceMgr::Get_Instance()->Get_Texture(L"UI/CinematicBar.png"));
	m_pBlackBarBottom->Set_RenderSize(820, 100.f);
	//m_pBlackBarBottom->Set_Position({ -220.f, 475 });
	m_pBlackBarBottom->Set_Position({ -220.f, 575});

	m_fbarMove = 0.f;
	m_bCinematic = false;

	CFactory::Save_Prefab(this, "CCinematicCamera");

	return S_OK;
}

int CCinematicCamera::Update_GameObject(const _float& fTimeDelta)
{
	if (m_bCinematic)
	{
		m_fbarMove += 100 * fTimeDelta;
		if (m_fbarMove > 100.f)
			m_fbarMove = 100.f;
	}
	else
	{
		m_fbarMove -= 100 * fTimeDelta;
		if (m_fbarMove < 0.f)
			m_fbarMove = 0.f;
	}
	
	float topY = -130.f + m_fbarMove;
	float bottomY = 575.f - m_fbarMove;
	m_pBlackBarTop->Set_Position({ -220.f,  topY });
	m_pBlackBarBottom->Set_Position({ -220.f,  bottomY });


	m_pTransform->Set_Pos(m_pTargetTransform->Get_Pos());
	m_pTransform->Set_Angle(m_pTargetTransform->Get_Angle());
	m_pTransform->Set_Look(m_pTargetTransform->Get_Info(INFO_LOOK));

	m_pCamera->AngleClamping();

	CGameObject::Update_GameObject(fTimeDelta);

	m_pBlackBarTop->Update_GameObject(fTimeDelta);
	m_pBlackBarBottom->Update_GameObject(fTimeDelta);

	return 0;
}

void CCinematicCamera::LateUpdate_GameObject(const _float& fTimeDelta)
{
	CGameObject::LateUpdate_GameObject(fTimeDelta);

	m_pBlackBarTop->LateUpdate_GameObject(fTimeDelta);
	m_pBlackBarBottom->LateUpdate_GameObject(fTimeDelta);
}

void CCinematicCamera::Start_Cinematic()
{
	m_bCinematic = true;
}

void CCinematicCamera::End_Cinematic()
{
	m_bCinematic = false;
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
	Safe_Release(m_pBlackBarTop);
	Safe_Release(m_pBlackBarBottom);

	Safe_Release(m_pCamera);
	Safe_Release(m_pTransform);
}

REGISTER_GAMEOBJECT(CCinematicCamera)
