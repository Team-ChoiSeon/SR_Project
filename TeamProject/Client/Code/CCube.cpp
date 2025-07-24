#include "pch.h"
#include "CCube.h"
#include "CCollider.h"
#include "CRigidBody.h"
#include "CSoundMgr.h"
#include "CGuiSystem.h"
#include "Engine_GUI.h"
#include "CTransform.h"
#include "CMetalCube.h"

CCube::CCube(LPDIRECT3DDEVICE9 pGraphicDev)
    : CGameObject(pGraphicDev), m_pModel(nullptr), m_pTransform(nullptr)
{
}

CCube::~CCube()
{
}

HRESULT CCube::Ready_GameObject()
{
    return E_NOTIMPL;
}

_int CCube::Update_GameObject(const _float& fTimeDelta)
{
    return _int();
}

void CCube::LateUpdate_GameObject(const _float& fTimeDelta)
{
}

_int CCube::Update_Cube(const _float& fTimeDelta)
{


	//auto colstate = Get_Component<CCollider>()->Get_ColState();
	//CGuiSystem::Get_Instance()->RegisterPanel("state", [colstate]() {
	//	// 간단한 GUI 창 하나 출력
	//	ImGui::SetNextWindowSize(ImVec2{ 200,200 });
	//	//if (zonesensor)
	//	//	ImGui::Begin("TRUE");
	//	//else
	//	//	ImGui::Begin("FALSE");
	//	switch (colstate)
	//	{
	//	case ColliderState::ENTER:
	//		ImGui::Begin("ENTER");
	//		break;
	//	case ColliderState::STAY:
	//		ImGui::Begin("STAY");
	//		break;
	//	case ColliderState::EXIT:
	//		ImGui::Begin("EXIT");
	//		break;
	//	case ColliderState::NONE:
	//		ImGui::Begin("NONE");
	//	}
	//	//if (m_pRigid->Get_OnGround())
	//	//    ImGui::Begin("On Ground");
	//	//else if (!m_pRigid->Get_OnGround())
	//	//    ImGui::Begin("Not On Ground");

	//	ImGui::End();
	//	});


    return _int();
}

void CCube::Set_Tap(bool Trigger)
{
	m_bTap = Trigger;
}

void CCube::Set_Away(bool Trigger)
{
    m_bAway = Trigger;
}

void CCube::PlayColSound(int i)
{
	_float fVelClamp = 0.f;
	if (Get_Component<CRigidBody>()->Get_Velocity().y > 0)
		fVelClamp = Get_Component<CRigidBody>()->Get_Velocity().y / 50.f;
	if (fVelClamp < 0.2f)
		fVelClamp = 0.2f;

	
	string ColSound = "Collision" + to_string(i);
	CSoundMgr::Get_Instance()->Set_Volume(ColSound, fVelClamp);


	CSoundMgr::Get_Instance()->Play(ColSound, "SFX", false);

}

void CCube::PlayDragSound()
{
	const float EPSILON = 0.001f;
	_vec3 pos = Get_Component<CTransform>()->Get_Pos();
	_vec3 Gap = (pos - m_vPrePos);
	float dist = D3DXVec3Length(&Gap);
	_bool onground = Get_Component<CRigidBody>()->Get_OnGround();
	if (!m_bCurGrab && dist>EPSILON && onground && m_bMoveEdge)
	{
		CSoundMgr::Get_Instance()->Set_Volume("CubeDrag", 0.3f);
		CSoundMgr::Get_Instance()->Play("CubeDrag", "SFX", false);
		m_bMoveEdge = false;
	}
	else
	{
		CSoundMgr::Get_Instance()->Stop("CubeDrag");
		m_bMoveEdge = true;
	}
	m_vPrePos = pos;
	m_bPreGround = onground;

}