#pragma once

#include "pch.h"
#include "SimpleMath.h"
#include <flag.h>

// Stuff that will exist later :p
struct GameData;


using namespace DirectX;
using namespace SimpleMath;

class BaseObject
{
public:
	BaseObject();
	virtual ~BaseObject() {};

	Flag sleep_flag;

	
	
	BaseObject* getGameObject() { return this; }

	void setPos(Vector2 _pos) { m_pos = _pos; }
	void setXPos(float _xpos) { m_pos.x = _xpos; }
	void setYPos(float _ypos) { m_pos.y = _ypos; }
	void setRot(float _rot) { m_rotation = _rot; }

	void setVelX(float x) { vel.x = x; }
	void setVelY(float y) { vel.y = y; }
	void setVel(Vector2 _vel) { vel = _vel; }

	Vector2 getSize() const { return m_size * m_scale; }
	Vector2 getPos() const { return m_pos; }
	float getXPos() const { return m_pos.x; }
	float getYPos() const { return m_pos.y; }

	bool getSleep() { return sleep; }
	bool getActive() { return active; }

	void setSleep(bool new_state) { sleep = new_state; }
	void setActive(bool new_state) { active = new_state; }

	void toggleSleep() { sleep = !sleep; }
	void toggleActive() { active = !active; }


	Vector2 getPos() { return m_pos; }
	float getXPos() { return m_pos.x; }
	float getYPos() { return m_pos.y; }

	float getRot() { return m_rotation; }

	Color getColour() { return m_colour; }
	float getAlpha() { return m_colour.A(); }

	Vector2 getScale() { return m_scale; }
	float getXScale() { return m_scale.x; }
	float getYScale() { return m_scale.y; }


	// TODO Review usage
	Vector2 getVel() const { return vel; }
	float getRot() const { return m_rotation; }

	void setColourR(float _r) { m_colour.R(_r); }
	void setColourG(float _g) { m_colour.G(_g); }
	void setColourB(float _b) { m_colour.B(_b); }

	float getColourR() { return m_colour.R(); }
	float getColourG() { return m_colour.G(); }
	float getColourB() { return m_colour.B(); }


	Vector2 getOrigin() { return m_origin; }
	float getXOrigin() { return m_origin.x; }
	float getYOrigin() { return m_origin.y; }



protected:

	bool restrictLeft = false;
	bool restrictRight = false;

	bool jumping = false;
	bool gravity = false;

	// Sleep does the movement
	bool sleep = false;
	// Active does the rendering/tick
	bool active = true;

	float m_rotation;
	Color m_colour;
	Vector2 m_scale;
	Vector2 m_origin;
	Vector2 m_size;
	Vector2 m_pos;

	// Width & hight
	float m_width;
	float m_hight;

	float speed = 30.0f;
	Vector2 vel = { 0 , 0 };
	Vector2 grav = { 0 , 2 };

};