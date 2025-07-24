#pragma once
#include "pch.h"
#include "CLogoDirector.h"

#include "CProjectile.h"
#include "CSceneMgr.h"
#include "CTransform.h"
#include "CRigidBody.h"
#include "CCollider.h"
#include "CParticle.h"
#include "CModel.h"

#include "CFactory.h"

#include <algorithm>
#include <random>
#include <string>

namespace
{
    // === 고정 크기 설정 ===
    const float FONT_SCALE = 0.2f; // 무조건 작게 시작
    const float CHAR_SPACING = 1.5f * FONT_SCALE;
    const float LINE_SPACING = 6.0f * FONT_SCALE;

    // 투사체 관련 설정
    const float PROJECTILE_SPEED = 25.f;
    const float SPAWN_RADIUS = 20.f;
    const float SPAWN_INTERVAL = 0.01f;
    const float ARRIVAL_DISTANCE_THRESHOLD = 1.0f;
    const float POINT_DENSITY = 0.8f;

    // 텍스트 내용
    const string TOP_LINE_TEXT = "Team";
    const string BOTTOM_LINE_TEXT = "ChoiSeon";
}

CLogoDirector::CLogoDirector(LPDIRECT3DDEVICE9 pGraphicDev)
    : CGameObject(pGraphicDev)
    , m_fProjectileSpeed(PROJECTILE_SPEED)
    , m_fSpawnRadius(SPAWN_RADIUS)
    , m_fArrivalThreshold(ARRIVAL_DISTANCE_THRESHOLD* ARRIVAL_DISTANCE_THRESHOLD)
{
}

HRESULT CLogoDirector::Ready_GameObject()
{
    Initialize_FontData();
    Generate_LogoPoints(); // 고정된 값으로 목표 지점 생성

    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(m_vecAllTargetPoints.begin(), m_vecAllTargetPoints.end(), g);

    return S_OK;
}

_int CLogoDirector::Update_GameObject(const _float& fTimeDelta)
{
    // 연출 시작 딜레이
    if (!m_bStarted)
    {
        m_fStartDelay -= fTimeDelta;
        if (m_fStartDelay <= 0.f) m_bStarted = true;
        return 0;
    }

    // 투사체 생성
    m_fSpawnCooltime -= fTimeDelta;
    if (m_fSpawnCooltime <= 0.f && m_iSpawnIndex < m_vecAllTargetPoints.size())
    {
        Spawn_Projectile(m_vecAllTargetPoints[m_iSpawnIndex]);
        m_iSpawnIndex++;
        m_fSpawnCooltime = SPAWN_INTERVAL;
    }

    // 투사체 유도
    for (auto& info : m_vecManagedProjectiles)
    {
        if (info.bArrived || !info.pTransform || !info.pRigidBody) continue;

        _vec3 vCurrentPos = info.pTransform->Get_Pos();
        _vec3 vDirection = info.vTargetPos - vCurrentPos;
        if (D3DXVec3LengthSq(&vDirection) < m_fArrivalThreshold)
        {
            info.bArrived = true;
            info.pTransform->Set_Pos(info.vTargetPos);
            info.pRigidBody->Set_Velocity({ 0.f, 0.f, 0.f });
            info.pRigidBody->Stop_Motion();
            info.pRigidBody->Set_UseGravity(false);
        }
        else
        {
            D3DXVec3Normalize(&vDirection, &vDirection);
            info.pRigidBody->Set_Velocity(vDirection * m_fProjectileSpeed);
        }
    }

    return CGameObject::Update_GameObject(fTimeDelta);
}

void CLogoDirector::Free()
{
    m_vecManagedProjectiles.clear();
    CGameObject::Free();
}

CLogoDirector* CLogoDirector::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
    CLogoDirector* pInstance = new CLogoDirector(pGraphicDev);
    if (FAILED(pInstance->Ready_GameObject()))
    {
        Safe_Release(pInstance);
        return nullptr;
    }
    return pInstance;
}

