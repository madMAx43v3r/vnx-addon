
// AUTO GENERATED by vnxcppcodegen

#ifndef INCLUDE_vnx_addons_FileServer_CLIENT_HXX_
#define INCLUDE_vnx_addons_FileServer_CLIENT_HXX_

#include <vnx/Client.h>
#include <vnx/Buffer.hpp>
#include <vnx/Module.h>
#include <vnx/addons/HttpRequest.hxx>
#include <vnx/addons/HttpResponse.hxx>
#include <vnx/addons/file_info_t.hxx>


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
	
	std::shared_ptr<const ::vnx::ModuleInfo> vnx_get_module_info();
	
	void vnx_restart();
	
	void vnx_restart_async();
	
	void vnx_stop();
	
	void vnx_stop_async();
	
	::vnx::Buffer read_file(const std::string& path);
	
	::vnx::Buffer read_file_range(const std::string& path, const int64_t& offset, const int64_t& length);
	
	::vnx::addons::file_info_t get_file_info(const std::string& path);
	
	std::vector<::vnx::addons::file_info_t> read_directory(const std::string& path);
	
	void write_file(const std::string& path, const ::vnx::Buffer& data);
	
	void write_file_async(const std::string& path, const ::vnx::Buffer& data);
	
	std::shared_ptr<const ::vnx::addons::HttpResponse> http_request(std::shared_ptr<const ::vnx::addons::HttpRequest> request, const std::string& sub_path);
	
	std::shared_ptr<const ::vnx::addons::HttpResponse> http_request_chunk(std::shared_ptr<const ::vnx::addons::HttpRequest> request, const std::string& sub_path, const int64_t& offset, const int64_t& max_bytes);
	
};


} // namespace vnx
} // namespace addons

#endif // INCLUDE_vnx_addons_FileServer_CLIENT_HXX_
