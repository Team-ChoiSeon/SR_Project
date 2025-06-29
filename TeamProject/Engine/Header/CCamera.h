#pragma once
#include "CComponent.h"

BEGIN(Engine)

class ENGINE_DLL CCamera : public CComponent
{
private:
	explicit CCamera();
	explicit CCamera(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CCamera(const CCamera& rhs);

public:
	virtual ~CCamera();

public:
	HRESULT		Ready_Camera();
	virtual void Update_Camera(const _float& fTimeDelta);
	virtual void LateUpdate_Camera();

	//Computing
	void ComputeLookRightUpVectors();
	void ComputeViewMatrix();
	void ComputeProjMatrix();

	//Create, Release
	virtual CComponent* Clone();
	static CCamera* Create(LPDIRECT3DDEVICE9 pGraphicDev);

	virtual void Free();

public:			//Getter, Setter
	const _vec3 Get_Eye() const { return m_vEye; }
	const _vec3 Get_At() const { return m_vAt; }
	const _vec3 Get_Up() const { return m_vUp; }
	const _vec3 Get_Look() const { return m_vLook; }
	const _vec3 Get_Right() const { return m_vRight; }
	const float Get_Fov() const { return m_fFov; }
	const float Get_Aspect() const { return m_fAspect; }
	const float Get_Near() const { return m_fNear; }
	const float Get_Far() const { return m_fFar; }
	const _matrix Get_ViewMatrix() const { return m_mView; }
	const _matrix Get_ProjectionMatrix() const { return m_mProj; }

	void Set_Eye(const _vec3& vEye) { m_vEye = vEye; }
	void Set_At(const _vec3& vAt) { m_vAt = vAt; }
	void Set_Up(const _vec3& vUp) { m_vUp = vUp; }
	void Set_Look(const _vec3& vLook) { m_vLook = vLook; }
	void Set_Right(const _vec3& vRight) { m_vRight = vRight; }
	void Set_Fov(const float& fFov) { m_fFov = fFov; }
	void Set_Aspect(const float& fAspect){ m_fAspect = fAspect; }
	void Set_Near(const float& fNear) { m_fNear = fNear; }
	void Set_Far(const float& fFar) { m_fFar = fFar; }
	void Set_ViewMatrix(const _vec3& vEye, const _vec3& vAt, const _vec3& vUp) { m_vEye = vEye; m_vAt = vAt; m_vUp = vUp; }
	void Set_ProjectionMatrix(const float& fFov, const float& fAspect, const float& fNear, const float& fFar) { m_fFov = fFov; m_fAspect = fAspect; m_fNear = fNear; m_fFar = fFar; }

private:
	_vec3 m_vEye;
	_vec3 m_vAt;
	_vec3 m_vUp;
	_vec3 m_vLook;
	_vec3 m_vRight;
	
	float m_fFov;
	float m_fAspect;
	float m_fNear;
	float m_fFar;

	_matrix		m_mView;
	_matrix		m_mProj;

private:
};

END
