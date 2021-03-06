#include "GroundObject.h"

std::vector<GroundObject*> GroundObject::changeDirectionTiles;

GroundObject::GroundObject(float xLoc, float yLoc, std::string tileName){
	bool isEnd = false;
	Sprite* sprite = Sprite::create(tileName + ".png");
	this->addChild(sprite);
	

	std::vector<std::string> tTypeObject = Utility::splitString(tileName, ",");
	std::string tType = tTypeObject[1];
	GroundType type;

	if (tType == "1"){
		type = GroundType::NORMAL;
		createNormalPhysicBody(sprite);
        this->setTag(TAG_OBJECT_GROUND);
	}
	else if (tType == "2"){
		type = GroundType::SLOPE;
		createSlopePhysicBody(sprite, tTypeObject[2]);
        this->setTag(TAG_OBJECT_GROUND);
	}
	else if (tType == "3")
	{
		type = GroundType::BARNORMAL;
		createBarNormalPhysicBody(sprite);
        this->setTag(TAG_OBJECT_BARNORMAL);
	}
	else if (tType == "4")
	{
		type = GroundType::BARSLOPE;
		createBarSlopePhysicBody(sprite, tTypeObject[2]);
        this->setTag(TAG_OBJECT_BARSLOPE);
	}
	else if (tType == "5")
	{
		createNormalPhysicBody(sprite);
		this->setTag(TAG_OBJECT_BACK);
	}
	else if (tType == "6")
	{
		createNormalPhysicBody(sprite);
		this->setTag(TAG_OBJECT_END);
	}
    else if (tType == "7")
    {
        createNormalPhysicBody(sprite);
		this->removeChild(sprite);
		auto snow = SnowFlake::create();
		snow->setTag(6969);
		this->addChild(snow);
        this->setTag(TAG_OBJECT_SLOW);
    }
	else if (tType == "8")
	{
		createNormalPhysicBody(sprite);
		this->removeChild(sprite);
		auto bomb = Bomb::create();
		bomb->setTag(6969);
		this->addChild(bomb);
		this->setTag(TAG_OBJECT_BOMB);
	}
	else if (tType == "9")
	{
		createNormalPhysicBody(sprite);
		this->setTag(TAG_OBJECT_ROLL_BACK);
	}
	/*else
	{
		type = GroundType::END;
		createEndlPhysicBody(sprite);
        this->setTag(TAG_OBJECT_BACK);
		isEnd = true;
	}*/

	
	this->setb2Position(xLoc, yLoc + sprite->getContentSize().height / 2);
	this->setPosition(xLoc, yLoc + sprite->getContentSize().height / 2);

	if (isEnd)
		changeDirectionTiles.push_back(this);

}

bool GroundObject::isChangeDirTile(Node* testTile){
	for (auto &tile : GroundObject::changeDirectionTiles){
		if (tile->getPosition().x == testTile->getPosition().x&&tile->getPosition().y == testTile->getPosition().y)
			return true;
	}
	//return false;
	//return this->isChangeDirectionTile;
}

void GroundObject::createNormalPhysicBody(Sprite* sprite){
	b2PhysicsBody *b2PhysicBody = new b2PhysicsBody();
	//b2Vec2 verts[] = {
	//	//b2Vec2(-sprite->getContentSize().width / 2, -sprite->getContentSize().height / 2),
	//	b2Vec2((int)-sprite->getContentSize().width / 2, sprite->getContentSize().height / 2),
	//	b2Vec2(sprite->getContentSize().width / 2, sprite->getContentSize().height / 2)
	//	//b2Vec2(sprite->getContentSize().width / 2, -sprite->getContentSize().height / 2)
	//};
	//b2PhysicBody = b2PhysicsBody::createChain(verts, 2,
	//	b2PhysicsMaterial(0, 0, 0));

	b2PhysicBody = b2PhysicsBody::createBox(Size(sprite->getContentSize().width - 1 , sprite->getContentSize().height),
		b2PhysicsMaterial(0, 0.01, 0));

	//int num = 4;
	//b2Vec2 verts[] = {
	//	b2Vec2((int)-sprite->getContentSize().width / 2, (int)-sprite->getContentSize().height / 2),
	//	b2Vec2((int)-sprite->getContentSize().width / 2, (int)sprite->getContentSize().height / 2),
	//	b2Vec2((int)sprite->getContentSize().width / 2, (int)sprite->getContentSize().height / 2),
	//	b2Vec2((int)sprite->getContentSize().width / 2, (int)-sprite->getContentSize().height / 2)
	//	//b2Vec2(32.2f , 35.0f )
	//};
	// b2PhysicBody = b2PhysicsBody::createPolygon(verts, num, b2PhysicsMaterial(0, 0.01, 0));

	b2PhysicBody->setBodyType(b2_staticBody);
	this->setb2PhysicsBody(b2PhysicBody);
}

