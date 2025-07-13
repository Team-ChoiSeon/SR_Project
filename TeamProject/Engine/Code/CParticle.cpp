#include "CParticle.h"
#include "CGameObject.h"
#include "CTransform.h"
#include "CRenderMgr.h"
#include "CCameraMgr.h"
#include "CCamera.h"
#include "CTexture.h"


CParticle::CParticle(LPDIRECT3DDEVICE9 pGraphicDev)
	: CComponent(pGraphicDev)
{
}

CParticle::~CParticle()
{
}

CParticle* CParticle::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CParticle* instance = new CParticle(pGraphicDev);

	if (FAILED(instance->Ready_Component())) {
		Safe_Release(instance);
		instance = nullptr;
	}

	return instance;
}

HRESULT CParticle::Ready_Component()
{
	m_vecParticles.resize(m_iMaxParticles); //파티클 생성
	m_pGraphicDev->CreateVertexBuffer(
		sizeof(VTXPARTICLE) * 6 * m_iMaxParticles,
		D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY,
		FVF_PARTICLE,
		D3DPOOL_DEFAULT,
		&m_pVB,
		nullptr);
	return S_OK;
}

void CParticle::Update_Component(const _float& fTimeDelta)
{

	switch (m_iMovementType)
	{
	case 0: m_MoveType = PARTICLE_MOVE_TYPE::FIRE;break;
	case 1: m_MoveType = PARTICLE_MOVE_TYPE::DUST;break;
	}

	if (m_pOwner)
		m_vPos = m_pOwner->Get_Component<CTransform>()->Get_Pos() + m_vOffset;


	for (auto& particle : m_vecParticles)
	{
		if (!particle.bActive) continue;

		particle.fAge += fTimeDelta; // 생존 시간 증가

		if (particle.fAge >= particle.fLifeTime)
		{
			particle.bActive = false;
			continue;
		}
		// 알파 감소
		float lifeRatio = 1.f - (particle.fAge / particle.fLifeTime); // 1 → 0
		lifeRatio = max(0.f, min(1.f, lifeRatio)); // 안전한 클램프

		BYTE r = (BYTE)((particle.color >> 16) & 0xFF);
		BYTE g = (BYTE)((particle.color >> 8) & 0xFF);
		BYTE b = (BYTE)((particle.color) & 0xFF);
		BYTE a = (BYTE)(255.f * lifeRatio); // 점점 줄어듦

		particle.color = D3DCOLOR_ARGB(a, r, g, b);

		switch (particle.moveType)
		{
		case PARTICLE_MOVE_TYPE::FIRE:
			particle.vVelocity.y += 0.1f * fTimeDelta; // 위로 가속
			particle.fSize += 0.3f * fTimeDelta; // 점점 커짐
			break;
		case PARTICLE_MOVE_TYPE::DUST:
			particle.vVelocity.y -= 0.1f * fTimeDelta; // 중력
			particle.fSize -= 0.1f * fTimeDelta;
			break;
		default:
			break;
		}

		particle.vPos += particle.vVelocity * fTimeDelta;
	}
}

void CParticle::LateUpdate_Component(const _float& fTimeDelta)
{

	m_fElapsedTime += fTimeDelta;
	if (m_fElapsedTime >= m_fSpawnInterval)
	{
		Emit_Particle();
		m_fElapsedTime = 0.f;
	}

	CRenderMgr::Get_Instance()->Add_ParticleRenderer(this);
}

