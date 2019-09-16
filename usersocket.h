#pragma once
#include <drogon/WebSocketController.h>
using namespace drogon;
class usersocket:public drogon::WebSocketController<usersocket>
{
public:
	using Document = std::vector<std::string>;
	usersocket();
    virtual void handleNewMessage(const WebSocketConnectionPtr&,
                                  std::string &&,
                                  const WebSocketMessageType &) override;
    virtual void handleNewConnection(const HttpRequestPtr &,
                                     const WebSocketConnectionPtr&)override;
    virtual void handleConnectionClosed(const WebSocketConnectionPtr&)override;
    WS_PATH_LIST_BEGIN
    //list path definitions here;
    WS_PATH_ADD("/chat");
    WS_PATH_LIST_END
private:
		using Pool = std::set<WebSocketConnectionPtr>;
		Pool _pool;
		std::mutex _valueMutex;
		Json::Value _value;
		Document _document;
};
