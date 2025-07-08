#pragma once
#include "Engine_Define.h"

#include "CFactory.h"
#include "CGameObject.h"
#include "CTransform.h"
#include "CCamera.h"

#include "CModel.h"
#include "CMesh.h"
#include "CMaterial.h"
#include "CTexture.h"

#include "CRigidBody.h"
#include "CCollider.h"
#include "CPickTarget.h"
#include "CLight.h"
#include "CScene.h"

void CFactory::Register(const wstring& className, CreatorFunc func)
{
	Get_Map()[className] = func;
}

CGameObject* CFactory::Create(const wstring& className, LPDIRECT3DDEVICE9 pGraphicDev)
{
	auto it = Get_Map().find(className);
	if (it != Get_Map().end())
		return it->second(pGraphicDev); // 함수 호출
	return nullptr;
}
void CFactory::Save_Prefab(CGameObject* object, const string& className)
{
	if (!object)
		return;

	json jObj;
	jObj["class"] = className;

	json jComponents;

	if (CTransform* comp = object->Get_Component<CTransform>()) {
		json jTrans;
		Serialize_Transform(jTrans, comp);
		jComponents["CTransform"] = jTrans;
	}
	if (CCamera* comp = object->Get_Component<CCamera>()) {
		json jCam;
		Serialize_Camera(jCam, comp);
		jComponents["CCamera"] = jCam;
	}
	if (CModel* comp = object->Get_Component<CModel>()) {
		json jModel;
		Serialize_Model(jModel, comp);
		jComponents["CModel"] = jModel;
	}
	if (CLight* comp = object->Get_Component<CLight>()) {
		json jLight;
		Serialize_Light(jLight, comp);
		jComponents["CLight"] = jLight;
	}
	if (CPickTarget* comp = object->Get_Component<CPickTarget>()) {
		json jPick;
		Serialize_PickTarget(jPick, comp);
		jComponents["CPickable"] = jPick;
	}
	if (CCollider* comp = object->Get_Component<CCollider>()) {
		json jCol;
		Serialize_Collider(jCol, comp);
		jComponents["CCollider"] = jCol;
	}
	if (CRigidbody* comp = object->Get_Component<CRigidbody>()) {
		json jRigid;
		Serialize_RigidBody(jRigid, comp);
		jComponents["CRigidBody"] = jRigid;
	}

	jObj["components"] = jComponents;

	// 저장 경로 준비
	string dir = "../Bin/Resource/Data/";
	CreateDirectoryA(dir.c_str(), NULL);

	string path = dir + className + ".json";
	string jsonText = jObj.dump(4);

	HANDLE hFile = ::CreateFileA(
		path.c_str(),
		GENERIC_WRITE,
		0,
		NULL,
		CREATE_ALWAYS,
		FILE_ATTRIBUTE_NORMAL,
		NULL);

	if (hFile != INVALID_HANDLE_VALUE)
	{
		DWORD written = 0;
		::WriteFile(hFile, jsonText.c_str(), (DWORD)jsonText.length(), &written, NULL);
		::CloseHandle(hFile);
	}
	else
	{
		MessageBoxW(nullptr, L"파일 저장 실패", L"Error", MB_OK);
	}
}


