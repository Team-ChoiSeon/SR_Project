#pragma once
#include "CGameObject.h"
#include "CMonsterPart.h"

#include "CTransform.h"
#include "CModel.h"

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

private:
	int m_iPartCnt = 6;
	vector<CMonsterPart*> m_vPart;
};

