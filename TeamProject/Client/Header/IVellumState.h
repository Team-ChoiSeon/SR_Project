#pragma once
#include "Engine_Define.h"

class CVellum;

class IVellumState
{
public:
	virtual ~IVellumState() = default;

public:
	virtual void Enter(CVellum* pVellum)PURE;
	virtual void Update(const _float fTimeDelta, CVellum* pVellum)PURE;
	virtual void Exit(CVellum* pVellum)PURE;


};