#ifndef _HUD_LAYER_H__
#define _HUD_LAYER_H__
#include "cocos2d.h"
#include "GameConfig.h"

USING_NS_CC;

class HUDLayer : public Layer
{
protected: 
	Layer* parent;
	Sprite* background;
	virtual bool init(Layer* parent,bool canExitTouchOnSite);
	void exit();
    
public:
	HUDLayer(Layer* parent,bool canExit);
	~HUDLayer();
    
    
    
	//touches event

	//void onTouchesBegan(const std::vector<Touch*>& touches, Event  *event);
	//void onTouchesMoved(const std::vector<Touch*>& touches, Event  *event);
	//void onTouchesEnded(const std::vector<Touch*>& touches, Event  *event);
	//void onTouchesCancelled(const std::vector<Touch*>& touches, cocos2d::Event  *event);


	//CC_SYNTHESIZE(Runner*, mRunner, Runner);
};


#endif 
