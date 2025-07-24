#include "pch.h"
#include "CObjCapture.h"
#include "CUiQuad.h"
#include "CTransform.h"
#include "CModel.h"
#include "CMaterial.h"
#include "CCameraMgr.h"
#include "CLightMgr.h"
#include "CCamera.h"
#include "CShaderMgr.h"

CObjCapture::CObjCapture(LPDIRECT3DDEVICE9 pGraphicDev)
:  CGameObject(pGraphicDev)
{
}

CObjCapture::~CObjCapture()
{
}

CObjCapture* CObjCapture::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CObjCapture* instance = new CObjCapture(pGraphicDev);

	if (FAILED(instance->Ready_GameObject())) {
		Safe_Release(instance);
		instance = nullptr;
	}

	return instance;
}

HRESULT CObjCapture::Ready_GameObject()
{
	m_pQuad = Add_Component<CUiQuad>(ID_DYNAMIC, m_pGraphicDev);
	m_pTransform = Add_Component<CTransform>(ID_DYNAMIC, m_pGraphicDev);
	vRatio = { WINCX,WINCY };
	vRatio /= 8.f;
	m_tPanel.Set_Size(vRatio);

	m_pQuad->Set_Texture(L"UI/TXUI_Storage.png");
	m_pQuad->Set_Shader(L"g_UIShader.fx");
	//m_pQuad->Set_Alpha(0.4f);

	m_pGraphicDev->CreateTexture(
		vRatio.x, vRatio.y+30,
		1, D3DUSAGE_RENDERTARGET,
		D3DFMT_A8R8G8B8,
		D3DPOOL_DEFAULT,
		&m_pTargetTex,
		NULL
	);
	return S_OK;
}

_int CObjCapture::Update_GameObject(const _float& fTimeDelta)
{
	if (!m_bOpen) return 0;
	m_tPanel.Set_Size(vRatio);
	RenderTarget();
	CGameObject::Update_GameObject(fTimeDelta);
	m_pTransform->Set_Scale(m_tPanel.Get_WorldScale());
	m_pTransform->Set_Pos({ m_tPanel.Get_WorldPos(WINCX, WINCY) });
	return 0;
}

void CObjCapture::LateUpdate_GameObject(const _float& fTimeDelta)
{
	if (!m_bOpen) return ;
	CGameObject::LateUpdate_GameObject(fTimeDelta);
}

void CObjCapture::RenderTarget()
{
	if (!m_pRenderTarget) return;
	CModel* targetModel = m_pRenderTarget->Get_Component<CModel>();

	LPDIRECT3DSURFACE9 pBackBuffer = nullptr;
	m_pGraphicDev->GetRenderTarget(0, &pBackBuffer);// 백업
	
	LPDIRECT3DSURFACE9 pDestSurface = nullptr;
	m_pTargetTex->GetSurfaceLevel(0, &pDestSurface);

	m_pGraphicDev->SetRenderTarget(0, pDestSurface);
	m_pGraphicDev->Clear(0, nullptr, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_ARGB(55, 255, 255, 255), 1.0f, 0);

	if (targetModel)
		RenderModel(targetModel);

	m_pGraphicDev->SetRenderTarget(0, pBackBuffer);
	pDestSurface->Release();
	pBackBuffer->Release();

	m_pQuad->Set_TextureHandle(m_pTargetTex);
}

void CObjCapture::Set_Pos(_vec2 pos)
{
	m_tPanel.Set_Anchor(UIPanel::Anchor::LeftTop, pos);
}

void CObjCapture::Free()
{
	CGameObject::Free();
}


void CObjCapture::RenderModel(CModel* model)
{
	if (!model->Get_Mesh() || !model->Get_Material())
	{
		return;
	}

	CTransform* pTransform = model->m_pOwner->Get_Component<CTransform>();
	if (pTransform == nullptr)
	{
		return;
	}

	m_pGraphicDev->SetTransform(D3DTS_WORLD, pTransform->Get_WorldMatrix());
	//LPD3DXEFFECT shader = CShaderMgr::Get_Instance()->GetShader(L"g_Target.fx");
	LPD3DXEFFECT shader = model->Get_Material()->Get_Effect();

	UINT passCount = 0;
	model->Get_Material()->Apply(m_pGraphicDev); // 내부에서 텍스처를 Bind
	//m_pGraphicDev->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_POINT);
	//m_pGraphicDev->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_POINT);
	//m_pGraphicDev->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_POINT);

	if (shader)
	{
		const D3DXVECTOR3& scale = pTransform->Get_Scale();
		CGameObject* Cam = CCameraMgr::Get_Instance()->Get_MainCamera();
		CCamera* CamCam = Cam->Get_Component<CCamera>();
		//CTransform* CamTrans = Cam->Get_Component<CTransform>();

		D3DXMATRIX TargetWorld = *(model->m_pOwner->Get_Component<CTransform>()->Get_WorldMatrix());
		D3DXMatrixIdentity(&TargetWorld);
		_vec3 CamPos = CamCam->Get_Eye();
		_vec3 MakeWorld = CamCam->Get_Look();
		D3DXVec3Normalize(&MakeWorld, &MakeWorld);

		MakeWorld *=5;
		MakeWorld += CamPos;
		D3DXMATRIX scaleMat;
		//D3DXMatrixScaling(&scaleMat, 0.8f, 0.8f, 0.8f);
		TargetWorld =  TargetWorld;

		TargetWorld._41 = MakeWorld.x;
		TargetWorld._42 = MakeWorld.y;
		TargetWorld._43 = MakeWorld.z;

		D3DXMATRIX view = *(CCameraMgr::Get_Instance()->Get_MainViewMatrix());
		D3DXMATRIX proj = *(CCameraMgr::Get_Instance()->Get_MainProjectionMatrix());
	
		shader->SetMatrix("g_matWorld", &TargetWorld);
		shader->SetMatrix("g_matView", &view);
		shader->SetMatrix("g_matProj", &proj);
		//shader->SetBool("g_usingLight", true);

		D3DLIGHT9 pLight = CLightMgr::Get_Instance()->Get_MainLight();
		_vec3 vLightDir = pLight.Direction;
		vLightDir *= -1.f;

		shader->SetVector("g_LightDir", reinterpret_cast<D3DXVECTOR4*>(&vLightDir));
		shader->SetVector("g_LightColor", reinterpret_cast<D3DXVECTOR4*>(&pLight.Diffuse));
		shader->SetVector("g_Ambient", reinterpret_cast<D3DXVECTOR4*>(&pLight.Ambient));
		shader->SetTechnique("Opaque");
		shader->SetFloat("g_Alpha", 1.f);

		_vec4 tmp = { 1.f,1.f,0.f,0.f };
		D3DXVECTOR4 transScale(scale.x, scale.y, 0.f, 0.f); // Z, W는 임시값
		shader->SetVector("g_UVScale", &transScale);
	}

	if (shader) {
		shader->Begin(&passCount, 0);
		for (UINT i = 0; i < passCount; ++i) {
			shader->BeginPass(i);
			model->Get_Mesh()->Render_Buffer();
			shader->EndPass();
		}
		shader->End();
	}
	else {
		model->Get_Mesh()->Render_Buffer();
	}
}