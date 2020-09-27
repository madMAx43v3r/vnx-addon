
// AUTO GENERATED by vnxcppcodegen

#ifndef INCLUDE_vnx_addons_FileServer_CLIENT_HXX_
#define INCLUDE_vnx_addons_FileServer_CLIENT_HXX_

#include <vnx/Client.h>
#include <vnx/Buffer.hpp>
#include <vnx/Module.h>
#include <vnx/addons/HttpRequest.hxx>
#include <vnx/addons/HttpResponse.hxx>


namespace vnx {
namespace addons {

class FileServerClient : public vnx::Client {
public:
	FileServerClient(const std::string& service_name);
	
	FileServerClient(vnx::Hash64 service_addr);
	
	::vnx::Object vnx_get_config_object();
	
	::vnx::Variant vnx_get_config(const std::string& name);
	
	void vnx_set_config_object(const ::vnx::Object& config);
	
	void vnx_set_config_object_async(const ::vnx::Object& config);
	
	void vnx_set_config(const std::string& name, const ::vnx::Variant& value);
	
	void vnx_set_config_async(const std::string& name, const ::vnx::Variant& value);
	
	::vnx::TypeCode vnx_get_type_code();
	
	void vnx_restart();
	
	void vnx_restart_async();
	
	void vnx_close();
	
	void vnx_close_async();
	
	::vnx::Buffer read_file(const std::string& path);
	
	std::shared_ptr<const ::vnx::addons::HttpResponse> http_request(const std::shared_ptr<const ::vnx::addons::HttpRequest>& request, const std::string& sub_path);
	
};


} // namespace vnx
} // namespace addons

#endif // INCLUDE_vnx_addons_FileServer_CLIENT_HXX_
