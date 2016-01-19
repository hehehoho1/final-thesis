#include "RunnerController.h"
bool isChangeDirection;
bool Runner::init(){
	//this->mModel = RunnerModel::create(); 
	isChangeDirection = false;
	this->mModel = new RunnerModel();
	this->mModel->init();
	this->addChild(mModel);
	this->mModel->getb2PhysicsBody()->setb2Position(100, 100);
	this->setb2PhysicsBody(this->mModel->getb2PhysicsBody());
	this->mView = RunnerView::create();
	this->addChild(mView);
	this->setTag(TAG_OBJECT_PLAYER);
	this->scheduleUpdate();
	return true;
}

void Runner::update(float delta){
	lastX = this->getPositionX();
	switch (this->mModel->getState()){

	case PlayerState::ON_GROUND: this->mView->runOnGround(); break;
	case PlayerState::ON_AIR: this->mView->runOnAir(); break;
	default:this->mView->runOnGround(); break;
	}
}

void Runner::BeginContact(b2Node* node, b2Contact* contact){
//	int targetTag = node->getTag();
//	switch (targetTag)
//	{
//	case TAG_OBJECT_GROUND:{
//							   this->collideGround(node, contact);
//							   break;
//	}
//	default:
//		break;
//	}
    this->collideGround(node, contact);
}

void Runner::collideGround(b2Node* groundNode, b2Contact* contact){
	int groundTag = groundNode->getTag();
	float tmp;
    switch (groundTag)
    	{
            case TAG_OBJECT_GROUND:
            {
					runNormal();
					float groundWidth = groundNode->getBoundingBox().getMaxX(); 
					bool positionCheckY = this->mModel->getPosY() - groundNode->getPosition().y > this->mModel->getBoundingBox().getMaxY() / 2;
					float a1 = std::abs(this->mModel->getPosX() - groundNode->getPosition().x);
					float a2 = this->mModel->getBoundingBox().getMaxX();
					bool positionCheckX = std::abs(this->mModel->getPosX() - groundNode->getPosition().x) < (groundWidth / 2 + this->mModel->getBoundingBox().getMaxX() / 2);
					if (positionCheckY&&positionCheckX)
						this->mModel->setState(PlayerState::ON_GROUND);
                    break;
            }
            case TAG_OBJECT_BARNORMAL:
            case TAG_OBJECT_BARSLOPE:
            {
                float groundWidth = groundNode->getBoundingBox().getMaxX();
                tmp = 10;
				bool velocityCheck = this->mModel->getVelocityY() > tmp;
				bool positionCheck = std::abs(this->mModel->getPosX() - groundNode->getPosition().x) < groundWidth / 2;

				if (velocityCheck && positionCheck){
                        contact->SetEnabled(false);
				}
				else{
					runNormal();
					float groundWidth = groundNode->getBoundingBox().getMaxX();
					bool positionCheckY = this->mModel->getPosY() - groundNode->getPosition().y > this->mModel->getBoundingBox().getMaxY() / 2;
					float a1 = std::abs(this->mModel->getPosX() - groundNode->getPosition().x);
					float a2 = this->mModel->getBoundingBox().getMaxX();
					bool positionCheckX = std::abs(this->mModel->getPosX() - groundNode->getPosition().x) < (groundWidth / 2 + this->mModel->getBoundingBox().getMaxX() / 2);
					if (positionCheckY&&positionCheckX)
						this->mModel->setState(PlayerState::ON_GROUND);
					else
						this->mModel->isMultiJump = true;
					break;
				}
                break;
            }
            case TAG_OBJECT_BACK:
            {
				if (!isChangeDirection)
				{
					mModel->setDirection(-1);
					mView->setDirection(-1);
					isChangeDirection = true;
				}
				
                break;
            }
			case TAG_OBJECT_END:
			{
				if (isChangeDirection)
				{
					isChangeDirection = false;
					mModel->setFinish(true);
				}
				break;
			}
            case TAG_OBJECT_SLOW:
            {
                
                break;
            }
            default:
                break;
    	}
}

void Runner::endCollideGround(){
	this->mModel->setState(PlayerState::ON_AIR);
	//this->mModel->isMultiJump = true;
}

void Runner::EndContact(b2Node* node, b2Contact* contact){
	int targetTag = node->getTag();
	switch (targetTag)
	{
	case TAG_OBJECT_GROUND:{
							   this->endCollideGround();
							   break;
	}
	default:
		break;
	}
}

void Runner::runNormal(){
	this->mModel->setVelocityX(15.0f*this->mModel->getDirection());
	//this->mModel->setState(PlayerState::ON_GROUND);
	this->mModel->isMultiJump = false;
}

void Runner::jump(){

	if (this->mModel->canJump()){
		float x = this->getPositionX();
		float y = this-> mModel->getVelocityY();
		if (std::abs(x - lastX) < 0.5f && y !=0.0f)
			this->mModel->isMultiJump = true;
		else
			this->mModel->isMultiJump = false;
		this->mModel->setState(PlayerState::ON_AIR);
		this->mModel->setVelocityY(this->mModel->getJumpSpeed());
	}
}

