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
#include "CParticle.h"

#include "CScene.h"
#include "CLayer.h"
#include "CGraphicDev.h"

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
	if (CRigidBody* comp = object->Get_Component<CRigidBody>()) {
		json jRigid;
		Serialize_RigidBody(jRigid, comp);
		jComponents["CRigidBody"] = jRigid;
	}
	if (CParticle* comp = object->Get_Component<CParticle>()) {
		json jParticle;
		Serialize_Particle(jParticle, comp);
		jComponents["CParticle"] = jParticle;
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
	json jLayers = jScene["layers"];

	for (auto& [layerName, jLayer] : jLayers.items()) {
		LAYERID eID = stringToLayer(layerName); // 레이어 이름 → enum
		CLayer* layer = scene->Get_Layer(eID); // ← string → enum 변환 필요시 처리
		if (!layer) continue;

		json jObjects = jLayer["objects"];
		DeSerializeLayer(layer, jObjects); // ← 이제 jObjects는 배열!
	}
}

void CFactory::DeSerializeLayer(CLayer* pLayer, const json& inJson)
{

	if (!pLayer || !inJson.is_array())
		return;
	//const auto& jObjects = inJson["objects"];

	for (const auto& jObj : inJson)
	{
		if (!jObj.is_object())
			continue;

		CGameObject* obj = DeSerializeObject(jObj);
		if (obj)
		{
			std::string nameStr = jObj.value("name", "");
			pLayer->Add_GameObject(ToWString(nameStr), obj);
		}
	}
}


