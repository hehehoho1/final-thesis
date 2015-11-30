#include "HelloWorldScene.h"
#include <string>
#include "spine/Json.h"

USING_NS_CC;
//USING_NS_CC_EXT;
using namespace cocos2d::network;

Scene* HelloWorld::createScene()
{
	// 'scene' is an autorelease object
	auto scene = Scene::create();

	// 'layer' is an autorelease obsject
	auto layer = HelloWorld::create();

	// add layer as a child to scene
	scene->addChild(layer);

	// return the scene
	return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
	//////////////////////////////
	// 1. super init first
	if (!Layer::init())
	{
		return false;
	}

	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	const int MARGIN = 40;
	const int SPACE = 35;

	const int LEFT = visibleSize.width / 2;

	auto menuRequest = Menu::create();
	menuRequest->setPosition(Vec2::ZERO);
	addChild(menuRequest);

	// Post
	auto labelPost = Label::createWithTTF("Test Post", "fonts/arial.ttf", 22);
	auto itemPost = MenuItemLabel::create(labelPost, CC_CALLBACK_1(HelloWorld::onMenuPostTestClicked, this, false));
	itemPost->setPosition(LEFT, visibleSize.height - MARGIN - 2 * SPACE);
	menuRequest->addChild(itemPost);

	// Response Code Label
	_labelStatusCode = Label::createWithTTF("HTTP Status Code", "fonts/arial.ttf", 18);
	_labelStatusCode->setPosition(visibleSize.width / 2, visibleSize.height - MARGIN - 6 * SPACE);
	addChild(_labelStatusCode);

	return true;
}


void HelloWorld::menuCloseCallback(Ref* pSender)
{
	Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	exit(0);
#endif
}


void HelloWorld::onHttpRequestCompleted(HttpClient *sender, HttpResponse *response)
{
	if (!response)
	{
		return;
	}
	
	// You can get original request type from: response->request->reqType
	if (0 != strlen(response->getHttpRequest()->getTag()))
	{
		log("%s completed", response->getHttpRequest()->getTag());
	}

	long statusCode = response->getResponseCode();
	char statusString[64] = {};
	sprintf(statusString, "HTTP Status Code: %ld, tag = %s", statusCode, response->getHttpRequest()->getTag());
	_labelStatusCode->setString(statusString);
	log("response code: %ld", statusCode);

	if (!response->isSucceed())
	{
		log("response failed");
		log("error buffer: %s", response->getErrorBuffer());
		return;
	}

	// dump data
	std::vector<char> *buffer = response->getResponseData();	
	char* text = reinterpret_cast<char*>(buffer->data());
	_labelStatusCode->setString(text);


	// The data will be placed in the buffer.
	/*std::vector<char> * buffer = response->getResponseData();
	char * concatenated = (char *)malloc(buffer->size() + 1);
	std::string s2(buffer->begin(), buffer->end());
	strcpy(concatenated, s2.c_str());*/

	 //JSON Parser. Include "spine/Json.h".
	//std::string a = "";
	//Json * json = Json_create(a);
	//const char * test1 = Json_getString(json, "test1", "default");
	//const char * test2 = Json_getString(json, "test2", "default");

	//// View the console
	//CCLog("HTTP Response : key 1 : %s", test1);
	//CCLog("HTTP Response : key 2 : %s", test2);

	//// Delete the JSON object
	//Json_dispose(json);
}


void HelloWorld::onMenuPostTestClicked(cocos2d::Ref *sender, bool isImmediate)
{

	HttpRequest* request = new (std::nothrow) HttpRequest();
	request->setUrl("http://192.168.1.13:8090/runner");
	request->setRequestType(HttpRequest::Type::POST);
	request->setResponseCallback(CC_CALLBACK_2(HelloWorld::onHttpRequestCompleted, this));

	// write the post data
	const char* postData = "{\"method\":\"uploadMap\", \"data\":{}}";
	request->setRequestData(postData, strlen(postData));
	if (isImmediate)
	{
		HttpClient::getInstance()->sendImmediate(request);
	}
	else
	{
		HttpClient::getInstance()->send(request);
	}
	request->release();
}


void HelloWorld::onMenuPostBinaryTestClicked(cocos2d::Ref *sender, bool isImmediate)
{
	
}
