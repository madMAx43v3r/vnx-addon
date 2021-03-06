
// AUTO GENERATED by vnxcppcodegen

#ifndef INCLUDE_vnx_addons_FileServerBase_HXX_
#define INCLUDE_vnx_addons_FileServerBase_HXX_

#include <vnx/addons/package.hxx>
#include <vnx/Buffer.hpp>
#include <vnx/Module.h>
#include <vnx/addons/HttpData.hxx>
#include <vnx/addons/HttpRequest.hxx>
#include <vnx/addons/HttpResponse.hxx>
#include <vnx/addons/file_info_t.hxx>


namespace vnx {
namespace addons {

class FileServerBase : public ::vnx::Module {
public:
	
	std::string www_root;
	std::map<std::string, std::string> mime_type_map;
	std::vector<std::string> directory_files;
	std::string redirect_not_found;
	vnx::bool_t allow_directory_list = 0;
	vnx::bool_t read_only = true;
	vnx::bool_t show_hidden = false;
	int32_t max_queue_ms = 1000;
	int32_t max_queue_size = 1000;
	uint64_t limit_no_chunk = 1048576;
	
	typedef ::vnx::Module Super;
	
	static const vnx::Hash64 VNX_TYPE_HASH;
	static const vnx::Hash64 VNX_CODE_HASH;
	
	static constexpr uint64_t VNX_TYPE_ID = 0xcf578d3ac2b39852ull;
	
	FileServerBase(const std::string& _vnx_name);
	
	vnx::Hash64 get_type_hash() const override;
	std::string get_type_name() const override;
	const vnx::TypeCode* get_type_code() const override;
	
	void read(std::istream& _in) override;
	void write(std::ostream& _out) const override;
	
	void accept(vnx::Visitor& _visitor) const override;
	
	vnx::Object to_object() const override;
	void from_object(const vnx::Object& object) override;
	
	vnx::Variant get_field(const std::string& name) const override;
	void set_field(const std::string& name, const vnx::Variant& value) override;
	
	friend std::ostream& operator<<(std::ostream& _out, const FileServerBase& _value);
	friend std::istream& operator>>(std::istream& _in, FileServerBase& _value);
	
	static const vnx::TypeCode* static_get_type_code();
	static std::shared_ptr<vnx::TypeCode> static_create_type_code();
	
protected:
	using Super::handle;
	
	virtual ::vnx::Buffer read_file(const std::string& path) const = 0;
	virtual ::vnx::Buffer read_file_range(const std::string& path, const int64_t& offset, const int64_t& length) const = 0;
	virtual ::vnx::addons::file_info_t get_file_info(const std::string& path) const = 0;
	virtual std::vector<::vnx::addons::file_info_t> read_directory(const std::string& path) const = 0;
	virtual void write_file(const std::string& path, const ::vnx::Buffer& data) = 0;
	virtual void delete_file(const std::string& path) = 0;
	virtual void http_request_async(std::shared_ptr<const ::vnx::addons::HttpRequest> request, const std::string& sub_path, const vnx::request_id_t& _request_id) const = 0;
	void http_request_async_return(const vnx::request_id_t& _request_id, const std::shared_ptr<const ::vnx::addons::HttpResponse>& _ret_0) const;
	virtual void http_request_chunk_async(std::shared_ptr<const ::vnx::addons::HttpRequest> request, const std::string& sub_path, const int64_t& offset, const int64_t& max_bytes, const vnx::request_id_t& _request_id) const = 0;
	void http_request_chunk_async_return(const vnx::request_id_t& _request_id, const std::shared_ptr<const ::vnx::addons::HttpData>& _ret_0) const;
	
	void vnx_handle_switch(std::shared_ptr<const vnx::Value> _value) override;
	std::shared_ptr<vnx::Value> vnx_call_switch(std::shared_ptr<const vnx::Value> _method, const vnx::request_id_t& _request_id) override;
	
};


} // namespace vnx
} // namespace addons


namespace vnx {

} // vnx

#endif // INCLUDE_vnx_addons_FileServerBase_HXX_
