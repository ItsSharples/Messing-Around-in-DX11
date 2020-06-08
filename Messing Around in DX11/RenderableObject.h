#pragma once
#include "BaseObject.h"

struct DrawData2D;

class RenderableObject :
	public BaseObject
{

	virtual void Draw(DrawData2D* _DD) = 0;

	void setColour(Color _colour) { m_colour = _colour; }

	void setAlpha(float _a) { m_colour.A(_a); }

	void scale(Vector2 _scale) { m_scale *= _scale; }
	void setScale(Vector2 _scale) { m_scale = _scale; }
	void setScale(float _scale) { m_scale = _scale * Vector2::One; }
	void setOrigin(Vector2 _origin) { m_origin = _origin; }
};

