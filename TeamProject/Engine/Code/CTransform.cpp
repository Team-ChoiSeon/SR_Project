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


HRESULT CTransform::Ready_Component()
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

void CTransform::LateUpdate_Component(const _float& fTimeDelta)
{

}

void CTransform::Rotate_Axis(const _vec3& axis, const _float& fAngle)
{
	_matrix matRot;
	D3DXMatrixRotationAxis(&matRot, &axis, fAngle);

	// 회전 행렬 누적 (주의: scale/translation은 건들지 않음)
	m_matRot = matRot * m_matRot; // 누적 회전만 저장
}

void CTransform::Set_Look(const _vec3& vLook)
{
	_vec3 Look;
	D3DXVec3Normalize(&Look, &vLook);

	_vec3 WUp = _vec3(0.f, 1.f, 0.f);
	_vec3 Right;
	D3DXVec3Cross(&Right, &WUp, &Look);

	// 짐벌락 방지
	if (D3DXVec3LengthSq(&Right) < 0.001f)
	{
		// Look이 위를 향할 때 -> Right를 월드 X축으로 설정
		if (Look.y > 0.f)
		{
			_vec3 WForward = _vec3(0.f, 0.f, 1.f);
			D3DXVec3Cross(&Right, &WForward, &Look);
		}
		// Look이 아래를 향할 때 -> Right를 월드 -X축으로 설정
		else
		{
			_vec3 WBack = _vec3(0.f, 0.f, -1.f);
			D3DXVec3Cross(&Right, &WBack, &Look);
		}
	}

	D3DXVec3Normalize(&Right, &Right);

	_vec3 Up;
	D3DXVec3Cross(&Up, &Look, &Right);
	D3DXVec3Normalize(&Up, &Up);

	// 행렬 갱신
	D3DXMatrixIdentity(&m_matRot);
	memcpy(m_matRot.m[0], &Right, sizeof(_vec3));
	memcpy(m_matRot.m[1], &Up, sizeof(_vec3));
	memcpy(m_matRot.m[2], &Look, sizeof(_vec3));

	// m_vInfo 갱신
	m_vInfo[INFO_RIGHT] = Right;
	m_vInfo[INFO_UP] = Up;
	m_vInfo[INFO_LOOK] = Look;
}

CTransform* CTransform::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CTransform* pTransform = new CTransform(pGraphicDev);

	if (FAILED(pTransform->Ready_Component()))
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