CGameObject* CFactory::DeSerializeObject(const nlohmann::json& inJson)
{
	string classNameStr = inJson.value("class", "");
	string nameStr = inJson.value("name", "");

	wstring className = ToWString(classNameStr);
	wstring objectName = ToWString(nameStr);
	LPDIRECT3DDEVICE9 pDevice = CGraphicDev::Get_Instance()->Get_GraphicDev();
	// 0. 팩토리 생성
	CGameObject* obj = CFactory::Create(className, pDevice);
	if (!obj)
	{
		wstring err = L"CFactory:: obj is nullptr : " + className;
		MessageBoxW(0, err.c_str(), L"Error", MB_OK);
		return nullptr;
	}
	const auto& jComponents = inJson["components"];

	// 1. CTransform
	if (jComponents.contains("CTransform")) {
		if (auto comp = obj->Get_Component<CTransform>()) {
			const auto& jTrans = jComponents["CTransform"];
			comp->Set_Pos({ jTrans["position"][0], jTrans["position"][1], jTrans["position"][2] });
			if (jTrans.contains("rotation")) {
				const auto& r = jTrans["rotation"];

				_vec3 rotDeg = { r[0], r[1], r[2] };
				_vec3 axisX = { 1.f, 0.f, 0.f };
				_vec3 axisY = { 0.f, 1.f, 0.f };
				_vec3 axisZ = { 0.f, 0.f, 1.f };

				// XYZ 축 회전을 누적 회전 방식으로 적용
				comp->Rotate_Axis(axisX, D3DXToRadian(rotDeg.x));
				comp->Rotate_Axis(axisY, D3DXToRadian(rotDeg.y));
				comp->Rotate_Axis(axisZ, D3DXToRadian(rotDeg.z));
			}

			comp->Set_Scale({ jTrans["scale"][0], jTrans["scale"][1], jTrans["scale"][2] });
		}
	}

	// 2. CCamera
	if (jComponents.contains("CCamera")) {
		if (auto comp = obj->Get_Component<CCamera>()) {
			const auto& jTrans = jComponents["CCamera"];
			comp->Set_Fov(jTrans["fov"]);
			comp->Set_Near(jTrans["near"]);
			comp->Set_Far(jTrans["far"]);
		}
	}

	// 3. CModel
	if (jComponents.contains("CModel")) {
		CModel* model = obj->Get_Component<CModel>();
		const auto& jModel = jComponents["CModel"];
		if (model) {
			wstring meshKey = L"Default_A.obj";
			wstring matKey = L"Default_A.mtl";
			wstring shaderPath = L"g_UVScale.fx";

			if (jComponents["CModel"].contains("mesh"))
				meshKey = ToWString(jComponents["CModel"]["mesh"]);

			if (jComponents["CModel"].contains("matKey"))
				matKey = ToWString(jComponents["CModel"]["matKey"]);

			if (jComponents["CModel"].contains("shader"))
				shaderPath = ToWString(jComponents["CModel"]["shader"]);

			model->Set_Model(meshKey, matKey);

			if (!shaderPath.empty() && model->Get_Material())
				model->Get_Material()->Set_Shader(shaderPath);

			if (jComponents["CModel"].contains("uvScale") &&
				jComponents["CModel"]["uvScale"].is_array())
			{
				const auto& uvArr = jComponents["CModel"]["uvScale"];
				_vec4 uvScale = { 1.f, 1.f, 0.f, 0.f }; // 기본값

				if (uvArr.size() > 0) uvScale.x = uvArr[0].get<float>();
				if (uvArr.size() > 1) uvScale.y = uvArr[1].get<float>();
				if (uvArr.size() > 2) uvScale.z = uvArr[2].get<float>();
				if (uvArr.size() > 3) uvScale.w = uvArr[3].get<float>();

				model->Set_UVScale(uvScale);
			}

			if (jComponents["CModel"].contains("Alpha") )
			{
				model->Set_Alpha(jModel["Alpha"]);
			}
		}
	}

	// 4. CLight
	if (jComponents.contains("CLight")) {
		CLight* light = obj->Get_Component<CLight>();
		if (light) {
			const auto& jLight = jComponents["CLight"];
			D3DLIGHT9 d3dLight{};
			d3dLight.Type = static_cast<D3DLIGHTTYPE>(jLight["type"]);
			d3dLight.Position = { jLight["position"][0], jLight["position"][1], jLight["position"][2] };
			d3dLight.Direction = { jLight["direction"][0], jLight["direction"][1], jLight["direction"][2] };
			d3dLight.Range = jLight["range"];
			d3dLight.Attenuation0 = jLight["attenuation"][0];
			d3dLight.Attenuation1 = jLight["attenuation"][1];
			d3dLight.Attenuation2 = jLight["attenuation"][2];
			d3dLight.Ambient = { jLight["ambient"][0], jLight["ambient"][1], jLight["ambient"][2], jLight["ambient"][3] };
			d3dLight.Diffuse = { jLight["diffuse"][0], jLight["diffuse"][1], jLight["diffuse"][2], jLight["diffuse"][3] };
			d3dLight.Specular = { jLight["specular"][0], jLight["specular"][1], jLight["specular"][2], jLight["specular"][3] };
			d3dLight.Theta = jLight["theta"];
			d3dLight.Phi = jLight["phi"];
			light->Set_LightInfo(d3dLight);
		}
	}

	// 5. CPickable
	if (jComponents.contains("CPickable")) {
		const auto& jPick = jComponents["CPickable"];
		bool valid = jPick.value("validInGame", true);
	}

	// 6. CCollider
	if (jComponents.contains("CCollider") && !jComponents["CCollider"].is_null()) {
		const auto& jCol = jComponents["CCollider"];
		if (auto col = obj->Get_Component<CCollider>()) {
			if (jCol.contains("ColType")) col->Set_ColType(jCol["ColType"]);
			if (jCol.contains("ColTag")) col->Set_ColTag(jCol["ColTag"]);
			if (jCol.contains("BoundType")) col->Set_BoundType(jCol["BoundType"]);
		}
	}


	// 7. CRigidBody
	if (jComponents.contains("CRigidBody")) {
		const auto& jRigid = jComponents["CRigidBody"];
		if (auto rigid = obj->Get_Component<CRigidBody>()) {

			if (jRigid.contains("m_bGravity") && jRigid["m_bGravity"].is_boolean()) {
				rigid->Set_UseGravity(jRigid["m_bGravity"].get<bool>());
			}
			else {
				// 기본값 처리
				rigid->Set_UseGravity(false);
			}

			rigid->Set_Mass(jRigid["Mass"]);
			rigid->Set_Friction(jRigid["Friction"]);
			rigid->Set_Bounce(jRigid["Bounciness"]);

			rigid->Set_Velocity({
				jRigid["Velocity"][0],
				jRigid["Velocity"][1],
				jRigid["Velocity"][2]
				});

			rigid->Set_Accel({
				jRigid["Accelator"][0],
				jRigid["Accelator"][1],
				jRigid["Accelator"][2]
				});

			rigid->Set_AVelocity({
				jRigid["AngleVelocity"][0],
				jRigid["AngleVelocity"][1],
				jRigid["AngleVelocity"][2]
				});

			rigid->Set_AAccel({
				jRigid["AngleAccel"][0],
				jRigid["AngleAccel"][1],
				jRigid["AngleAccel"][2]
				});

			rigid->Set_Force({
				jRigid["ExternalForce"][0],
				jRigid["ExternalForce"][1],
				jRigid["ExternalForce"][2]
				});

			rigid->Set_Torque({
				jRigid["Torque"][0],
				jRigid["Torque"][1],
				jRigid["Torque"][2]
				});

			rigid->Set_Inertia({
				jRigid["Inertia"][0],
				jRigid["Inertia"][1],
				jRigid["Inertia"][2]
				});
		}
	}

	//08.Particle
	if (jComponents.contains("CParticle")) {
		const auto& jParticle = jComponents["CParticle"];
		if (auto particle = obj->Get_Component<CParticle>()) {
			particle->Set_SpawnInterval(jParticle.value("spawn_interval", 1.0f));
			particle->Set_MaxParticle(jParticle.value("max_particles", 100));
			particle->Set_LifeTime(jParticle.value("life_time", 1.0f));
			particle->Set_Size(jParticle.value("size", 1.0f));
			particle->Set_Type(jParticle.value("Particle_Type", PARTICLE_MOVE_TYPE::FIRE));

			auto off = jParticle.value("offset", vector<float>{0.f, 0.f, 0.f});
			if (off.size() == 3)
				particle->Set_Offset({ off[0], off[1], off[2] });

			auto col = jParticle.value("base_color", vector<int>{255, 255, 255, 255});
			if (col.size() == 4)
				particle->Set_Color(D3DCOLOR_ARGB(col[0], col[1], col[2], col[3]));

			if (jParticle.contains("texture_path")) {
				particle->Set_Texture(ToWString(jParticle["texture_path"]));
			}
		}
	}
	return obj;
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
	float fov = comp->Get_Fov();
	float zNear = comp->Get_Near();
	float zFar = comp->Get_Far();

	outJson["fov"] = fov;
	outJson["znear"] = zNear;
	outJson["zfar"] = zFar;
}
void CFactory::Serialize_Model(nlohmann::json& outJson, CModel* comp)
{
	if (!comp) {
		// nullptr이면 기본값 지정
		outJson["mesh"] = "Defualt_A.obj";
		outJson["matKey"] = "Defualt_A.mtl";
		outJson["shader"] = "g_UVScale.fx";
		return;
	}

	CMaterial* material = comp->Get_Material();
	CMesh* mesh = comp->Get_Mesh();

	// mesh 없으면 기본값
	if (mesh)
		outJson["mesh"] = ToString(mesh->Get_Key());
	else
		outJson["mesh"] = "Defualt_A.obj";

	// material 없으면 기본값
	if (material)
		outJson["matKey"] = ToString(material->Get_MatrialKey());
	else
		outJson["matKey"] = "Defualt_A.mtl";

	// shader 키도 CModel 아래로 직접 넣기 (역직렬화에 맞춤)
	if (material && material->Get_Effect()) {
		outJson["shader"] = ToString(material->Get_ShaderKey());
	}
	else {
		outJson["shader"] = "g_UVScale.fx"; // 기본값
	}
	outJson["Alpha"] = comp->Get_Alpha(); // float 값
	// 머티리얼 내부 텍스처 키들만 material 블록에
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

		outJson["material"] = jMat;
	}
}


