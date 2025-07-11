#include "CTransform.h"
#include "CGameObject.h"

class CCamera;
class CUI;

CTransform::CTransform()
	: m_vScale(1.f, 1.f, 1.f), m_vAngle(0.f, 0.f, 0.f), m_vPosition(0.f, 0.f, 0.f)
{
	ZeroMemory(m_vInfo, sizeof(m_vInfo));
	D3DXMatrixIdentity(&m_matWorld);
	D3DXMatrixIdentity(&m_matParent);
	D3DXMatrixIdentity(&m_matOrbit);
	D3DXMatrixIdentity(&m_matRot);
}

CTransform::CTransform(LPDIRECT3DDEVICE9 pGraphicDev)
	: CComponent(pGraphicDev), m_vScale(1.f, 1.f, 1.f), m_vAngle(0.f, 0.f, 0.f), m_vPosition(0.f, 0.f, 0.f)
{
	ZeroMemory(m_vInfo, sizeof(m_vInfo));
	D3DXMatrixIdentity(&m_matWorld);
	D3DXMatrixIdentity(&m_matParent);
	D3DXMatrixIdentity(&m_matOrbit);
	D3DXMatrixIdentity(&m_matRot);
}

CTransform::CTransform(const CTransform& rhs)
	: CComponent(rhs),
	m_vScale(rhs.m_vScale), m_vAngle(rhs.m_vAngle), m_vPosition(rhs.m_vPosition),
	m_matWorld(rhs.m_matWorld), m_matParent(rhs.m_matParent), m_matOrbit(rhs.m_matOrbit)
{
	for (_uint i = 0; i < INFO_END; ++i)
		m_vInfo[i] = rhs.m_vInfo[i];
}

CTransform::~CTransform()
{
}


HRESULT CTransform::Ready_Transform()
{
	D3DXMatrixIdentity(&m_matWorld);
	D3DXMatrixIdentity(&m_matParent);
	D3DXMatrixIdentity(&m_matOrbit);
	D3DXMatrixIdentity(&m_matRot);

	m_vScale = _vec3(1.f, 1.f, 1.f);
	m_vAngle = _vec3(0.f, 0.f, 0.f);
	m_vPosition = _vec3(0.f, 0.f, 0.f);

	return S_OK;
}

void CTransform::Update_Component(const _float& fTimeDelta)
{
	if (m_pOwner == nullptr)	return;
	_matrix matScale, matRot, matTrans;

	D3DXMatrixScaling(&matScale, m_vScale.x, m_vScale.y, m_vScale.z);
	D3DXMatrixTranslation(&matTrans, m_vPosition.x, m_vPosition.y, m_vPosition.z);

	if (m_pOwner->Has_Component<CCamera>() || m_pOwner->Has_Component<CUI>())
	{
		// 오일러 회전
		_matrix matRotX, matRotY, matRotZ;
		D3DXMatrixRotationX(&matRotX, m_vAngle.x);
		D3DXMatrixRotationY(&matRotY, m_vAngle.y);
		D3DXMatrixRotationZ(&matRotZ, m_vAngle.z);
		matRot = matRotX * matRotY * matRotZ;
	}
	else
	{
		// 회전축 누적 회전 사용
		matRot = m_matRot;
	}

	m_matWorld = matScale * matRot * matTrans * m_matOrbit * m_matParent;

	for (_uint i = 0; i < INFO_POS; ++i)
		memcpy(&m_vInfo[i], &m_matWorld.m[i][0], sizeof(_vec3));

	return ;
}

void CTransform::LateUpdate_Component()
{

}

void CTransform::Rotate_Axis(const _vec3& axis, const _float& fAngle)
{
	_matrix matRot;
	D3DXMatrixRotationAxis(&matRot, &axis, fAngle);

	// 회전 행렬 누적 (주의: scale/translation은 건들지 않음)
	m_matRot = matRot * m_matRot; // 누적 회전만 저장
}

CTransform* CTransform::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CTransform* pTransform = new CTransform(pGraphicDev);

	if (FAILED(pTransform->Ready_Transform()))
	{
		Safe_Release(pTransform);
		MSG_BOX("Transform Create Failed");
		return nullptr;
	}

	return pTransform;
}

void CTransform::Free()
{
	Safe_Release(m_pGraphicDev);

	//Safe_Release(m_pOwner);
}
