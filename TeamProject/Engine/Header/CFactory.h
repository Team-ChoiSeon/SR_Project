#pragma once
BEGIN(Engine)

class CGameObject;
class CTransform;
class CCamera;
class CModel;

using CreatorFunc = std::function<CGameObject* (LPDIRECT3DDEVICE9)>;

class ENGINE_DLL CFactory
{
public:
    static void Register(const wstring& className, CreatorFunc func);
    static CGameObject* Create(const wstring& className, LPDIRECT3DDEVICE9 pGraphicDev);
    
    static void Save_Prefab(CGameObject* object, const string& className);
    static void DeSerializeScene(const wstring& SceneData);
    static CGameObject* DeSerializeObject(const nlohmann::json& inJson);

private:
    static void Serialize_Transform(nlohmann::json& outJson, CTransform* comp);
    static void Serialize_Camera(nlohmann::json& outJson,CCamera* comp);
    static void Serialize_Model(nlohmann::json& outJson, CModel* comp);

private:
    static unordered_map<wstring, CreatorFunc>& Get_Map();

};

END