void CParticle::Render_Particle()
{
	if (!m_pGraphicDev || !m_pTexture || !m_pVB) return;


	m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CW);

	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pGraphicDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	// 색상은 텍스처 RGB만
	m_pGraphicDev->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
	m_pGraphicDev->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);

	// 알파는 텍스처 × 정점 알파
	m_pGraphicDev->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	m_pGraphicDev->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	m_pGraphicDev->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);

	// 카메라 정보
	CCamera* pCamera = CCameraMgr::Get_Instance()->Get_MainCamera()->Get_Component<CCamera>();
	if (!pCamera) return;

	D3DXMATRIX identity;
	D3DXMatrixIdentity(&identity);
	m_pGraphicDev->SetTransform(D3DTS_WORLD, &identity); // 모든 파티클에 동일 적용

	D3DXVECTOR3 camPos = pCamera->Get_Eye();

	// 텍스처 및 FVF 설정
	m_pGraphicDev->SetTexture(0, m_pTexture->Get_Texture());
	m_pGraphicDev->SetFVF(FVF_PARTICLE);

	// 정점 버퍼 Lock
	VTXPARTICLE* pVertices = nullptr;
	if (FAILED(m_pVB->Lock(0, 0, (void**)&pVertices, D3DLOCK_DISCARD)))
		return;

	int vtxCount = 0;

	for (const auto& particle : m_vecParticles)
	{
		if (!particle.bActive) continue;

		const float half = particle.fSize * 0.5f;
		const D3DXVECTOR3& c = particle.vPos;

		// 빌보드 회전 벡터 계산
		D3DXVECTOR3 vLook = camPos - c;
		D3DXVec3Normalize(&vLook, &vLook);

		D3DXVECTOR3 vUp(0.f, 1.f, 0.f);
		D3DXVECTOR3 vRight;
		D3DXVec3Cross(&vRight, &vUp, &vLook);
		D3DXVec3Normalize(&vRight, &vRight);

		// 다시 up 보정 (직교화)
		D3DXVECTOR3 vTrueUp;
		D3DXVec3Cross(&vTrueUp, &vLook, &vRight);

		// 정점 6개 (두 삼각형)
		pVertices[vtxCount++] = { c - vRight * half - vTrueUp * half,	particle.color ,{0.f, 1.f} };
		pVertices[vtxCount++] = { c - vRight * half + vTrueUp * half,	particle.color ,{0.f, 0.f} };
		pVertices[vtxCount++] = { c + vRight * half + vTrueUp * half,	particle.color ,{1.f, 0.f} };

		pVertices[vtxCount++] = { c - vRight * half - vTrueUp * half,	particle.color ,{0.f, 1.f} };
		pVertices[vtxCount++] = { c + vRight * half + vTrueUp * half,	particle.color ,{1.f, 0.f} };
		pVertices[vtxCount++] = { c + vRight * half - vTrueUp * half,	particle.color ,{1.f, 1.f} };
	}

	m_pVB->Unlock();

	// 그리기
	m_pGraphicDev->SetStreamSource(0, m_pVB, 0, sizeof(VTXPARTICLE));
	m_pGraphicDev->DrawPrimitive(D3DPT_TRIANGLELIST, 0, vtxCount / 3);

	// 상태 복구
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	m_pGraphicDev->SetTexture(0, nullptr);
}

void CParticle::Emit_Particle()
{
	for (auto& particle : m_vecParticles)
	{
		if (!particle.bActive)
		{
			particle.bActive = true;
			particle.fLifeTime = m_fLifeTime;
			particle.fSize = m_fSize;
			particle.fAge = 0;
			particle.moveType = m_MoveType;
			particle.color = m_BaseColor; // 주황 계열

			switch (particle.moveType)
			{
			case PARTICLE_MOVE_TYPE::FIRE:
				particle.vVelocity = {
					randRange(-0.6f, 0.6f),
					randRange(1.0f, 2.0f),
					randRange(-0.4f, 0.4f)
				};
				break;
			case PARTICLE_MOVE_TYPE::DUST:
				particle.vVelocity = {
					randRange(-0.3f, 0.3f),
					randRange(0.1f, 0.3f),
					randRange(-0.3f, 0.3f)
				};
				break;
			default:
				break;
			}
			particle.vPos = m_vPos; //오브젝트 위치
			break; // 한 개만 emit
		}
	}
}

wstring CParticle::Get_Texture()
{
	if (m_pTexture)
		return m_pTexture->Get_Key();

	return L"";
}

void CParticle::Set_Texture(const wstring& key)
{
	if (!key.empty()) {
		m_pTexture = CResourceMgr::Get_Instance()->Load_Texture(key);
	}
}

float CParticle::randRange(float min, float max)
{
	float t = (float)rand() / (float)RAND_MAX; // 0.0f ~ 1.0f
	return min + t * (max - min);
}

void CParticle::Free()
{
	Safe_Release(m_pVB);
	Safe_Release(m_pGraphicDev);
}
