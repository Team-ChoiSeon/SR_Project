#pragma once
#include "CCube.h"

class CActiveTrigger : public CCube
{
protected:
	explicit CActiveTrigger(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CActiveTrigger(const CActiveTrigger& rhs);
	virtual ~CActiveTrigger();

public:
	// Basic Function
	virtual HRESULT Ready_GameObject() override = 0;
	virtual _int Update_GameObject(const _float& fTimeDelta) override = 0;
	virtual void LateUpdate_GameObject(const _float& fTimeDelta) override = 0;

	// Create, Release Function
	virtual void Free() override =0;

	// Getter, Setter Function
	const _bool Get_TriggerState() const { return m_bTriggerOn; }
protected:
	// Compute Function
	void ComputeConditions();

	// Variables
	_float m_fTimeLimit;
	_float m_fCount;
	_float m_fWeight;
	_float m_fRecycleCount;
	vector<int> m_vecSequence;

	_bool m_bTimeLimit;
	_bool m_bWeight;
	_bool m_bCount;
	_bool m_bRecycleCount;
	_bool m_bSequence;

	_bool m_bTriggerOn;
};

