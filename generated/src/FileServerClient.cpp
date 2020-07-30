
// AUTO GENERATED by vnxcppcodegen

#include <vnx/addons/package.hxx>
#include <vnx/addons/FileServerClient.hxx>
#include <vnx/Buffer.hpp>
#include <vnx/Module.h>
#include <vnx/ModuleInterface_vnx_get_type_code.hxx>
#include <vnx/ModuleInterface_vnx_get_type_code_return.hxx>
#include <vnx/addons/FileServer_read_file.hxx>
#include <vnx/addons/FileServer_read_file_return.hxx>
#include <vnx/addons/HttpComponent_http_request.hxx>
#include <vnx/addons/HttpComponent_http_request_return.hxx>
#include <vnx/addons/HttpRequest.hxx>
#include <vnx/addons/HttpResponse.hxx>

#include <vnx/vnx.h>


namespace vnx {
namespace addons {

FileServerClient::FileServerClient(const std::string& service_name)
	:	Client::Client(vnx::Hash64(service_name))
{
}

FileServerClient::FileServerClient(vnx::Hash64 service_addr)
	:	Client::Client(service_addr)
{
}

::vnx::TypeCode FileServerClient::vnx_get_type_code() {
	auto _method = ::vnx::ModuleInterface_vnx_get_type_code::create();
	auto _return_value = vnx_request(_method);
	auto _result = std::dynamic_pointer_cast<const ::vnx::ModuleInterface_vnx_get_type_code_return>(_return_value);
	if(!_result) {
		throw std::logic_error("FileServerClient: !_result");
	}
	return _result->_ret_0;
}

::vnx::Buffer FileServerClient::read_file(const std::string& path) {
	auto _method = ::vnx::addons::FileServer_read_file::create();
	_method->path = path;
	auto _return_value = vnx_request(_method);
	auto _result = std::dynamic_pointer_cast<const ::vnx::addons::FileServer_read_file_return>(_return_value);
	if(!_result) {
		throw std::logic_error("FileServerClient: !_result");
	}
	return _result->_ret_0;
}

std::shared_ptr<const ::vnx::addons::HttpResponse> FileServerClient::http_request(const std::shared_ptr<const ::vnx::addons::HttpRequest>& request, const std::string& sub_path) {
	auto _method = ::vnx::addons::HttpComponent_http_request::create();
	_method->request = request;
	_method->sub_path = sub_path;
	auto _return_value = vnx_request(_method);
	auto _result = std::dynamic_pointer_cast<const ::vnx::addons::HttpComponent_http_request_return>(_return_value);
	if(!_result) {
		throw std::logic_error("FileServerClient: !_result");
	}
	return _result->_ret_0;
}


} // namespace vnx
} // namespace addons