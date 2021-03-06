#include "RealGamePlayLayer.h"
#include "RankingScene.h"

// on "init" you need to initialize your instance
bool RealGamePlayLayer::init(std::string map, Layer *parentLayer, HttpShortMapInfo mapInfo)
{
	if (!b2Layer::init())
	{
		//		return false;
	}

	mMapInfo = mapInfo;
	time = 0;
	mCanRate = false;
	Size visibleSize = Director::getInstance()->getVisibleSize();
    this->mParentLayer = parentLayer;
	this->mMap = map;
	mRunner = Runner::create();
	mRunner->mModel->setb2Position(50, 300);
	mRunner->setPosition(50, 300);
	this->addChild(mRunner);
	mRunner->mModel->setVelocityX(15.0f);
	vector<int>* t = new vector<int>();
	t->push_back(0);
	t->push_back(0);
	t->at(0) = 0;
	t->at(1) = 0;

	quadTree = new QuadNode(map, t);
	currentObjectList = new set<string>();
	currentQuadNode = new vector<QuadNode*>();
	quadtreeUpdateCounter = 10;
	updateQuadTree();

	/*std::vector<std::string> part = Utility::splitString(map, "dm");

	std::vector<std::string> widthHeight = Utility::splitString(part.at(0), "\n");
	int nTilesWidth = std::stoi(widthHeight.at(0));
	int nTilesHeight = std::stoi(widthHeight.at(1));

	float x, y;
	float tileSize = 70 / GameConfig::scale;
	std::vector<std::string> objectData = Utility::splitString(part.at(1), "\n");
	for (int i = 0; i < nTilesHeight; i++){
	y = (int)(nTilesHeight - i - 1) * tileSize;
	int tmp = y;
	y = tmp;
	std::vector<std::string> currentLineData = Utility::splitString(objectData.at(i), ";");
	for (int j = 0; j < nTilesWidth; j++){
	std::string tileName = currentLineData.at(j);
	if (tileName == "0")
	continue;
	x = j * tileSize ;
	addTile(tileName, x, y);
	}
	}
	this->runAction(cocos2d::Follow::create(mRunner, Rect(0, 0, x / GameConfig::scale,
	y / GameConfig::scale)));
	*/

	auto listener = EventListenerTouchOneByOne::create();

	listener->onTouchBegan = CC_CALLBACK_2(RealGamePlayLayer::onTouchBegan, this);
	listener->onTouchMoved = CC_CALLBACK_2(RealGamePlayLayer::onTouchMoved, this);
	listener->onTouchEnded = CC_CALLBACK_2(RealGamePlayLayer::onTouchEnded, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	this->scheduleUpdate();


	this->runAction(cocos2d::Follow::create(mRunner, Rect(0 - 53, 0 - 18, quadTree->nodeRect.right / GameConfig::scale,
		quadTree->nodeRect.top / GameConfig::scale)));

	this->setTag(TAG_NORMAL_LAYER);
	return true;
}

void RealGamePlayLayer::update(float delta){
    
	if (mRunner->mModel->finish() && !mCanRate)
	{
		this->mDelegate->realUpdateTime(0);
		long upTime = time * 1000;
		mCanRate = true;
		//mRunner->mModel->setFinish(false);
		rateLayer = new RateHUDLayer(this->mParentLayer, time);
		rateLayer->setDelegate(this);
		//this->removeChild(menu);
		this->addChild(rateLayer);

		if (stol(mMapInfo.playTime) == 0 || stol(mMapInfo.playTime) > upTime)
		{
			std::vector<HttpRequestParameter> resData{ HttpRequestParameter(phoneKey, "test"), HttpRequestParameter("mid", mMapInfo.mid), HttpRequestParameter("playTime", StringUtils::format("%ld", upTime)) };
			HttpServices::inst->sendRequest(rateLayer, resData, HttpRequestMethod::UP_TIME);
			HttpServices::inst->setDelegate(this);
		}
	}
	else
	{
		time += delta;
		this->mDelegate->realUpdateTime(time);
		updateQuadTree();
		if (!mRunner->mView->isRollingBack()){

			b2Layer::update(delta);
		}
	}
}

void RealGamePlayLayer::exitBack()
{
    
}

void RealGamePlayLayer::ratePress(int Number)
{
	std::string info = mMapInfo.mid;
	std::vector<HttpRequestParameter> resData{ HttpRequestParameter(phoneKey, "test"), HttpRequestParameter("mid", mMapInfo.mid), HttpRequestParameter("rate", StringUtils::format("%i", Number)) };
	HttpServices::inst->sendRequest(this, resData, HttpRequestMethod::RATE_MAP);
	HttpServices::inst->setDelegate(this);
    CCLOG("%i", Number);
}

void RealGamePlayLayer::rateMap()
{
	auto mainMenuScene = MainMenuScene::createScene();
	Director::getInstance()->replaceScene(mainMenuScene);
}

void RealGamePlayLayer::addTile(std::string tileName, float xLoc, float yLoc){
	if (tileName == "0")
		return;

	GroundObject* go = new GroundObject(xLoc, yLoc, tileName);
	this->addChild(go);
}

#pragma region touch event
bool RealGamePlayLayer::onTouchBegan(Touch *touch, Event *event){
	this->mRunner->jump();
	return true;
}

void RealGamePlayLayer::onTouchMoved(Touch *touch, Event *event)
{

}

void RealGamePlayLayer::onTouchEnded(Touch *touch, Event *event)
{

}

void RealGamePlayLayer::onTouchCancelled(Touch *touch, Event *event)
{
	//this->mRunner->getb2PhysicsBody()->getBody()->SetLinearVelocity(b2Vec2(20.0f, 0));

}
#pragma endregion

#pragma region collision and response

void RealGamePlayLayer::BeginContact(b2Contact* contact)
{
	auto nodeA = (b2Node*)contact->GetFixtureA()->GetBody()->GetUserData();
	auto nodeB = (b2Node*)contact->GetFixtureB()->GetBody()->GetUserData();

	if (nodeA && nodeB)
	{
		int aTag = nodeA->getTag();
		int bTag = nodeB->getTag();

		if (aTag == TAG_OBJECT_PLAYER)
		{
			if (bTag == TAG_OBJECT_SLOW){
				GroundObject* snow = (GroundObject*)contact->GetFixtureB()->GetBody()->GetUserData();
				mRunner->collideSnowTile(snow);
			}

			if (bTag == TAG_OBJECT_BOMB){
				GroundObject* bomb = (GroundObject*)contact->GetFixtureB()->GetBody()->GetUserData();
				mRunner->collideBombTile(bomb);
			}

			else{
				this->mRunner->BeginContact(nodeB, contact);
			}
		}
		else
		{
			if (bTag == TAG_OBJECT_PLAYER)
			{
				if (aTag == TAG_OBJECT_SLOW){
					GroundObject* snow = (GroundObject*)contact->GetFixtureA()->GetBody()->GetUserData();
					mRunner->collideSnowTile(snow);
				}

				if (aTag == TAG_OBJECT_BOMB){
					GroundObject* bomb = (GroundObject*)contact->GetFixtureA()->GetBody()->GetUserData();
					mRunner->collideBombTile(bomb);
				}

				this->mRunner->BeginContact(nodeA, contact);
			}
		}
	}
}

void RealGamePlayLayer::PreSolve(b2Contact* contact, const b2Manifold* oldManifold)
{
	this->BeginContact(contact);
	//return true;
}

void RealGamePlayLayer::PostSolve(b2Contact* contact, const b2ContactImpulse* impulse)
{
	this->BeginContact(contact);
}

void RealGamePlayLayer::EndContact(b2Contact* contact)
{
	auto nodeA = (b2Node*)contact->GetFixtureA()->GetBody()->GetUserData();
	auto nodeB = (b2Node*)contact->GetFixtureB()->GetBody()->GetUserData();

	if (nodeA && nodeB)
	{
		int aTag = nodeA->getTag();
		int bTag = nodeB->getTag();

		if (aTag == TAG_OBJECT_PLAYER)
		{
			this->mRunner->EndContact(nodeB, contact);

		}
		else
		{
			if (bTag == TAG_OBJECT_PLAYER)
			{
				this->mRunner->EndContact(nodeA, contact);

			}
		}
	}
}

#pragma endregion

void RealGamePlayLayer::updateQuadTree(){


	if (quadtreeUpdateCounter++ > 10){
		quadtreeUpdateCounter = 0;

		QuadRect cameraRect = QuadRect(DESIGN_SCREEN_HEIGHT + this->mRunner->getPosition().y, this->mRunner->getPosition().y - DESIGN_SCREEN_HEIGHT,
			this->mRunner->getPosition().x - DESIGN_SCREEN_WIDTH,
			this->mRunner->getPosition().x + DESIGN_SCREEN_WIDTH);
		std::vector<ObjectNode>* object2bAdd = new std::vector<ObjectNode>();
		std::vector<ObjectNode>* object2bRemove = new std::vector<ObjectNode>();

		object2bAdd = this->quadTree->getObjectFromQuadtree(cameraRect,
			object2bAdd, this->currentObjectList, this->currentQuadNode);

		for (int i = 0; i != object2bAdd->size(); i++) {
			//addTile(origin->at(i).name, origin->at(i).rect.left + TILE_SIZE / 2, origin->at(i).rect.bot + TILE_SIZE / 2);
			//if (this->getChildByName(origin->at(i).id) != nullptr)
			//	continue;
			GroundObject* go = new GroundObject(object2bAdd->at(i).rect.left + TILE_SIZE / 2,
				object2bAdd->at(i).rect.bot + TILE_SIZE / 2, object2bAdd->at(i).name);
			go->setName(object2bAdd->at(i).id);
			this->addChild(go);
		}

		object2bRemove = this->quadTree->removeObjectFromQuadtree(cameraRect,
			object2bRemove, this->currentObjectList, this->currentQuadNode);

		for (int i = 0; i != object2bRemove->size(); i++) {
			this->removeChildByName(object2bRemove->at(i).id);
		}
	}
}