#include "oatpp/base/Log.hpp"
#include "oatpp/Environment.hpp"
#include "oatpp/network/Server.hpp"
#include "oatpp/network/tcp/server/ConnectionProvider.hpp"
#include "oatpp/web/server/HttpConnectionHandler.hpp"
#include "oatpp/web/server/HttpRouter.hpp"
#include "oatpp/web/protocol/http/incoming/Request.hpp"
#include "oatpp/web/protocol/http/outgoing/Response.hpp"


#include "oatpp/Types.hpp" // 包含基本类型
#include "oatpp/macro/codegen.hpp" // 包含代码生成宏
#include "oatpp/json/ObjectMapper.hpp" // 包含 JSON 对象映射器

#include OATPP_CODEGEN_BEGIN(DTO) // 开始生成 DTO 代码	
class MessageDto : public oatpp::DTO {
	DTO_INIT(MessageDto, DTO)
	DTO_FIELD(Int32, statusCode);
	DTO_FIELD(String, message);   
};
#include OATPP_CODEGEN_END(DTO) // 结束生成 DTO 代码

#define O_UNUSED(x) (void)x;

// 自定义请求处理程序
class Handler : public oatpp::web::server::HttpRequestHandler
{
private:
    std::shared_ptr<oatpp::json::ObjectMapper> myObjectMapper;
public:
    // 处理传入的请求，并返回响应
    // std::shared_ptr<OutgoingResponse> handle(const std::shared_ptr<IncomingRequest>& request) override {
    //     O_UNUSED(request);
 
    //     return ResponseFactory::createResponse(Status::CODE_200, "Hello, World!");
    // }

	Handler(const std::shared_ptr<oatpp::json::ObjectMapper>& objectMapper) 
		: myObjectMapper(objectMapper)
		{
			
		}

	std::shared_ptr<OutgoingResponse> handle(const std::shared_ptr<IncomingRequest>& request) override {
    auto message = MessageDto::createShared();
    message->statusCode = 1024;
    message->message = "Hello DTO!";
    return ResponseFactory::createResponse(Status::CODE_200, message, myObjectMapper);
	
  }
};

void run()
{
	auto objectMapper = std::make_shared<oatpp::json::ObjectMapper>();;// 创建对象映射器
    // 为 HTTP 请求创建路由器
    auto router = oatpp::web::server::HttpRouter::createShared();

    // 路由 GET - "/hello" 请求到处理程序
    router->route("GET", "/hello", std::make_shared<Handler>(objectMapper));

    // 创建 HTTP 连接处理程序
    auto connectionHandler = oatpp::web::server::HttpConnectionHandler::createShared(router);

    // 创建 TCP 连接提供者
    auto connectionProvider = oatpp::network::tcp::server::ConnectionProvider::createShared({"localhost", 8000, oatpp::network::Address::IP_4});

    // 创建服务器，它接受提供的 TCP 连接并将其传递给 HTTP 连接处理程序
    oatpp::network::Server server(connectionProvider, connectionHandler);

    // 打印服务器端口
    OATPP_LOGi("MyApp", "Server running on port %s", connectionProvider->getProperty("port").getData());

    // 运行服务器
    server.run();
}

int main()
{
    // 初始化 oatpp 环境
    oatpp::Environment::init();

    // 运行应用
    run();

    // 销毁 oatpp 环境
    oatpp::Environment::destroy();

    return 0;
}

