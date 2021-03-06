#ifndef _REAL_GAME_PLAY_LAYER_H__
#define _REAL_GAME_PLAY_LAYER_H__

#include "cocos2d.h"
#include "Anakin.h"
#include "GameConfig.h"
#include "BulletPool.h"
#include "SoundManager.h"
USING_NS_CC;

class RealGamePlayLayer : public LayerColor
{
private:
	Camera* camera;
	Follow* cameraFollow;
	Vec2 centralPoint;

public:
	virtual bool init();
	void update(float delta);

	//touches event
	bool onTouchBegan(Touch *touch, Event *event);
	void onTouchMoved(Touch *touch, Event *event);
	void onTouchEnded(Touch *touch, Event *event);
	void onTouchCancelled(Touch *touch, Event *event);

	void updateBulletPool();
	BulletPool* bulletPool;
	float scale;

	Anakin* mAnakin;

	void destroyBullet(Bullet* bullet, int i);
	CREATE_FUNC(RealGamePlayLayer);

	void reInit();
};

#endif 
