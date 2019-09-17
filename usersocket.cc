#include "usersocket.h"
#include "redirect.h"
#include <fstream>
#include <filesystem>
usersocket::usersocket() {
}
void usersocket::handleNewMessage(const WebSocketConnectionPtr& wsConnPtr, std::string &&message, const WebSocketMessageType &type)
{
	if(message.empty() || message == "null") {
		return;
	}

	/*
	 *	Insert changes incrementally instead of fetching the entire string
	auto insertDelta = [&](const std::string &str, int col, int row) {
		
	};
	*/

	auto distributeOthers = [&](const std::string &msg) {
		for(const auto &client : _pool) {
			if(client != wsConnPtr) {
				client->send(msg);
			}
		}
	};

	auto distributeAll = [&](const std::string &msg) {
		for(const auto &client : _pool) {
			client->send(msg);
		}
	};

	auto consume = [&](std::fstream &stream) {
		auto size = stream.tellg();
		stream.seekg(0, std::ios::beg);
		std::vector<char> bytes(size);
		stream.read(bytes.data(), size);
		return std::string(bytes.data(), size );
	};

	auto run = [&](const Json::Value &var) {
		std::lock_guard<std::mutex> guard(_runningMutex);
		_running = true;

		Json::Value mess;
		Json::StreamWriterBuilder builder;
		mess["state"] = "halted";
		builder["indentation"] = "";
		std::string strin = Json::writeString(builder, mess);
		std::cout << "Sending message: " << strin <<'\n';
		distributeAll(strin);

		const auto path = std::filesystem::temp_directory_path();
		const auto fname = path / "main.cpp";
		const Json::Value str = var["contents"];
		std::fstream file(fname);
		if(file.is_open() ) std::cout << "File successfully opened\n";
		std::string contents = str.asString();
		std::cout << "Contents: " << contents << '\n';
		file << contents;
		file.close();
		std::cout << "Written contents to " << path.c_str() << '\n';

		redirect(std::string("g++ ") + fname.c_str() + " -Wall", path / "build.out", path / "build.err");

		file.open(path / "build.err", std::ios::in | std::ios::binary | std::ios::ate);

		if(file.is_open() ) {
			std::cout << "File is now open\n";
			std::string result = consume(file);
			if(result.empty() ) result = "Code successfully compiled\n";

			mess["state"] = "normal";
			mess["result"] = result;
			
			strin = Json::writeString(builder, mess);
			distributeAll(strin);
		} else std::cout << "File did not open\n";
			

		_running = false;
	};

	try {

		Json::Value value;
		Json::Reader reader;
		Json::StreamWriterBuilder builder;
		builder["indentation"] = "";
		reader.parse(message, value);

		std::cout << "Message recieved: " << message << '\n';

		const Json::Value queue = value["queue"];
		if(queue.isArray() ) {

			/*
			 * Insert changes incrementally
			for(const auto &q : queue) {
				insertDelta(q)
			}
			*/
			std::cout << "Queue is array" << '\n';

			distributeOthers(message);
			return;
		}

		const Json::Value requestRun = value["run"];
		if(requestRun.isBool() && !_running) {
			run(value);
			return;
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
