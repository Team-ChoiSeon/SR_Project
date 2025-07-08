#pragma once
#include "CComponent.h"

BEGIN(Engine)

class CRigidBody;

enum class ColliderType		{ PASSIVE, ACTIVE, TRIGGER };
enum class ColliderTag		{ NONE, GROUND, WALL, PLATFORM };
enum class BoundingType		{ AABB, OBB };
enum class ColliderState	{ NONE, ENTER, STAY, EXIT };

struct BoundingInfo
{
	BoundingType eType = BoundingType::AABB;
	_vec3 vMin;  // AABB��
	_vec3 vMax;

	_vec3 vCenter;  // OBB��
	_vec3 vAxisX, vAxisY, vAxisZ; // ���� ����
	_vec3 vHalf; // ������ ����

	// World ��ȯ�� ���� �ڳʵ� (����/�浹��)
	vector<_vec3> vCorners;

	void Calc_Transform(const _matrix& matWorld);
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
	void Set_Offset(const _vec3& vOffset)
	{
		m_tAABBOff.vMin = -vOffset;
		m_tAABBOff.vMax = vOffset;
	}

	ColliderType Get_ColType()		const { return m_eType; }
	ColliderTag  Get_ColTag()		const { return m_eTag; }
	ColliderState Get_ColState()	const { return m_eState; }
	const AABB&  Get_AABBW()			  { return m_tAABBWorld; }
	const AABB& Get_Offset()		const { return m_tAABBOff; }

public:
	virtual void Update_Component(const _float& fTimeDelta) override;
	virtual void LateUpdate_Component() override;
	void Render(LPDIRECT3DDEVICE9 pDevice);
	// pCollider : other
	// ex) change color, set flag
	void On_Collision_Enter(CCollider* pCollider);
	void On_Collision_Stay(CCollider* pCollider);
	void On_Collision_Exit(CCollider* pCollider);
	bool Calc_Push(const AABB& a, const AABB& b, _vec3& push);
	void Calc_Transform(const _matrix& matWorld);
	virtual void Free();

private:
	AABB m_tAABB = { {-1,-1,-1},{1,1,1} };
	AABB m_tAABBOff = { {0, 0, 0}, {0, 0, 0} };
	AABB m_tAABBWorld;

	LPDIRECT3DVERTEXBUFFER9 m_pVB = nullptr;
	LPDIRECT3DINDEXBUFFER9  m_pIB = nullptr;

	ColliderType m_eType = ColliderType::ACTIVE;
	ColliderTag m_eTag	 = ColliderTag::NONE;
	ColliderState m_eState = ColliderState::NONE;

	CRigidBody* m_pRigid = nullptr;
};

END

