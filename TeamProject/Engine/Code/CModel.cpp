#include "CModel.h"
#include "CRenderMgr.h"
#include "CTransform.h"
#include "CCameraMgr.h"
#include "CLightMgr.h"

CModel::CModel(LPDIRECT3DDEVICE9 pDevice)
	: CComponent(pDevice), m_uvScale{1.f,1.f,0.f,0.f}
{
	if (m_pGraphicDev)
		m_pGraphicDev->AddRef();
}

CModel::~CModel()
{
}

CModel* CModel::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CModel* instance = new CModel(pDevice);

	if (FAILED(instance->Ready_Component())) {
		Safe_Release(instance);
		instance = nullptr;
	}

	DefaultCubeModel model;
	instance->Set_Model(model.meshKey, model.materialKey);
	return instance;
}

void CModel::LateUpdate_Component(const _float& fTimeDelta)
{
	if (m_fAlpha < 1.f) {
		m_ePass = RENDER_PASS::RP_TRANSPARENT;
	}

	CRenderMgr::Get_Instance()->Add_Model(this);
}

void CModel::Render(LPDIRECT3DDEVICE9 m_pDevice)
{
	if (!m_pMesh)
	{
		//OutputDebugString("[CModel] m_pMesh is nullptr\n");
		return;
	}
	if (!m_pMaterial)
	{
		OutputDebugString("[CModel] m_pMaterial is nullptr\n");
		return;
	}

	CTransform* pTransform = m_pOwner->Get_Component<CTransform>();
	if (pTransform == nullptr)
	{
		MSG_BOX("CModel::Render : pTransform is nullptr");
		return;
	}

	m_pDevice->SetTransform(D3DTS_WORLD, pTransform->Get_WorldMatrix());

	if (!m_pMaterial)
		return;

	LPD3DXEFFECT shader = m_pMaterial->Get_Effect();

	UINT passCount = 0;
	m_pMaterial->Apply(m_pDevice); // 내부에서 텍스처를 Bind

	if (shader)
	{
		const D3DXVECTOR3& scale = pTransform->Get_Scale();

		D3DXMATRIX world = *(pTransform->Get_WorldMatrix());
		D3DXMATRIX view = *(CCameraMgr::Get_Instance()->Get_MainViewMatrix());
		D3DXMATRIX proj = *(CCameraMgr::Get_Instance()->Get_MainProjectionMatrix());

		shader->SetMatrix("g_matWorld", &world);
		shader->SetMatrix("g_matView", &view);
		shader->SetMatrix("g_matProj", &proj);

		D3DLIGHT9 pLight = CLightMgr::Get_Instance()->Get_MainLight();
		_vec3 vLightDir = pLight.Direction;
		vLightDir *= -1.f;

		shader->SetVector("g_LightDir", reinterpret_cast<D3DXVECTOR4*>(&vLightDir));
		shader->SetVector("g_LightColor", reinterpret_cast<D3DXVECTOR4*>(&pLight.Diffuse));
		shader->SetVector("g_Ambient", reinterpret_cast<D3DXVECTOR4*>(&pLight.Ambient));
		shader->SetFloat("g_Alpha", m_fAlpha);

		_vec4 tmp = { 1.f,1.f,0.f,0.f };
		if (m_uvScale == tmp){
			D3DXVECTOR4 transScale(scale.x, scale.y, 0.f, 0.f); // Z, W는 임시값
			shader->SetVector("g_UVScale", &transScale);
			//shader->SetVector("g_UVPosition", &uvPos);
		}
		else {
			shader->SetVector("g_UVScale", &m_uvScale);
		}
	}

	if (shader) {
		shader->Begin(&passCount, 0);
		for (UINT i = 0; i < passCount; ++i) {
			shader->BeginPass(i);
			m_pMesh->Render_Buffer();
			shader->EndPass();
		}
		shader->End();
	}
	else {
		m_pMesh->Render_Buffer();
	}
}

HRESULT CModel::Set_Model(const wstring& meshType, const wstring& matType)
{

	if (!meshType.empty()) {
		Safe_Change(m_pMesh, CResourceMgr::Get_Instance()->Load_Mesh(m_pGraphicDev,meshType));
	}

	if (!matType.empty()) {
		Safe_Change(m_pMaterial, CResourceMgr::Get_Instance()->Load_Material(matType));
	}

	return S_OK;
}

void CModel::Free()
{
	Safe_Release(m_pGraphicDev);
	Safe_Release(m_pMesh);
	Safe_Release(m_pMaterial);
}
