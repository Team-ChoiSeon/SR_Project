#pragma once
#include "CComponent.h"

BEGIN(Engine)

class CRigidBody;

enum class ColliderType		{ PASSIVE, ACTIVE, TRIGGER };
enum class ColliderTag		{ NONE, GROUND, WALL, PLATFORM };
enum class BoundingType		{ AABB, OBB };
enum class ColliderState	{ NONE, ENTER, STAY, EXIT };

struct BoundInfo
{
	BoundingType eType = BoundingType::AABB;

	// AABB��
	_vec3 vMin;  
	_vec3 vMax;
	// OBB��
	_vec3 vCenter;  
	_vec3 vAxisX, vAxisY, vAxisZ; // ���� ����
	_vec3 vHalf; // ������ ����

	// World ��ȯ�� ���� �ڳʵ� (����/�浹��)
	vector<_vec3> vCorners;

	void Calc_Transform(const _matrix& matWorld)
	{
		vCenter = *(_vec3*)&matWorld._41;

		vAxisX = *(_vec3*)&matWorld._11;
		vAxisY = *(_vec3*)&matWorld._21;
		vAxisZ = *(_vec3*)&matWorld._31;

		D3DXVec3Normalize(&vAxisX, &vAxisX);
		D3DXVec3Normalize(&vAxisY, &vAxisY);
		D3DXVec3Normalize(&vAxisZ, &vAxisZ);

		vCorners.clear();
		vCorners.push_back(vCenter + (-vAxisX * vHalf.x + -vAxisY * vHalf.y + -vAxisZ * vHalf.z)); // 0
		vCorners.push_back(vCenter + (vAxisX * vHalf.x + -vAxisY * vHalf.y + -vAxisZ * vHalf.z)); // 1
		vCorners.push_back(vCenter + (vAxisX * vHalf.x + vAxisY * vHalf.y + -vAxisZ * vHalf.z)); // 2
		vCorners.push_back(vCenter + (-vAxisX * vHalf.x + vAxisY * vHalf.y + -vAxisZ * vHalf.z)); // 3
		vCorners.push_back(vCenter + (-vAxisX * vHalf.x + -vAxisY * vHalf.y + vAxisZ * vHalf.z)); // 4
		vCorners.push_back(vCenter + (vAxisX * vHalf.x + -vAxisY * vHalf.y + vAxisZ * vHalf.z)); // 5
		vCorners.push_back(vCenter + (vAxisX * vHalf.x + vAxisY * vHalf.y + vAxisZ * vHalf.z)); // 6
		vCorners.push_back(vCenter + (-vAxisX * vHalf.x + vAxisY * vHalf.y + vAxisZ * vHalf.z)); // 7
	}
};

class ENGINE_DLL CCollider : public CComponent
{
protected:
	explicit CCollider(LPDIRECT3DDEVICE9 pGraphicDev);

public:
	virtual  ~CCollider();

public:
	static CCollider* Create(LPDIRECT3DDEVICE9 pGraphicDev, CRigidBody* pRigid);

	void Set_ColType(ColliderType eType)	{ m_eType = eType; }
	void Set_ColTag(ColliderTag eTag)		{ m_eTag = eTag; }
	void Set_ColState(ColliderState eState) { m_eState = eState; }
	void Set_BoundType(BoundingType eType)  { m_tBound.eType = eType; }
	// �� �� ���������� ������ ����
	void Set_Offset(const _vec3& vOffset)
	{
		m_tAABBOff.vMin = -vOffset;
		m_tAABBOff.vMax = vOffset;

		m_tBound.vHalf = vOffset;
	}

	ColliderType Get_ColType()			const { return m_eType; }
	ColliderTag  Get_ColTag()			const { return m_eTag; }
	ColliderState Get_ColState()		const { return m_eState; }
	const AABB&  Get_AABBW()				  { return m_tAABBWorld; }
	const AABB& Get_Offset()			const { return m_tAABBOff; }
	const BoundInfo& Get_Bound()		const { return m_tBound; }
	const BoundingType& Get_BoundType() const { return m_tBound.eType; }

public:
	virtual void Update_Component(const _float& fTimeDelta) override;
	virtual void LateUpdate_Component() override;
	void Render(LPDIRECT3DDEVICE9 pDevice);
	// pCollider : other
	// ex) change color, set flag
	void On_Collision_Enter(CCollider* pCollider);
	void On_Collision_Stay(CCollider* pCollider);
	void On_Collision_Exit(CCollider* pCollider);

	bool Calc_Push_AABB(const AABB& a, const AABB& b, _vec3& push);
	bool Calc_Push_OBB(const BoundInfo& a, const BoundInfo& b, _vec3& push);

	virtual void Free();

private:
	AABB m_tAABB = { {-1,-1,-1},{1,1,1} };
	AABB m_tAABBOff = { {0, 0, 0}, {0, 0, 0} };
	AABB m_tAABBWorld;

	BoundInfo m_tBound;

	LPDIRECT3DVERTEXBUFFER9 m_pVB = nullptr;
	LPDIRECT3DINDEXBUFFER9  m_pIB = nullptr;

	ColliderType m_eType = ColliderType::ACTIVE;
	ColliderTag m_eTag	 = ColliderTag::NONE;
	ColliderState m_eState = ColliderState::NONE;

	CRigidBody* m_pRigid = nullptr;
};

END

