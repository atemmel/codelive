#include "usersocket.h"
void usersocket::handleNewMessage(const WebSocketConnectionPtr& wsConnPtr, std::string &&message, const WebSocketMessageType &type)
{
	std::cout << "New message recieved: " << message << '\n';
	wsConnPtr->send(message);
}
void usersocket::handleNewConnection(const HttpRequestPtr &req,const WebSocketConnectionPtr& wsConnPtr)
{
	std::cout << "New connection established\n";
}
void usersocket::handleConnectionClosed(const WebSocketConnectionPtr& wsConnPtr)
{
	std::cout << "Connection terminated\n";
}
