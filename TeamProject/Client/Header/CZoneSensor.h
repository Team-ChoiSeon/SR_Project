#pragma once
#include "CSensor.h"
class CZoneSensor : public CSensor
{
private:
	explicit CZoneSensor(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CZoneSensor(const CZoneSensor& rhs);
	virtual ~CZoneSensor();

public:
	//Basic Function
	HRESULT Ready_GameObject() override;
	_int Update_GameObject(const _float& fTimeDelta) override;
	void LateUpdate_GameObject(const _float& fTimeDelta) override;

	//Create, Release Function
	static CZoneSensor* Create(LPDIRECT3DDEVICE9 pGraphicDev);
	void Free() override;

	//Core Function
	_bool Detect() override;
	_bool DetectObj(CGameObject* gameobj);

	//Getter, Setter Function

private:


};

