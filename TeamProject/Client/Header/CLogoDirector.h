#pragma once

#include "CGameObject.h"
#include <vector>
#include <map>

// Foward declaration
class CProjectile;

namespace Engine
{
    class CTransform;
    class CRigidBody;
}

class CLogoDirector : public CGameObject
{
private:
    explicit CLogoDirector(LPDIRECT3DDEVICE9 pGraphicDev);
    virtual ~CLogoDirector() = default;

public:
    // CGameObject 기본 함수
    HRESULT Ready_GameObject() override;
    _int Update_GameObject(const _float& fTimeDelta) override;
    void Free() override;

    // 정적 생성 함수
    static CLogoDirector* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
    // 내부 헬퍼 함수
    void Initialize_FontData();
    void Generate_LogoPoints();
    void Spawn_Projectile(const _vec3& vTargetPos);

private:
    // 투사체 제어를 위한 최소 정보
    struct ProjectileInfo
    {
        CTransform* pTransform = nullptr;
        CRigidBody* pRigidBody = nullptr;
        _vec3       vTargetPos;
        bool        bArrived = false;
    };

private:
    std::map<char, std::vector<_vec2>> m_mapFontData;
    std::vector<ProjectileInfo> m_vecManagedProjectiles;
    std::vector<_vec3> m_vecAllTargetPoints;

    // 스폰 관리
    _float m_fSpawnCooltime = 0.f;
    _uint  m_iSpawnIndex = 0;
    _uint  m_iProjectileCounter = 0; // 고유 이름용 카운터

    // 연출 시작 딜레이
    bool m_bStarted = false;
    _float m_fStartDelay = 1.f;

    // 고정된 연출 설정값
    _float m_fProjectileSpeed;
    _float m_fSpawnRadius;
    _float m_fArrivalThreshold;
};