void CFactory::DeSerializeScene(const wstring& SceneData, CScene* scene)
{
	HANDLE hFile = ::CreateFileW(
		SceneData.c_str(), GENERIC_READ, FILE_SHARE_READ, NULL,
		OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	if (hFile == INVALID_HANDLE_VALUE) {
		MessageBoxW(nullptr, L"파일 열기 실패", L"Error", MB_OK);
		return;
	}

	DWORD fileSize = GetFileSize(hFile, NULL);
	string jsonText(fileSize, '\0');
	DWORD bytesRead = 0;
	ReadFile(hFile, jsonText.data(), fileSize, &bytesRead, NULL);
	CloseHandle(hFile);

	json jScene = json::parse(jsonText);
	string sceneName = jScene.value("scene_name", "");

	if (!sceneName.empty()) {
		
	}
	else {
		MessageBoxW(nullptr, L"scene_name이 없습니다", L"Error", MB_OK);
	}
}

void CFactory::DeSerializeLayer(LAYERID eID, const nlohmann::json& inJson)
{

}

CGameObject* CFactory::DeSerializeObject(const nlohmann::json& inJson)
{
	return nullptr;
}

void CFactory::Serialize_Transform(nlohmann::json& outJson, CTransform* comp)
{
	if (!comp)
		return;

	_vec3 pos = comp->Get_Pos();
	_vec3 rot = comp->Get_Angle();
	_vec3 scale = comp->Get_Scale();

	outJson["position"] = { pos.x, pos.y, pos.z };
	outJson["rotation"] = { rot.x, rot.y, rot.z };
	outJson["scale"] = { scale.x, scale.y, scale.z };
}

void CFactory::Serialize_Camera(nlohmann::json& outJson, CCamera* comp)
{
	if (!comp)
		return;

	const _vec3& eye = comp->Get_Eye();
	float fov = comp->Get_Fov();
	float aspect = comp->Get_Aspect();
	float zNear = comp->Get_Near();
	float zFar = comp->Get_Far();

	outJson["fov"] = fov;
	outJson["znear"] = zNear;
	outJson["zfar"] = zFar;
}

void CFactory::Serialize_Model(nlohmann::json& outJson, CModel* comp)
{
	if (!comp) return;
	CMaterial* material = comp->Get_Material();
	CMesh* mesh = comp->Get_Mesh();

	// 상위 기본 정보
	outJson["mesh"] = ToString(mesh->Get_Key());
	outJson["matKey"] = ToString(material->Get_MatrialKey());

	// 머티리얼 내부 텍스처 키
	if (material)
	{
		json jMat;

		if (CTexture* tex = material->Get_Diffuse()) {
			jMat["diffuse"] = ToString(tex->Get_Key());
		}

		if (CTexture* tex = material->Get_Normal()) {
			jMat["normal"] = ToString(tex->Get_Key());
		}
		if (CTexture* tex = material->Get_Roughness()) {
			jMat["roughness"] = ToString(tex->Get_Key());
		}

		if (LPD3DXEFFECT effect = material->Get_Effect()) {
			jMat["shader"] = ToString(material->Get_ShaderKey());
		}

		outJson["material"] = jMat;
	}
}

void CFactory::Serialize_Light(nlohmann::json& outJson, CLight* comp)
{
	D3DLIGHT9 light = comp->Get_LightInfo();
	//D3DLIGHT_POINT = 1,
	//D3DLIGHT_SPOT = 2,
	//D3DLIGHT_DIRECTIONAL = 3,

	outJson["type"] = light.Type;
	outJson["position"] = { light.Position.x, light.Position.y, light.Position.z };
	outJson["direction"] = { light.Direction.x, light.Direction.y, light.Direction.z };

	outJson["range"] = light.Range;
	outJson["attenuation"] = { light.Attenuation0, light.Attenuation1, light.Attenuation2 };

	outJson["ambient"] = { light.Ambient.r, light.Ambient.g, light.Ambient.b, light.Ambient.a };
	outJson["diffuse"] = { light.Diffuse.r, light.Diffuse.g, light.Diffuse.b, light.Diffuse.a };
	outJson["specular"] = { light.Specular.r, light.Specular.g, light.Specular.b, light.Specular.a };

	outJson["theta"] = light.Theta;
	outJson["phi"] = light.Phi;
}

void CFactory::Serialize_PickTarget(nlohmann::json& outJson, CPickTarget* comp)
{
	if (!comp) return;

	outJson["validInGame"] = "true";
}

void CFactory::Serialize_Collider(nlohmann::json& outJson, CCollider* comp)
{
	outJson["ColType"] = comp->Get_ColType();

}
void CFactory::Serialize_RigidBody(nlohmann::json& outJson, CRigidbody* comp)
{
	if (!comp)
		return;

	outJson["m_bGravity"] = comp->Get_UseGravity();

	outJson["Mass"] = comp->Get_Mass();
	//outJson["Friction"] = comp->Get_Friction();
	//outJson["Bounciness"] = comp->Get_Bounciness();

	outJson["Velocity"] = { comp->Get_Velocity().x, comp->Get_Velocity().y, comp->Get_Velocity().z };
	outJson["Accelator"] = { comp->Get_Accel().x, comp->Get_Accel().y, comp->Get_Accel().z };

	outJson["AngleVelocity"] = { comp->Get_AVelocity().x, comp->Get_AVelocity().y, comp->Get_AVelocity().z };
	//outJson["AngleAccel"] = { comp->Get_AAccel().x, comp->Get_AAccel().y, comp->Get_AAccel().z };

	outJson["Gravity"] = { comp->Get_Gforce().x, comp->Get_Gforce().y, comp->Get_Gforce().z };
	outJson["ExternalForce"] = { comp->Get_Eforce().x, comp->Get_Eforce().y, comp->Get_Eforce().z };
	//outJson["Torque"] = { comp->Get_Torque().x, comp->Get_Torque().y, comp->Get_Torque().z };

	//outJson["Inertia"] = { comp->Get_Inertia().x, comp->Get_Inertia().y, comp->Get_Inertia().z };
}



unordered_map<wstring, CreatorFunc>& CFactory::Get_Map()
{
	static unordered_map<wstring, CreatorFunc> umap;
	return umap;
}


string CFactory::ToString(const wstring& wstr)
{
	if (wstr.empty()) return {};
	int size_needed = WideCharToMultiByte(CP_UTF8, 0, wstr.data(), (int)wstr.size(), nullptr, 0, nullptr, nullptr);
	std::string result(size_needed, 0);
	WideCharToMultiByte(CP_UTF8, 0, wstr.data(), (int)wstr.size(), &result[0], size_needed, nullptr, nullptr);
	return result;
}

wstring CFactory::ToWString(const string& str)
{
	if (str.empty()) return {};
	int size_needed = MultiByteToWideChar(CP_UTF8, 0, str.data(), (int)str.size(), nullptr, 0);
	std::wstring result(size_needed, 0);
	MultiByteToWideChar(CP_UTF8, 0, str.data(), (int)str.size(), &result[0], size_needed);
	return result;
}

LAYERID CFactory::ToLayer(const wstring& wstr)
{
	if (wstr == L"CAMERA")  return LAYERID::LAYER_CAMERA;
	if (wstr == L"PLAYER")  return LAYERID::LAYER_PLAYER;
	if (wstr == L"OBJECT")  return LAYERID::LAYER_OBJECT;
	if (wstr == L"TILE")    return LAYERID::LAYER_TILE;

	return LAYERID::LAYER_END; // 또는 예외 처리용 L_INVALID이 있으면 더 좋음
}