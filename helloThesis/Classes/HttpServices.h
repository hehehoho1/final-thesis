#ifndef __HTTP_SERVICES_H__
#define __HTTP_SERVICES_H__

#include "cocos2d.h"



class HttpServices;

#include "GameConfig.h"
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
#include "HttpClient.h"
#include "Json.h"
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
#include "network\HttpClient.h"
#include "util\json\Json.h"
#endif
#include "HttpServicesDelegate.h"
#include "Utility.h"

#include "LoadingHUDLayer.h"


USING_NS_CC;
using namespace cocos2d::network;


class HttpServices
{
private:
	HttpServicesDelegate* mDelegate;
	Layer* mCurrentLayer;

	LoadingHUDLayer *mLoadingHUD;
	void showLoading(Layer* layer, HttpRequestMethod method);
	void hideLoading(bool isSucess, HttpRequestMethod method);

	inline bool checkValid(bool ass, HttpRequestMethod method);

	std::string getMethodName(HttpRequestMethod method);

	void returnDelegate(HttpRequestMethod method, std::map<std::string, std::string> response);

	//void deserResponseData(HttpRequestMethod type, std::map<std::string, std::string> response);
public:

	void setDelegate(HttpServicesDelegate* pDelegate) { mDelegate = pDelegate; }

	//Http Response Callback
	void onHttpRequestCompleted(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response, HttpRequestMethod type);

	void sendRequest(cocos2d::Ref *sender, std::vector<HttpRequestParameter> RequestParameter, HttpRequestMethod type, bool isImmediate = false);

	static HttpServices* inst;
};



#endif // __HELLOWORLD_SCENE_H__
