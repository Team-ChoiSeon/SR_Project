#pragma once
#include "CBase.h"
#include "CGameObject.h"
#include "CModel.h"

BEGIN(Engine)

class ENGINE_DLL CRenderMgr : public CBase
{
	DECLARE_SINGLETON(CRenderMgr)

private:
	explicit CRenderMgr();
	virtual ~CRenderMgr();

public:
	HRESULT Ready_RenderMgr(LPDIRECT3DDEVICE9 pDevice);
	void Render(LPDIRECT3DDEVICE9 pDevice);

public:
	void Add_Model(CModel* model);
	void Remove_Model(CModel* model);
	void Clear();

private:
	unordered_map<RENDER_PASS, list<CModel*>> m_Modellist;

private:
	virtual void Free() override;

};

END