void CFactory::Serialize_Light(nlohmann::json& outJson, CLight* comp)
{
	D3DLIGHT9 light = comp->Get_LightInfo();

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
	outJson["ColTag"] = comp->Get_ColTag();
}

void CFactory::Serialize_RigidBody(nlohmann::json& outJson, CRigidBody* comp)
{
	if (!comp)
		return;

	outJson["m_bGravity"] = comp->Get_UseGravity();

	outJson["Mass"] = comp->Get_Mass();
	outJson["Friction"] = comp->Get_Friction();
	outJson["Bounciness"] = comp->Get_Bounce();
	outJson["Gravity"] = { comp->Get_Gravity() };
	outJson["GravityForce"] = { comp->Get_Gforce().x, comp->Get_Gforce().y, comp->Get_Gforce().z };

	outJson["Velocity"] = { comp->Get_Velocity().x, comp->Get_Velocity().y, comp->Get_Velocity().z };
	outJson["Accelator"] = { comp->Get_Accel().x, comp->Get_Accel().y, comp->Get_Accel().z };

	outJson["AngleVelocity"] = { comp->Get_AVelocity().x, comp->Get_AVelocity().y, comp->Get_AVelocity().z };
	outJson["AngleAccel"] = { comp->Get_AAccel().x, comp->Get_AAccel().y, comp->Get_AAccel().z };

	outJson["ExternalForce"] = { comp->Get_Eforce().x, comp->Get_Eforce().y, comp->Get_Eforce().z };
	outJson["Torque"] = { comp->Get_Torque().x, comp->Get_Torque().y, comp->Get_Torque().z };
	outJson["Inertia"] = { comp->Get_Inertia().x, comp->Get_Inertia().y, comp->Get_Inertia().z };
}