void GroundObject::createSlopePhysicBody(Sprite* sprite, std::string type){
	b2PhysicsBody *b2PhysicBody = new b2PhysicsBody();
	//b2Vec2 verts[] = {
	//	b2Vec2(sprite->getContentSize().width / 2, sprite->getContentSize().height / 2),
	//	b2Vec2(-sprite->getContentSize().width / 2, -sprite->getContentSize().height / 2)
	//};
	//b2PhysicBody = b2PhysicsBody::createChain(verts, 2,
	//	b2PhysicsMaterial(0, 0.1, 1));

	int num = 3;
	if (type == "1")
	{
		b2Vec2 verts[] = {
			b2Vec2(sprite->getContentSize().width / 2, sprite->getContentSize().height / 2),
			b2Vec2(-sprite->getContentSize().width / 2, -sprite->getContentSize().height / 2),
			b2Vec2(sprite->getContentSize().width / 2, -sprite->getContentSize().height / 2)
		};
		b2PhysicBody = b2PhysicsBody::createPolygon(verts, num, b2PhysicsMaterial(0, 0.01, 0));
	}
	else
	{
		b2Vec2 verts[] = {
			b2Vec2(-sprite->getContentSize().width / 2, sprite->getContentSize().height / 2),
			b2Vec2(-sprite->getContentSize().width / 2, -sprite->getContentSize().height / 2),
			b2Vec2(sprite->getContentSize().width / 2, -sprite->getContentSize().height / 2)
		};
		b2PhysicBody = b2PhysicsBody::createPolygon(verts, num, b2PhysicsMaterial(0, 0.01, 0));
	}

	b2PhysicBody->setBodyType(b2_staticBody);
	this->setb2PhysicsBody(b2PhysicBody);
}

void GroundObject::createBarNormalPhysicBody(Sprite* sprite)
{
	b2PhysicsBody *b2PhysicBody = new b2PhysicsBody();

	int num = 4;
	b2Vec2 verts[] = {
		b2Vec2(-sprite->getContentSize().width / 2, -sprite->getContentSize().height / 2),
		b2Vec2(-sprite->getContentSize().width / 2, -sprite->getContentSize().height / 2 + sprite->getContentSize().height/3),
		b2Vec2(sprite->getContentSize().width / 2, -sprite->getContentSize().height / 2 + sprite->getContentSize().height / 3),
		b2Vec2(sprite->getContentSize().width / 2, -sprite->getContentSize().height / 2)
	};

	b2PhysicBody = b2PhysicsBody::createPolygon(verts, num, b2PhysicsMaterial(0, 0.01, 0));

	b2PhysicBody->setBodyType(b2_staticBody);
	this->setb2PhysicsBody(b2PhysicBody);
}

void GroundObject::createBarSlopePhysicBody(Sprite* sprite, std::string type)
{
	b2PhysicsBody *b2PhysicBody = new b2PhysicsBody();

	int num = 4;
	

	if (type == "1")
	{
		b2Vec2 verts[] = {
			b2Vec2(sprite->getContentSize().width / 2, sprite->getContentSize().height / 2),
			b2Vec2(-sprite->getContentSize().width / 2, -sprite->getContentSize().height / 2),
			b2Vec2(-sprite->getContentSize().width / 2 + 4, -sprite->getContentSize().height / 2),
			b2Vec2(sprite->getContentSize().width / 2, sprite->getContentSize().height / 2 - 4)
		};
		b2PhysicBody = b2PhysicsBody::createPolygon(verts, num, b2PhysicsMaterial(0, 0.01, 0));
	}
	else
	{
		b2Vec2 verts[] = {
			b2Vec2(-sprite->getContentSize().width / 2, sprite->getContentSize().height / 2),
			b2Vec2(sprite->getContentSize().width / 2, -sprite->getContentSize().height / 2),
			b2Vec2(sprite->getContentSize().width / 2 - 4, -sprite->getContentSize().height / 2),
			b2Vec2(-sprite->getContentSize().width / 2, sprite->getContentSize().height / 2 - 4)
		};
		b2PhysicBody = b2PhysicsBody::createPolygon(verts, num, b2PhysicsMaterial(0, 0.01, 0));
	}

	b2PhysicBody->setBodyType(b2_staticBody);
	this->setb2PhysicsBody(b2PhysicBody);
}

void GroundObject::createEndlPhysicBody(Sprite* sprite){
	b2PhysicsBody *b2PhysicBody = new b2PhysicsBody();
	b2PhysicBody = b2PhysicsBody::createBox(Size(sprite->getContentSize().width, sprite->getContentSize().height), 
		b2PhysicsMaterial(0, 0, 0.1));

	b2PhysicBody->setBodyType(b2_staticBody);
	this->setb2PhysicsBody(b2PhysicBody);
}

GroundType GroundObject::getType()
{
	return type;
}

void GroundObject::snowTileExplode(){
	SnowFlake* snowTile = (SnowFlake*)getChildByTag(6969);
	if (!snowTile->exploded){
		snowTile->explode();
		this->setTag(TAG_OBJECT_SLOW_EXLODED);
	}
}

void GroundObject::bombTileExplode(){
	Bomb* bombTile = (Bomb*)getChildByTag(6969);
	if (!bombTile->exploded){
		bombTile->explode();
		this->setTag(TAG_OBJECT_BOMB_EXLODED);
	}
}