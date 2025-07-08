#pragma once
BEGIN(Engine)

class CGameObject;
class CTransform;
class CCamera;
class CLight;
class CModel;
class CPickTarget;
class CCollider;
class CRigidBody;
class CScene;

using CreatorFunc = std::function<CGameObject* (LPDIRECT3DDEVICE9)>;

class ENGINE_DLL CFactory
{
public:
    static void Register(const wstring& className, CreatorFunc func);
    static CGameObject* Create(const wstring& className, LPDIRECT3DDEVICE9 pGraphicDev);
    
    static void Save_Prefab(CGameObject* object, const string& className);
    static void DeSerializeScene(const wstring& SceneData, CScene* scene);
    static void DeSerializeLayer(LAYERID eID, const nlohmann::json& inJson);
    static CGameObject* DeSerializeObject(const nlohmann::json& inJson);

private:
    static void Serialize_Transform(nlohmann::json& outJson, CTransform* comp);
    static void Serialize_Camera(nlohmann::json& outJson,CCamera* comp);
    static void Serialize_Model(nlohmann::json& outJson, CModel* comp);
    static void Serialize_Light(nlohmann::json& outJson, CLight* comp);
    static void Serialize_PickTarget(nlohmann::json& outJson, CPickTarget* comp);
    static void Serialize_Collider(nlohmann::json& outJson, CCollider* comp);
    static void Serialize_RigidBody(nlohmann::json& outJson, CRigidBody* comp);

private:
    static string ToString(const wstring& wstr);
    static wstring ToWString(const string& str);
    static LAYERID ToLayer(const wstring& wstr);


    static unordered_map<wstring, CreatorFunc>& Get_Map();

};

END
