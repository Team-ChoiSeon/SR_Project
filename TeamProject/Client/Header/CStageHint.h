#pragma once
#include "CGameObject.h"

class CHints;
namespace Engine {
	class CTransform;
	class CUiQuad;
	class CCamera;
}

class CStageHint :
    public CGameObject
{
private:
	explicit CStageHint(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CStageHint();
public:
	static CStageHint* Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual			HRESULT		Ready_GameObject();
	virtual			_int		Update_GameObject(const _float& fTimeDelta);
	virtual			void		LateUpdate_GameObject(const _float& fTimeDelta);

public:
	void Add_Hint(const wstring& hint);
private:
	_bool m_bOpen;
	vector<CHints*> m_vecHint;

private: 
	void Free();
};

