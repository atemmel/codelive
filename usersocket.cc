#include "usersocket.h"
usersocket::usersocket() {
}
void usersocket::handleNewMessage(const WebSocketConnectionPtr& wsConnPtr, std::string &&message, const WebSocketMessageType &type)
{
	if(message.empty() || message == "null") {
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

		std::cout << "Message recieved: " << message << '\n';

		const Json::Value queue = value["queue"];
		if(!queue.isArray() ) return;
		
		for(const auto &q : queue) {
			//insertDelta
		}

		for(const auto &client : _pool) {
			if(client != wsConnPtr) {
				client->send(message);
			}
		}

	} catch(Json::LogicError err) {
		std::cout << err.what() << '\n';
	}
}
void usersocket::handleNewConnection(const HttpRequestPtr &req,const WebSocketConnectionPtr& wsConnPtr)
{
	_pool.insert(wsConnPtr);
	std::cout << "New connection established\n";
	std::cout << _pool.size() << " connections\n";
	Json::StreamWriterBuilder builder;
	builder["indentation"] = "";
	std::lock_guard<std::mutex> guard(_valueMutex);
	const std::string output = Json::writeString(builder, _value);
	wsConnPtr->send(output);
}
void usersocket::handleConnectionClosed(const WebSocketConnectionPtr& wsConnPtr)
{
	_pool.erase(wsConnPtr);
	std::cout << "Connection terminated\n";
	std::cout << _pool.size() << " connections\n";
}
