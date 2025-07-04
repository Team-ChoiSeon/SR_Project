#pragma once
#include "CGameObject.h"
#include "CCollider.h"

namespace Engine {
	class CTransform;
	class CModel;
	class CPickTarget;
	class CCollider;
}
class DummyCube : public CGameObject
{
private :
	explicit DummyCube(LPDIRECT3DDEVICE9 pGrpahicDev);
	virtual ~DummyCube();

public:
	//Basic Function
	HRESULT Ready_GameObject();
	int Update_GameObject(const _float& fTimeDelta) override;
	void LateUpdate_GameObject(const _float& fTimeDelta) override;

	//Create, Release Function
	static DummyCube* Create(LPDIRECT3DDEVICE9 pGraphicDev);
	void Free();

	//Optional Function
	void Key_Input(const float& fTimeDelta);

private:
	CTransform*			m_pTransform;
	CModel*				m_pModel;
	CPickTarget*		m_pPick;
	CCollider*			m_pCollider;


};

