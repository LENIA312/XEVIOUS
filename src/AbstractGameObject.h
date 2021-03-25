#pragma once
#pragma warning(disable:26812)

// ゲームオブジェクトの種類
enum eTag
{
	Tag_Player,
	Tag_Zapper,
	Tag_Blaster,
	Tag_Enemy_Sky,
	Tag_Enemy_Ground,
	Tag_Enemy_Bullet,
	Tag_Item,

	Tag_None,
};

class AbstractGameObject
{
public:
	AbstractGameObject();
	~AbstractGameObject();
	virtual void update() = 0;
	virtual void draw() const = 0;
	virtual void drawCollision(int r,int g, int b) const;
	virtual void collisionGameObject(eTag aTag);
	virtual int getCollisionPoint(int aPos) const;
	virtual eTag getTag() const = 0;
	int getXY(int aXY);
	bool checkActiveFlag();
	void setCameraY(float aY);

protected:
	float mX;
	float mY;
	float mCameraY;
	float mAnime;
	int mLayer;
	bool mActiveFlag;

};