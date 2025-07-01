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
	HRESULT Ready_Dummy();
	int		Update_Dummy(const float& fTimeDelta);
	void	LateUpdate_Dummy(const float& fTimeDelta);
	void	Render_GameObject();

	static DummyCube* Create_Dummy(LPDIRECT3DDEVICE9 pGraphicDev);
	void Free();


private:
	LPDIRECT3DDEVICE9	m_pGraphicDev;
	CRcCube*			m_pCube;
	const _matrix*				m_mWorld;
	//CTransform*			m_pTransform;

};

