#include "CCameraObject.h"
#include "CTransform.h"
#include "CCamera.h"

CCameraObject::~CCameraObject()
{
}

CCameraObject::CCameraObject(LPDIRECT3DDEVICE9 pGraphicDev) : CGameObject(pGraphicDev)
{
	Add_Component<CTransform>(L"Transform", ID_DYNAMIC, pGraphicDev);
	Add_Component<CCamera>(L"Camera", ID_DYNAMIC, pGraphicDev);
}

CCameraObject::CCameraObject(const CCameraObject& rhs) : CGameObject(rhs.m_pGraphicDev)
{
	Add_Component<CTransform>(L"Transform", ID_DYNAMIC, rhs.m_pGraphicDev);
	Add_Component<CCamera>(L"Camera", ID_DYNAMIC, rhs.m_pGraphicDev);
}


HRESULT CCameraObject::Ready_CameraObject()
{
	return S_OK;
}

int CCameraObject::Update_CameraObject(const _float& fTimeDelta)
{
	return 0;
}

void CCameraObject::LateUpdate_CameraObject(const _float& fTimeDelta)
{
}

void CCameraObject::Render_CameraObject()
{
}

void CCameraObject::Render_GameObject()
{
}

CCameraObject* CCameraObject::Clone()
{
	return new CCameraObject(*this);
}

CGameObject* CCameraObject::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CCameraObject* pCameraObj = new CCameraObject(pGraphicDev);

	if (FAILED(pCameraObj->Ready_CameraObject()))
	{
		Safe_Release(pCameraObj);
		MSG_BOX("Camera Object Create Failed");
		return nullptr;
	}

	return pCameraObj;
}
