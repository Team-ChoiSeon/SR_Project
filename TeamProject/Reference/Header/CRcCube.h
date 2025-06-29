#pragma once
#include "CVIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CRcCube : public CVIBuffer
{
private:
	explicit CRcCube();
	explicit CRcCube(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CRcCube(const CRcCube& rhs);
	virtual ~CRcCube();

public:
	virtual HRESULT	Ready_Buffer();
	virtual void	Render_Buffer();

public:
	static CRcCube* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void		Free();

};

END

