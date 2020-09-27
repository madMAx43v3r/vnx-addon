
// AUTO GENERATED by vnxcppcodegen

#include <vnx/addons/package.hxx>
#include <vnx/addons/HttpComponentClient.hxx>
#include <vnx/addons/HttpComponent_http_request.hxx>
#include <vnx/addons/HttpComponent_http_request_return.hxx>
#include <vnx/addons/HttpRequest.hxx>
#include <vnx/addons/HttpResponse.hxx>

#include <vnx/vnx.h>


namespace vnx {
namespace addons {

HttpComponentClient::HttpComponentClient(const std::string& service_name)
	:	Client::Client(vnx::Hash64(service_name))
{
}

HttpComponentClient::HttpComponentClient(vnx::Hash64 service_addr)
	:	Client::Client(service_addr)
{
}

std::shared_ptr<const ::vnx::addons::HttpResponse> HttpComponentClient::http_request(const std::shared_ptr<const ::vnx::addons::HttpRequest>& request, const std::string& sub_path) {
	auto _method = ::vnx::addons::HttpComponent_http_request::create();
	_method->request = request;
	_method->sub_path = sub_path;
	auto _return_value = vnx_request(_method, false);
	auto _result = std::dynamic_pointer_cast<const ::vnx::addons::HttpComponent_http_request_return>(_return_value);
	if(!_result) {
		throw std::logic_error("HttpComponentClient: !_result");
	}
	return _result->_ret_0;
}


} // namespace vnx
} // namespace addons
