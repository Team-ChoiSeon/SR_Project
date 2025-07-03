#pragma once
#include "CGameObject.h"
#include "CTransform.h"


namespace Engine
{
	class CCubeTex;
	class CVIBuffer;
}
class Player : public Engine::CGameObject
{
private:
	Player(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~Player();

public:
	//Basic Function
	HRESULT Ready_GameObject();
	int Update_GameObject(const _float& fTimeDelta) override;
	void LateUpdate_GameObject(const _float& fTimeDelta) override;
	void Render_GameObject() override;

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
	CVIBuffer* m_pVIBuffer;
	CTransform* m_pTransform;

	const _matrix* m_mWorld;

	float m_fMoveSpeed;


	VTXCOL*		vertecies;
	INDEX32*	indecies;

	//temp
	LPDIRECT3DVERTEXBUFFER9		m_pVB;
	LPDIRECT3DINDEXBUFFER9		m_pIB;


	DWORD			m_dwVtxCnt;
	DWORD			m_dwVtxSize;
	DWORD			m_dwFVF;
	DWORD			m_dwTriCnt;
	DWORD			m_dwIdxSize;
	D3DFORMAT		m_IdxFmt;

	float m_fWidth;
	float m_fDepth;

	bool m_bCursorMove;
};