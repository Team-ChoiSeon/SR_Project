#pragma once
#include "CBase.h"
#include "CGameObject.h"
#include "CModel.h"
#include "CCollider.h"

BEGIN(Engine)

class ENGINE_DLL CRenderMgr : public CBase
{
	DECLARE_SINGLETON(CRenderMgr)

private:
	explicit CRenderMgr();
	virtual ~CRenderMgr();

public:
	HRESULT Ready_RenderMgr();
	void Render(LPDIRECT3DDEVICE9 pDevice);

public:
	void Add_Model(CModel* model);
	void Remove_Model(CModel* model);
	void Add_Collider(CCollider* collider);
	void Remove_Collider(CCollider* collider);
	void Clear();

private:
	vector<list<CModel*>> m_vModellist;
	vector<CCollider*> m_vCol;

private:
	virtual void Free() override;

};

END

