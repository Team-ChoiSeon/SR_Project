#pragma once
#include "CGameObject.h"
#include "CTransform.h"

namespace Engine {
	class CModel;
	class CCollider;
}
class Player : public CGameObject
{
private:
	Player(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~Player();

public:
	//Basic Function
	HRESULT Ready_GameObject();
	int Update_GameObject(const _float& fTimeDelta) override;
	void LateUpdate_GameObject(const _float& fTimeDelta) override;

	//Create, Release Function
	static Player* Create(LPDIRECT3DDEVICE9 pGraphicDev);
	void Free();

	//Gettter, Setter Function
	_vec3 GetPos() { return Get_Component<CTransform>()->Get_Pos(); }


protected:
	//Utility Function
	void KeyInput(const _float& fTimeDelta);
	void CursorRotate();

private:
	CTransform* m_pTransform;
	CModel* m_pModel;
	CCollider* m_pCollider;

	float m_fMoveSpeed;

	float m_fWidth;
	float m_fDepth;

	bool m_bCursorMove;
};