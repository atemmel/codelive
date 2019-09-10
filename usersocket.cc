#include "usersocket.h"
void usersocket::handleNewMessage(const WebSocketConnectionPtr& wsConnPtr, std::string &&message, const WebSocketMessageType &type)
{
	if(message == "null") {
		std::cout << "Message empty, returning previous string\n";
		wsConnPtr->send("null");
		return;
	}

	try {
		Json::Value value;
		Json::Reader reader;
		Json::StreamWriterBuilder builder;
		builder["indentation"] = "";
		reader.parse(message, value);

		std::cout << "New message recieved: " << message << '\n';

		const Json::Value clear = value["clear"];
		if(clear.asBool() ) {
			wsConnPtr->send(Json::writeString(builder, _value) );
			return;
		}

		std::swap(_value, value);
		const std::string output = Json::writeString(builder, _value);
		std::cout << "Data contained is now: " << output << '\n';
		wsConnPtr->send(output);
	} catch(Json::LogicError err) {
		std::cout << err.what() << '\n';
	}
}
void usersocket::handleNewConnection(const HttpRequestPtr &req,const WebSocketConnectionPtr& wsConnPtr)
{
	std::cout << "New connection established\n";
}
void usersocket::handleConnectionClosed(const WebSocketConnectionPtr& wsConnPtr)
{
	std::cout << "Connection terminated\n";
}
