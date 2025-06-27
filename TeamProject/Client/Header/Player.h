#pragma once
#include "CGameObject.h"
#include "CVIBuffer.h"
#include "CTransform.h"

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
	_vec3 GetVScale() { return m_pTransformCom->Get_Scale(); }
	void SetVScale(_vec3 scale) { m_vScale = scale; }
	void SetVScale(float x, float y, float z) { m_vScale = { x, y, z }; }
	_vec3 GetVRotation() { return m_vRotation; }
	void SetVRotation(_vec3 rot) { m_vRotation = rot; }
	void SetVRotation(float x, float y, float z) { m_vRotation = { x, y, z }; }
	_vec3 GetVTranslation() { return m_vTranslation; }
	void SetVTranslation(_vec3 pos) { m_vTranslation = pos; }
	void SetVTranslation(float x, float y, float z) { m_vTranslation = { x, y, z }; }
	_vec3 GetPos() { return m_vTranslation; }
	void SetPos(_vec3 pos) { m_vTranslation = pos; }
	void SetPos(float x, float y, float z) { m_vTranslation = { x, y, z }; }

protected:
	void ComputeWorldMatirx();
	void KeyInput(const _float& fTimeDelta);

private:
	CVIBuffer* m_pVIBuffer;
	CTransform* m_pTransformCom;

	_vec3 m_vScale;
	_vec3 m_vRotation;
	_vec3 m_vTranslation;

	_matrix m_mScale;
	_matrix m_mRotation;
	_matrix m_mTranslation;

	_matrix m_mWorld;

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
};
//일단 얘 왜 free가 없음? 이거 순수가상아닌가?