void CLogoDirector::Spawn_Projectile(const _vec3& vTargetPos)
{
    CProjectile* pProjectile = CProjectile::Create(m_pGraphicDev);
    if (!pProjectile) return;

    // 씬의 관리 시스템에 등록 (가장 중요!)
    wchar_t szUniqueName[128] = L"";
    swprintf_s(szUniqueName, L"LogoProjectile_%d", m_iProjectileCounter++);
    CSceneMgr::Get_Instance()->Get_Scene()->Get_Layer(LAYER_OBJECT)->Add_GameObject(szUniqueName, pProjectile);

    // 불필요한 컴포넌트 비활성화
    pProjectile->Get_Component<CParticle>()->Set_Active(false);
    pProjectile->Get_Component<CCollider>()->Set_ColType(ColliderType::PASSIVE);

    // 시작 위치 설정
    float fAngle = (rand() % 360) * D3DX_PI / 180.f;
    _vec3 vStartPos = { cosf(fAngle) * m_fSpawnRadius, vTargetPos.y, sinf(fAngle) * m_fSpawnRadius };
    CTransform* pTransform = pProjectile->Get_Component<CTransform>();
    pTransform->Set_Pos(vStartPos);
    pProjectile->Set_LifeTime(999.f);

    // 제어에 필요한 정보만 캐싱
    ProjectileInfo info;
    info.pTransform = pTransform;
    info.pRigidBody = pProjectile->Get_Component<CRigidBody>();
    info.vTargetPos = vTargetPos;
    info.bArrived = false;
    m_vecManagedProjectiles.push_back(info);
}

void CLogoDirector::Generate_LogoPoints()
{
    m_vecAllTargetPoints.clear();
    const _vec3 TOP_LINE_START_POS = { 0.f, LINE_SPACING / 2.f, 0.f };
    const _vec3 BOTTOM_LINE_START_POS = { 0.f, -LINE_SPACING / 2.f, 0.f };

    auto calculate_raw_width = [&](const std::string& text) -> float {
        float totalWidth = 0.f;
        for (size_t i = 0; i < text.length(); ++i) {
            char c = toupper(text[i]);
            if (m_mapFontData.count(c)) {
                float charWidth = 0.f;
                for (const auto& p : m_mapFontData[c]) { if (p.x > charWidth) charWidth = p.x; }
                totalWidth += charWidth * FONT_SCALE;
            }
            if (i < text.length() - 1) { totalWidth += CHAR_SPACING; }
        }
        return totalWidth;
        };

    auto generate_line = [&](const std::string& text, const _vec3& startPos) {
        float totalWidth = calculate_raw_width(text);
        _vec3 currentPos = startPos;
        currentPos.x -= totalWidth / 2.f;

        for (char c : text) {
            c = toupper(c);
            if (m_mapFontData.count(c)) {
                const auto& charPoints = m_mapFontData[c];
                float charWidth = 0.f;
                for (float i = 0; i < charPoints.size() - 1; i += POINT_DENSITY) {
                    int idx1 = static_cast<int>(i);
                    int idx2 = idx1 + 1;
                    _vec2 p1 = charPoints[idx1];
                    _vec2 p2 = charPoints[idx2];
                    _vec2 interpolatedPoint = p1 + (i - idx1) * (p2 - p1);
                    m_vecAllTargetPoints.push_back({
                        currentPos.x + interpolatedPoint.x * FONT_SCALE,
                        currentPos.y,
                        currentPos.z + interpolatedPoint.y * FONT_SCALE
                        });
                }
                for (const auto& p : charPoints) { if (p.x > charWidth) charWidth = p.x; }
                currentPos.x += charWidth * FONT_SCALE + CHAR_SPACING;
            }
        }
        };

    generate_line(TOP_LINE_TEXT, TOP_LINE_START_POS);
    generate_line(BOTTOM_LINE_TEXT, BOTTOM_LINE_START_POS);
}

void CLogoDirector::Initialize_FontData()
{
    m_mapFontData.clear();
    m_mapFontData['T'] = { {0, 4}, {4, 4}, {2, 4}, {2, 0} };
    m_mapFontData['E'] = { {4, 4}, {0, 4}, {0, 2}, {3, 2}, {0, 2}, {0, 0}, {4, 0} };
    m_mapFontData['A'] = { {0, 0}, {0, 2}, {2, 4}, {4, 2}, {4, 0}, {3, 2}, {1, 2} };
    m_mapFontData['M'] = { {0, 0}, {0, 4}, {2, 2}, {4, 4}, {4, 0} };
    m_mapFontData['C'] = { {4, 4}, {1, 4}, {0, 3}, {0, 1}, {1, 0}, {4, 0} };
    m_mapFontData['H'] = { {0, 0}, {0, 4}, {0, 2}, {4, 2}, {4, 4}, {4, 0} };
    m_mapFontData['O'] = { {1, 0}, {0, 1}, {0, 3}, {1, 4}, {3, 4}, {4, 3}, {4, 1}, {3, 0}, {1, 0} };
    m_mapFontData['I'] = { {0, 4}, {2, 4}, {1, 4}, {1, 0}, {0, 0}, {2, 0} };
    m_mapFontData['S'] = { {4, 4}, {1, 4}, {1, 2}, {3, 2}, {3, 0}, {0, 0} };
    m_mapFontData['N'] = { {0, 0}, {0, 4}, {4, 0}, {4, 4} };
}

REGISTER_GAMEOBJECT(CLogoDirector);
