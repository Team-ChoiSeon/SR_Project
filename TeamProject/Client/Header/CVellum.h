#pragma once
#include "CGameObject.h"
#include "CMonsterPart.h"

#include "CTransform.h"
#include "CModel.h"

enum class VPattern { IDLE, WANDER, CHASE, AROUND, BAF };

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
	void Wander(const _float& fTimeDelta, CTransform* pTransform, CRigidBody* pRigid);
	void Chase_Player(const _float& fTimeDelta, CTransform* pTransform, CRigidBody* pRigid);
	void Around_Player(const _float& fTimeDelta, CTransform* pTransform);

private:
	int m_iPartCnt = 6;
	vector<CMonsterPart*> m_vPart;

	_float m_fPatternTime = 0.f;
	_float m_fSwitchTime = 0.f;
	VPattern m_eCuPattern = VPattern::IDLE;
	bool m_bPattern = true;


};

