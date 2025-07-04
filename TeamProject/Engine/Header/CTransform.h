#pragma once
#include "CComponent.h"

BEGIN(Engine)

class ENGINE_DLL CTransform : public CComponent
{
private:
	explicit CTransform();
	explicit CTransform(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CTransform(const CTransform& rhs);

public:
	virtual ~CTransform();

public:
	HRESULT			Ready_Transform();
	virtual void	Update_Component(const _float& fTimeDelta);
	virtual void	LateUpdate_Component();

public:
    // ¼³Á¤
    void Set_Pos(const _vec3& vPos) { m_vPosition = vPos; }
    void Set_PosX(_float x) { m_vPosition.x = x; }
    void Set_PosY(_float y) { m_vPosition.y = y; }
    void Set_PosZ(_float z) { m_vPosition.z = z; }
    void Set_Scale(const _vec3& vScale) { m_vScale = vScale; }
    void Set_ScaleX(_float x) { m_vScale.x = x; }
    void Set_ScaleY(_float y) { m_vScale.y = y; }
    void Set_ScaleZ(_float z) { m_vScale.z = z; }
    void Set_Angle(const _vec3& vAngle) { m_vAngle = vAngle; }
    void Set_AngleX(_float x) { m_vAngle.x = x; }
    void Set_AngleY(_float y) { m_vAngle.y = y; }
    void Set_AngleZ(_float z) { m_vAngle.z = z; }
    void Set_ParentMatrix(const _matrix* pMat) { m_matParent = *pMat; }
    void Set_OrbitMatrix(const _matrix* pMat) { m_matOrbit = *pMat; }
    void Set_Right(const _vec3& vRight) { m_vInfo[INFO_RIGHT] = vRight; }
    void Set_Up(const _vec3& vUp) { m_vInfo[INFO_UP] = vUp; }
    void Set_Look(const _vec3& vLook) { m_vInfo[INFO_LOOK] = vLook; }
    

    // Á¶È¸
    const _vec3& Get_Pos() const { return m_vPosition; }
    const _vec3& Get_Scale() const { return m_vScale; }
    const _vec3& Get_Angle() const { return m_vAngle; }
    const _vec3& Get_Info(INFO eType) const { return m_vInfo[eType]; }

    const _matrix* Get_WorldMatrix() const { return &m_matWorld; }
    const _matrix* Get_ParentMatrix() const { return &m_matParent; }
    const _matrix* Get_OrbitMatrix() const { return &m_matOrbit; }

    // ÀÌµ¿/È¸Àü
    void Move_Pos(const _vec3* pDir, const _float& fSpeed, const _float& fDelta)
    {
        m_vPosition += *pDir * fSpeed * fDelta;
    }

    void Rotate(ROTATION eType, const _float& fAngle)
    {
        *(((_float*)&m_vAngle) + eType) += fAngle;
    }

public:
    static CTransform* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
    virtual void	Free()override;

private:
    _vec3       m_vScale;
    _vec3       m_vAngle;
    _vec3       m_vPosition;
    _vec3       m_vInfo[INFO_END];

    _matrix     m_matWorld;
    _matrix     m_matParent;
    _matrix     m_matOrbit;
};

END
