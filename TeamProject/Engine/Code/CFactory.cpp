#pragma once
#include "Engine_Define.h"

#include "CFactory.h"
#include "CGameObject.h"
#include "CTransform.h"
#include "CCamera.h"
#include "CModel.h"

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

#ifdef _DEBUG
        //std::cout << "[Prefab 저장 완료] " << path << " (" << written << " bytes)\n";
#endif
    }
    else
    {
        MessageBoxW(nullptr, L"파일 저장 실패", L"Error", MB_OK);
    }
}


void CFactory::DeSerializeScene(const wstring& SceneData)
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

    outJson["eye"] = { eye.x, eye.y, eye.z };
    outJson["fov"] = fov;
    outJson["aspect"] = aspect;
    outJson["znear"] = zNear;
    outJson["zfar"] = zFar;
}

void CFactory::Serialize_Model(nlohmann::json& outJson, CModel* comp)
{
}


unordered_map<wstring, CreatorFunc>& CFactory::Get_Map()
{
    static unordered_map<wstring, CreatorFunc> umap;
    return umap;
}