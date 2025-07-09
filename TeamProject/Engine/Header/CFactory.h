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
class CLayer;

using CreatorFunc = std::function<CGameObject* (LPDIRECT3DDEVICE9)>;

class ENGINE_DLL CFactory
{
public:
    static void Register(const wstring& className, CreatorFunc func);
    static CGameObject* Create(const wstring& className, LPDIRECT3DDEVICE9 pGraphicDev);
    
    static void Save_Prefab(CGameObject* object, const string& className);
    static void DeSerializeScene(const wstring& SceneData, CScene* scene);
    static void DeSerializeLayer(CLayer* pLayer, const json& inJson);

    static CGameObject* DeSerializeObject(const json& inJson);

private:
    static void Serialize_Transform(json& outJson, CTransform* comp);
    static void Serialize_Camera(json& outJson,CCamera* comp);
    static void Serialize_Model(json& outJson, CModel* comp);
    static void Serialize_Light(json& outJson, CLight* comp);
    static void Serialize_PickTarget(json& outJson, CPickTarget* comp);
    static void Serialize_Collider(json& outJson, CCollider* comp);
    static void Serialize_RigidBody(json& outJson, CRigidBody* comp);

private:
    static LAYERID stringToLayer(const string& wstr);
    static string ToString(const wstring& wstr);
    static wstring ToWString(const string& str);

    static unordered_map<wstring, CreatorFunc>& Get_Map();

};

END
