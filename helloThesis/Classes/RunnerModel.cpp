#include "RunnerModel.h"

bool RunnerModel::init(){
	//int num = 3;
	//b2Vec2 verts[] = {
	//	b2Vec2(0.0f, 40.8f),
	//	b2Vec2(-42.2f , 35.0f ),
	//	//b2Vec2(-20.0f , -40.0f ),
	//	//b2Vec2(20.0f , -40.0f ),
	//	b2Vec2(32.2f , 35.0f )
	//};
    
//	int num = 6;
//	b2Vec2 verts[] = {
//		b2Vec2(20, 50),
//		b2Vec2(0, 50),
//		b2Vec2(0, 20/3),
//		b2Vec2(20 / 3, 0),
//		b2Vec2(20*2/3, 0 ),
//		b2Vec2(20 , 20/3 +2.5)
//		//b2Vec2(32.2f , 35.0f )
//	};
//
//	auto b2PhysicBody = b2PhysicsBody::createPolygon(verts, num, b2PhysicsMaterial(0, 0, 0.1));
	auto b2PhysicBody = b2PhysicsBody::createBox(Size(34,70), b2PhysicsMaterial(0, 0, 0));
    
	b2PhysicBody->setBodyType(b2_dynamicBody);
	this->setb2PhysicsBody(b2PhysicBody);

	this->tag = TAG_OBJECT_PLAYER;

	this->direction = 1;
	this->changeDirectionCooldown = 1;
	this->scheduleUpdate();
	freezeeTime = 0;
	bombTime = 0;
	didRollBacked = false;
	return true;
}


void RunnerModel::update(float delta){
	//if (abs(this->getVelocityY()) < 0.2f){
	//	this->mState = PlayerState::ON_GROUND;
	//}
	//else{
	//	this->mState = PlayerState::ON_AIR;

	//}
}

bool RunnerModel::canJump(){
	bool onGround = isOnGround();
	float y = this->getVelocityY();
	return bombTime<0&&( onGround || (this->getVelocityY()<1 && isMultiJump));
}

bool RunnerModel::isOnGround(){
	bool statusCheck = this->mState == PlayerState::ON_GROUND ? true : false;
	//bool velocityCheck = this->getb2PhysicsBody()->getVelocityY() > -0.5f && this->getb2PhysicsBody()->getVelocityY() < 5.0f;
	return statusCheck;
}

float RunnerModel::getJumpSpeed(){
	if (jumpSpeedBoostCharges > 0){
		jumpSpeedBoostCharges--;
		return BOOSTED_JUMP_SPEED;
	}
	else{
		bool isJumpingMulti = isMultiJump;
		float x = this->getPositionX();
		if (freezeeTime > 0){
			return NORMAL_JUMP_SPEED* (FREEZEE_TIME - freezeeTime) / FREEZEE_TIME;
		}
		//if (isJumpingMulti)
		return isJumpingMulti ? NORMAL_JUMP_SPEED/2 : NORMAL_JUMP_SPEED;
	}
}

void RunnerModel::setState(PlayerState state){
	this->mState = state;
}

PlayerState RunnerModel::getState(){
	return mState;
}

float RunnerModel::getVelocityX(){
	return this->getb2PhysicsBody()->getVelocityX();
}

float RunnerModel::getVelocityY(){
	return this->getb2PhysicsBody()->getVelocityY();
}

void RunnerModel::setVelocityX(float value){
	this->getb2PhysicsBody()->setVelocityX(value);
}

void RunnerModel::setVelocityY(float value){
	this->getb2PhysicsBody()->setVelocityY(value);
}

float RunnerModel::getPosX(){
	return this->getb2PhysicsBody()->getb2Position().x;
}

float RunnerModel::getPosY(){
	return this->getb2PhysicsBody()->getb2Position().y;
}


void RunnerModel::setDirection(int dir){
	direction = dir;
}

int RunnerModel::getDirection()
{
	return direction;
}

bool RunnerModel::finish()
{
	return isFinish;
}

void RunnerModel::setFinish(bool finish)
{
	isFinish = finish;
}