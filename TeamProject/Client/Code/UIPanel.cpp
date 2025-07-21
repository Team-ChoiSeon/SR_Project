#include "pch.h"
#include "Engine_Define.h"
#include "UIPanel.h"

UIPanel::UIPanel()
{
}

UIPanel::UIPanel(_vec2 pos, _vec2 size)
	:	vPos(pos), vSize(size)
{
}

UIPanel::~UIPanel()
{
}

void UIPanel::Set_Anchor(Anchor _anchor, _vec2 _pivot)
{
	switch (_anchor)
	{
	case Anchor::Left:
		vPos.x = _pivot.x + vSize.x * 0.5f;
		break;
	case Anchor::Right:
		vPos.x = _pivot.x - vSize.x * 0.5f;
		break;
	case Anchor::Top:
		vPos.y = _pivot.y+ vSize.y * 0.5f;
		break;
	case Anchor::Bottom:
		vPos.y = _pivot.y - vSize.y * 0.5f;
		break;
	case Anchor::Center:
		vPos.x = _pivot.x;
		vPos.y = _pivot.y;
		break;
	default:
		vPos.x = _pivot.x;
		vPos.y = _pivot.y;
		break;
	}
}
