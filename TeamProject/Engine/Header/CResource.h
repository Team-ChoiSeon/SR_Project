#pragma once
#include "CBase.h"
#include "Engine_Define.h"
BEGIN(Engine)

class ENGINE_DLL CResource : public CBase
{
protected:
	explicit CResource();
	explicit CResource(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CResource(const CResource& rhs);

public:
	virtual ~CResource();

public:
	virtual void Free() override;
public:
	LPDIRECT3DDEVICE9 Get_Device() const { return m_pGraphicDev; }

protected:
	LPDIRECT3DDEVICE9			m_pGraphicDev;

};
END
