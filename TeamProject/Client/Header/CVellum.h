#pragma once
#include "CGameObject.h"
#include "CMonsterPart.h"

#include "CTransform.h"
#include "CModel.h"

enum class VPattern { CHASE, DIVE};
enum class DivePhase { Ready, In, Wait, Out, None };

class CVellum : public CGameObject
{
private:
	explicit CVellum(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CVellum();

public:
	//Basic Function
	HRESULT Ready_GameObject()override;
	int Update_GameObject(const _float& fTimeDelta) override;
	void LateUpdate_GameObject(const _float& fTimeDelta) override;

	void Key_Input(const _float& fTimeDelta);

	static CVellum* Create(LPDIRECT3DDEVICE9 pGraphicDev);
	void Free();

public:
	void Update_Pattern(const _float& fTimeDelta);
	void Chase(const _float& fTimeDelta, CTransform* pTransform, CRigidBody* pRigid);
	//void Snake_Curve(const _float& fTimeDelta, CTransform* pTransform, int idx);
	void Dive(const _float& fTimeDelta, CTransform* pTransform, CRigidBody* pRigid);

private:
	int m_iPartCnt = 6;
	vector<CMonsterPart*> m_vPart;

	//_float m_fCurveTime = 0.f;

	_float m_fPatternTime = 0.f;
	_float m_fSwitchTime = 0.f;
	VPattern m_eCurPattern;
	bool m_bPattern = false;

	DivePhase m_eDPhase = DivePhase::Ready;
	_float m_fSearch = 0.f;


};

