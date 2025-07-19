#pragma once
#include "CComponent.h"

BEGIN(Engine)

class ENGINE_DLL CUI : public CComponent
{
protected:
	explicit CUI();
	explicit CUI(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CUI(const CUI& rhs);

public:
	virtual ~CUI();

public:
	//HRESULT			Ready_UI();
	/*virtual void	Update_Component(const _float& fTimeDelta) = 0;
	virtual void	LateUpdate_Component() = 0;*/

	virtual HRESULT Ready_Component()override;
	virtual void	Update_Component(const _float& fTimeDelta) override = 0;
	virtual void	LateUpdate_Component(const _float& fTimeDelta)override = 0;
	virtual void	Render(LPDIRECT3DDEVICE9 pDevice) = 0;

	void Set_Position(const D3DXVECTOR2& pos) { m_vPosition = pos; }
	void Set_Scale(const D3DXVECTOR2& scale) { m_vScale = scale; }
	void Set_Visible(_bool bVisible) { m_bVisible = bVisible; }
	void Set_OrthoMatrix(const _matrix& matOrtho) { m_matOrtho = matOrtho; }

	const D3DXVECTOR2& Get_Position() const { return m_vPosition; }
	const D3DXVECTOR2& Get_Scale() const { return m_vScale; }
	const _bool& IsVisible() const { return m_bVisible; }

private:
	virtual void Free()override;

protected:	
	LPD3DXSPRITE	m_pSprite = nullptr;
	D3DXVECTOR2		m_vPosition{ 0.f,0.f };
	D3DXVECTOR2		m_vScale{1.f,1.f};
	_bool			m_bVisible = true;
	_matrix			m_matOrtho{};

};

END
