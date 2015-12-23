#include "HUDLayer.h"

HUDLayer::HUDLayer(Layer* parent,bool canExit){
	this->init(parent,canExit);
	this->autorelease();
}

HUDLayer::~HUDLayer(){
}

// on "init" you need to initialize your instance
bool HUDLayer::init(Layer* parent,bool canExitTouchOnSite)
{
	if (!Layer::init())
	{
		return false;
	}
    mCanExitTouchOnSite = canExitTouchOnSite;
	auto bg = cocos2d::LayerColor::create(Color4B(53, 53, 53, 100));
	this->addChild(bg);

	for (auto node : parent->getChildren()){
		if (node->getTag() == TAG_NORMAL_LAYER){
			Director::getInstance()->getEventDispatcher()->pauseEventListenersForTarget(node);
			node->pause();
		}
	}
	parent->pause();
	this->parent = parent;

	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = CC_CALLBACK_2(HUDLayer::onTouchBegan, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);


	this->setTag(TAG_HUD_LAYER);
	return true;
}

void HUDLayer::exit(){
	parent->resume();
	for (CCNode* node : parent->getChildren()){
		if (node->getTag() == TAG_NORMAL_LAYER){
			node->resume();
		}
        
	}
	parent->removeChild(this);
}

bool HUDLayer::onTouchBegan(Touch *touch, Event *event){
    if (mCanExitTouchOnSite) {
        if (!isTouchInside(touch)){
            //CCLog("dkm");
            exit();
        }
    }
	return true;
}

bool HUDLayer::isTouchInside(Touch *touch){
	//auto z = this->background->getTexture();
	//if (!true)
	//	return false;
	float x = touch->getLocation().x;
	float y = touch->getLocation().y;

	bool bottomCheck = (y >= this->background->getPosition().y - this->background->getContentSize().height / 2);
	bool topCheck = (y <= this->background->getPosition().y + this->background->getContentSize().height / 2);
	bool leftCheck = (x >= this->background->getPosition().x - this->background->getContentSize().width / 2);
	bool rightCheck = (x <= this->background->getPosition().x + this->background->getContentSize().width / 2);

	return bottomCheck && topCheck && leftCheck && rightCheck;
}
