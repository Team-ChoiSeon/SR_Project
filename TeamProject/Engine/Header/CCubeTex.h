#pragma once
#include "CMesh.h"

BEGIN(Engine)

class ENGINE_DLL CCubeTex : public CMesh
{
private:
	explicit CCubeTex();
	explicit CCubeTex(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CCubeTex(const CCubeTex& rhs);
public:
	virtual ~CCubeTex();

public:
	virtual HRESULT	Ready_Buffer();
	virtual void	Render_Buffer();

public:
	static CCubeTex* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void		Free()override;

};

END

