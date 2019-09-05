#include "usersocket.h"
void usersocket::handleNewMessage(const WebSocketConnectionPtr& wsConnPtr, std::string &&message, const WebSocketMessageType &type)
{
	if(message.empty() ) {
		std::cout << "Message empty, returning previous string\n";
		wsConnPtr->send(_string);
		return;
	}
	_string = message;
	std::cout << "New message recieved: " << _string << '\n';
	wsConnPtr->send(_string);
}
void usersocket::handleNewConnection(const HttpRequestPtr &req,const WebSocketConnectionPtr& wsConnPtr)
{
	std::cout << "New connection established\n";
}
void usersocket::handleConnectionClosed(const WebSocketConnectionPtr& wsConnPtr)
{
	std::cout << "Connection terminated\n";
}
