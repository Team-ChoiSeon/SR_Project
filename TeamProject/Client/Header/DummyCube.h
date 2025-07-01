#pragma once
#include "CGameObject.h"

namespace Engine {
	class CRcCube;
	class CTransform;
}
class DummyCube : public CGameObject
{
private :
	explicit DummyCube(LPDIRECT3DDEVICE9 pGrpahicDev);
	virtual ~DummyCube();

public:
	HRESULT Ready_GameObject();
	int Update_GameObject(const _float& fTimeDelta) override;
	void LateUpdate_GameObject(const _float& fTimeDelta) override;
	void Render_GameObject() override;


	static DummyCube* Create(LPDIRECT3DDEVICE9 pGraphicDev);
	void Free();


private:
	LPDIRECT3DDEVICE9	m_pGraphicDev;
	CRcCube*			m_pCube;
	const _matrix*				m_mWorld;
	//CTransform*			m_pTransform;

};

