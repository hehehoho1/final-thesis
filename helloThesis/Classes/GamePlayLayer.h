#ifndef _GAME_PLAY_LAYER_H__
#define _GAME_PLAY_LAYER_H__

#include "cocos2d.h"
#include "b2Layer.h"
#include "Runner.h"
#include "GameConfig.h"
USING_NS_CC;

class GamePlayLayer : public b2Layer
{
private: 

public:

	virtual bool init();
	void update(float delta);
	void initTiles();
	void createTiles(float xloc, float yLoc);
	void createSlope(float xLoc, bool direction);

	//touches event
	void onTouchesBegan(const std::vector<Touch*>& touches, Event  *event);
	void onTouchesMoved(const std::vector<Touch*>& touches, Event  *event);
	void onTouchesEnded(const std::vector<Touch*>& touches, Event  *event);
	void onTouchesCancelled(const std::vector<Touch*>& touches, cocos2d::Event  *event);

	//Physics event
	void BeginContact(b2Contact* contact);
	void EndContact(b2Contact* contact);
	void PreSolve(b2Contact* contact, const b2Manifold* oldManifold);
	void PostSolve(b2Contact* contact, const b2ContactImpulse* impulse);

	CREATE_FUNC(GamePlayLayer);
	CC_SYNTHESIZE(Runner*, mRunner, Runner);
};

#endif 
