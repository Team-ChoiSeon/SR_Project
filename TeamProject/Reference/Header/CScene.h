#pragma once
#include "CBase.h"
#include "CLayer.h"
#include "CComponent.h"

BEGIN(Engine)

class ENGINE_DLL CScene : public CBase
{
protected:
	explicit CScene(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CScene();

public:
	virtual			HRESULT		Ready_Scene();
	virtual			_int		Update_Scene(const _float& fTimeDelta);
	virtual			void		LateUpdate_Scene(const _float& fTimeDelta);
	virtual			void		Render_Scene();

protected:
	unordered_map<LAYERID, CLayer*>			m_umLayer;
	LPDIRECT3DDEVICE9						m_pGraphicDev;

protected:
	virtual void			Free();

};

END
