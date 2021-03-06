#include "DBContext.h"
UserDefault* DBContext::context = nullptr;
void DBContext::init(){
	context =UserDefault::sharedUserDefault();
}

std::string DBContext::get(const char* key){
	std::string result = context->getStringForKey(key);
	return context->getStringForKey(key);
}

void DBContext::set(const char* key, std::string value){
	context->setStringForKey(key, value);
	context->flush();
}

GLboolean DBContext::keyExist(const char* key){
	return !context->getStringForKey(key).empty();
}

void DBContext::del(const char* key){
	context->setStringForKey(key, "");
}