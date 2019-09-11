#include "usersocket.h"
usersocket::usersocket() {
	_value["str"] = "";
	_value["clear"] = "false";
}
void usersocket::handleNewMessage(const WebSocketConnectionPtr& wsConnPtr, std::string &&message, const WebSocketMessageType &type)
{
	if(message == "null") {
		std::cout << "Message empty, returning previous string\n";
		wsConnPtr->send("null");
		return;
	}

	auto asStr = [&](){
		std::stringstream stream;
		for(auto &line : _document) {
			stream << line << '\n';
		}
		return stream.str();
	};

	auto insertDelta = [&](const std::string &str, int col, int row) {
		
	};

	try {
		Json::Value value;
		Json::Reader reader;
		Json::StreamWriterBuilder builder;
		builder["indentation"] = "";
		reader.parse(message, value);

		const Json::Value clear = value["clear"];
		if(!clear.isBool() ) return;
		if(clear.asBool() ) {
			std::lock_guard<std::mutex> guard(_valueMutex);
			const std::string output = Json::writeString(builder, _value);
			wsConnPtr->send(output);
			return;
		}

		std::cout << "New message recieved: " << message << '\n';

		std::lock_guard<std::mutex> guard(_valueMutex);
		std::swap(_value, value);
		const std::string output = Json::writeString(builder, _value);
		std::cout << "Data contained is now: " << output << '\n';
		wsConnPtr->send("null");
	} catch(Json::LogicError err) {
		std::cout << err.what() << '\n';
	}
}
void usersocket::handleNewConnection(const HttpRequestPtr &req,const WebSocketConnectionPtr& wsConnPtr)
{
	std::cout << "New connection established\n";
	Json::StreamWriterBuilder builder;
	builder["indentation"] = "";
	std::lock_guard<std::mutex> guard(_valueMutex);
	const std::string output = Json::writeString(builder, _value);
	wsConnPtr->send(output);
}
void usersocket::handleConnectionClosed(const WebSocketConnectionPtr& wsConnPtr)
{
	std::cout << "Connection terminated\n";
}
