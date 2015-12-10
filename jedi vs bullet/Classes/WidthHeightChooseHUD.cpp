//
//  WidthHeightChooseHUD.cpp
//  GameRun
//
//  Created by Thien Banh on 11/14/15.
//
//

#include "WidthHeightChooseHUD.h"
#include "MainMenuScene.h"

WidthHeightChooseHUD::WidthHeightChooseHUD(Layer* parent,bool canExit, bool withBackground) : HUDLayer(parent,canExit){
	this->init(withBackground);
}

WidthHeightChooseHUD::~WidthHeightChooseHUD(){
    
}

// on "init" you need to initialize your instance
bool WidthHeightChooseHUD::init(bool withBackground)
{
    auto visibleSize = Director::getInstance()->getVisibleSize();
    auto origin = Director::getInstance()->getVisibleOrigin();
	if (!withBackground)
	{
		background = Sprite::create("bg.png");
		background->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
		background->setScale(visibleSize.width / background->getContentSize().width, visibleSize.height / background->getContentSize().height);
		this->addChild(background);
	}
    
    
	auto boxSprite = Sprite::create("Windown1.png");
	boxSprite->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
	boxSprite->setScale((DESIGN_SCREEN_WIDTH / 1.5) / boxSprite->getContentSize().width, (DESIGN_SCREEN_HEIGHT / 1.5) / boxSprite->getContentSize().height);
	this->addChild(boxSprite);

	mWidthEditBox = ui::EditBox::create(Size(DESIGN_SCREEN_WIDTH / 3, 60), ui::Scale9Sprite::create("blue_button03.png"));
    mWidthEditBox->setMaxLength(3);
	mWidthEditBox->setFontSize(25);
	mWidthEditBox->setAnchorPoint(Vec2(0.5,0));
	mWidthEditBox->setPosition(Vec2(DESIGN_SCREEN_WIDTH / 2, DESIGN_SCREEN_HEIGHT/3));
    mWidthEditBox->setInputMode(cocos2d::ui::EditBox::InputMode::NUMERIC );
	mWidthEditBox->setPlaceholderFontColor(Color3B::WHITE);
    mWidthEditBox->setPlaceHolder("Input number tile of width");


	mHeightEditBox = ui::EditBox::create(Size(DESIGN_SCREEN_WIDTH / 3, 60), ui::Scale9Sprite::create("blue_button03.png"));
    mHeightEditBox->setMaxLength(3);
	mHeightEditBox->setFontSize(25);
	mHeightEditBox->setAnchorPoint(Vec2(0.5, 0));
	mHeightEditBox->setPosition(Vec2(DESIGN_SCREEN_WIDTH / 2, DESIGN_SCREEN_HEIGHT/2));
    mHeightEditBox->setInputMode(cocos2d::ui::EditBox::InputMode::NUMERIC );
    mHeightEditBox->setPlaceHolder("Input number tile of height");
	mHeightEditBox->setPlaceholderFontColor(Color3B::WHITE);
	mHeightEditBox->setReturnType(ui::EditBox::KeyboardReturnType::DONE);

    this->addChild(mHeightEditBox);
    this->addChild(mWidthEditBox);
    
    cocos2d::Vector<MenuItem*> items;
    
    auto mBackButton = MenuItemImage::create("ExitNormal.png", "ExitSelected.png", CC_CALLBACK_0(WidthHeightChooseHUD::menuBackCallback, this));
    mBackButton->setAnchorPoint(Vec2(0.5, 0));
    mBackButton->setScale(70 / mBackButton->getContentSize().width);
    mBackButton->setPosition(Vec2(DESIGN_SCREEN_WIDTH/3, 0));
    items.pushBack(mBackButton);
    
	auto mNextButton = MenuItemImage::create("AddNormal.png", "AddSelected.png", CC_CALLBACK_0(WidthHeightChooseHUD::menuNextCallback, this));
	mNextButton->setAnchorPoint(Vec2(0.5, 0));
	mNextButton->setScale(70 / mNextButton->getContentSize().width);
	mNextButton->setPosition(Vec2((DESIGN_SCREEN_WIDTH*2)/3, 0 ));
	items.pushBack(mNextButton);

    auto menu = Menu::createWithArray(items);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);
    
    return true;
}
void WidthHeightChooseHUD::menuNextCallback()
{
	

	
		mDelegate->exitBack(0, 0);
		exit();
	
}
void WidthHeightChooseHUD::menuBackCallback()
{   
	auto mainScene = MainMenuScene::createScene();
    Director::getInstance()->replaceScene(mainScene);
}