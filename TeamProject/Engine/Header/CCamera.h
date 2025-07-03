#pragma once
#include "CComponent.h"

BEGIN(Engine)

class ENGINE_DLL CCamera : public CComponent
{
private:
	explicit CCamera();
	explicit CCamera(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CCamera(const CCamera& rhs);
	virtual ~CCamera();

public:
	//Basic Function
	HRESULT			Ready_Component();
	virtual void	Update_Component(const _float& fTimeDelta);
	virtual void	LateUpdate_Component();

	//Create, Release Function
	static CCamera* Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual void	Free();

	//Optional Function
	void ComputeLookRightUpVectors();

	//Getter, Setter Function
	const _vec3 Get_Eye()					const { return m_vEye; }
	const _vec3 Get_At()					const { return m_vAt; }
	const _vec3 Get_Up()					const { return m_vUp; }
	const _vec3 Get_Look()					const { return m_vLook; }
	const _vec3 Get_Right()					const { return m_vRight; }
	const _vec3 Get_Info(INFO eType)		const { return m_vInfo[eType]; }
	const float Get_Fov()					const { return m_fFov; }
	const float Get_Aspect()				const { return m_fAspect; }
	const float Get_Near()					const { return m_fNear; }
	const float Get_Far()					const { return m_fFar; }
	const _matrix* Get_ViewMatrix()			const { return &m_mView; }
	const _matrix* Get_ProjectionMatrix()	const { return &m_mProj; }

	void Set_Eye				(const _vec3& vEye) { m_vEye = vEye; }
	void Set_Eye				(const float& x, const float& y, const float& z) { m_vEye = { x, y, z }; }
	void Set_At					(const _vec3& vAt) { m_vAt = vAt; }
	void Set_At					(const float& x, const float& y, const float& z) { m_vAt = { x, y, z }; }
	void Set_Up					(const _vec3& vUp) { m_vUp = vUp; m_vInfo[INFO_UP] = vUp; }
	void Set_Up					(const float& x, const float& y, const float& z) { m_vUp = { x, y, z }; m_vInfo[INFO_UP] = { x, y, z };}
	void Set_Look				(const _vec3& vLook) { m_vLook = vLook; m_vInfo[INFO_LOOK] = vLook;}
	void Set_Look				(const float& x, const float& y, const float& z) { m_vLook = { x, y, z }; m_vInfo[INFO_LOOK] = { x, y, z };}
	void Set_Right				(const _vec3& vRight) { m_vRight = vRight; m_vInfo[INFO_RIGHT] = vRight; }
	void Set_Right				(const float& x, const float& y, const float& z) { m_vRight = { x, y, z }; m_vInfo[INFO_RIGHT] = { x, y, z };}
	void Set_Fov				(const float& fFov) { m_fFov = fFov; }
	void Set_Aspect				(const float& fAspect){ m_fAspect = fAspect; }
	void Set_Near				(const float& fNear) { m_fNear = fNear; }
	void Set_Far				(const float& fFar) { m_fFar = fFar; }
	void Set_ViewMatrix			(const _matrix * mView) { m_mView = *mView; }
	void Set_ViewMatrix			(const _vec3& vEye, const _vec3& vAt, const _vec3& vUp) { m_vEye = vEye; m_vAt = vAt; m_vUp = vUp; }
	void Set_ProjectionMatrix	(const _matrix* mProj) { m_mProj = *mProj; }
	void Set_ProjectionMatrix	(const float& fFov, const float& fAspect, const float& fNear, const float& fFar) { m_fFov = fFov; m_fAspect = fAspect; m_fNear = fNear; m_fFar = fFar; }

private:
	//Computing Functions
	void ComputeViewMatrix();
	void ComputeProjMatrix();

	//Variables, Structs
	_matrix		m_mView;
	_matrix		m_mProj;

	_vec3		m_vEye;
	_vec3		m_vAt;
	_vec3		m_vUp;
	_vec3		m_vLook;
	_vec3		m_vRight;
	_vec3		m_vInfo[INFO_END];
	
	float		m_fFov;
	float		m_fAspect;
	float		m_fNear;
	float		m_fFar;
};

END
