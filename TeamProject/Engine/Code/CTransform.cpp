#include "CTransform.h"

CTransform::CTransform()
	: m_vScale(1.f, 1.f, 1.f), m_vAngle(0.f, 0.f, 0.f), m_vPosition(0.f, 0.f, 0.f)
{
	ZeroMemory(m_vInfo, sizeof(m_vInfo));
	D3DXMatrixIdentity(&m_matWorld);
	D3DXMatrixIdentity(&m_matParent);
	D3DXMatrixIdentity(&m_matOrbit);
}

CTransform::CTransform(LPDIRECT3DDEVICE9 pGraphicDev)
	: CComponent(pGraphicDev), m_vScale(1.f, 1.f, 1.f), m_vAngle(0.f, 0.f, 0.f), m_vPosition(0.f, 0.f, 0.f)
{
	ZeroMemory(m_vInfo, sizeof(m_vInfo));
	D3DXMatrixIdentity(&m_matWorld);
	D3DXMatrixIdentity(&m_matParent);
	D3DXMatrixIdentity(&m_matOrbit);
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

	m_vScale = _vec3(1.f, 1.f, 1.f);
	m_vAngle = _vec3(0.f, 0.f, 0.f);
	m_vPosition = _vec3(0.f, 0.f, 0.f);

	return S_OK;
}

void CTransform::Update_Component(const _float& fTimeDelta)
{
	_matrix matScale, matRotX, matRotY, matRotZ, matRot, matTrans;

	// 크기
	D3DXMatrixScaling(&matScale, m_vScale.x, m_vScale.y, m_vScale.z);

	// 회전
	D3DXMatrixRotationX(&matRotX, m_vAngle.x);
	D3DXMatrixRotationY(&matRotY, m_vAngle.y);
	D3DXMatrixRotationZ(&matRotZ, m_vAngle.z);
	matRot = matRotY * matRotX * matRotZ;
	
	// 이동
	D3DXMatrixTranslation(&matTrans, m_vPosition.x, m_vPosition.y, m_vPosition.z);

	m_matWorld = m_matOrbit * matScale * matRot * matTrans * m_matParent;

	for (_uint i = 0; i < INFO_POS; ++i)
		memcpy(&m_vInfo[i], &m_matWorld.m[i][0], sizeof(_vec3));

	return ;
}

void CTransform::LateUpdate_Component()
{

}

CComponent* CTransform::Clone()
{
	return new CTransform(*this);
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
	CComponent::Free();

}
