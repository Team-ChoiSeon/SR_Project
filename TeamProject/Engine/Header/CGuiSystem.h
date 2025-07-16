#pragma once
#include "CBase.h"
BEGIN(Engine)

struct PANEL
{
    bool bOpen = true;
    std::function<void()> callback;

    PANEL() = default;
    PANEL(bool open, std::function<void()> cb)
        : bOpen(open), callback(std::move(cb)) {
    }
};

class ENGINE_DLL CGuiSystem :
    public CBase
{
    DECLARE_SINGLETON(CGuiSystem)

private:
    explicit CGuiSystem();
    virtual ~CGuiSystem();

public:
    HRESULT Ready_GUI(HWND hwnd);
    void Render_GUI();

    ImFont* Get_Font(const string& tag);
    bool Set_Input(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
    void RegisterPanel(const string& name, function<void()> callback);
    void RemovePanel(const string& name);

private:
    LPDIRECT3DDEVICE9 m_pDevice;
    unordered_map<string, PANEL> m_PanelContainer;
    unordered_map<string, ImFont*> m_fontContainer;
private:
    void Free();

};

END