void CFactory::Serialize_Particle(json& outJson, CParticle* comp)
{
	outJson["spawn_interval"] = comp->Get_Spawn();
	outJson["max_particles"] = comp->Get_MaxParticle();
	outJson["life_time"] = comp->Get_LifeTime();
	outJson["size"] = comp->Get_Size();

	_vec3 offset = comp->Get_Offset();
	outJson["offset"] = { offset.x, offset.y, offset.z };

	outJson["base_color"] =  //a,r,g,b
	{		((BYTE)((comp->Get_Color()) >> 24) & 0xFF),
			((BYTE)((comp->Get_Color()) >> 16) & 0xFF),
			((BYTE)((comp->Get_Color()) >> 8) & 0xFF),
			((BYTE)((comp->Get_Color()) >> 0) & 0xFF)
	};
	if (!comp->Get_Texture().empty()) {
		outJson["texture_path"] = ToString(comp->Get_Texture());
	}
	outJson["Particle_Type"] = comp->Get_Type();
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

LAYERID CFactory::stringToLayer(const string& wstr)
{
	if (wstr == "CAMERA")  return LAYERID::LAYER_CAMERA;
	if (wstr == "PLAYER")  return LAYERID::LAYER_PLAYER;
	if (wstr == "DEFAULT")  return LAYERID::LAYER_OBJECT;
	if (wstr == "OBJECT")  return LAYERID::LAYER_OBJECT;
	if (wstr == "TILE")    return LAYERID::LAYER_TILE;
	if (wstr == "UI")    return LAYERID::LAYER_UI;
	if (wstr == "LIGHT")    return LAYERID::LAYER_LIGHT;
};
