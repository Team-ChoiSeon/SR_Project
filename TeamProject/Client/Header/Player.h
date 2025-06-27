#pragma once
#include "CGameObject.h"
#include "CTransform.h"


namespace Engine
{
	class CCubeTex;
	class CVIBuffer;
}
class Player : public CGameObject
{
public:
	Player(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~Player();

	HRESULT Ready_GameObject();
	int Update_GameObject(const _float& fTimeDelta) override;
	void LateUpdate_GameObject(const _float& fTimeDelta) override;
	void Render_GameObject() override;


public:		//GetSet
	_vec3 GetPos() { return Get_Component<CTransform>(L"Transform")->Get_Pos(); }


protected:
	void KeyInput(const _float& fTimeDelta);

private:
	CVIBuffer* m_pVIBuffer;
	CTransform* m_pTransformCom;

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
};
//일단 얘 왜 free가 없음? 이거 순수가상아닌가?