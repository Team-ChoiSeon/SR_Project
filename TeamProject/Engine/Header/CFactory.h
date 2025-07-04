#pragma once
#include "Engine_Define.h"

BEGIN(Engine)

class CGameObject;

using CreatorFunc = std::function<CGameObject* (LPDIRECT3DDEVICE9)>;

class ENGINE_DLL CFactory
{
public:
    static void Register(const wstring& className, CreatorFunc func);
    static CGameObject* Create(const wstring& className, LPDIRECT3DDEVICE9 pGraphicDev);

private:
    static unordered_map<wstring, CreatorFunc>& Get_Map();
};

END