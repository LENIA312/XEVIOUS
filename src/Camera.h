#pragma once

#include "SceneList.h"

class Camera
{
public:
	Camera();
	~Camera();
	void update();
	void draw() const;
	float getY() const;
	void reset();
	void setMode(int aMode);

private:
	int mMode;
	float mY;

};