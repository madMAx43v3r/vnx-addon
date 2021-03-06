
// AUTO GENERATED by vnxcppcodegen

#ifndef INCLUDE_vnx_addons_HttpResponse_HXX_
#define INCLUDE_vnx_addons_HttpResponse_HXX_

#include <vnx/addons/package.hxx>
#include <vnx/Hash64.hpp>
#include <vnx/Object.hpp>
#include <vnx/Value.h>
#include <vnx/addons/HttpData.hxx>


namespace vnx {
namespace addons {

class HttpResponse : public ::vnx::addons::HttpData {
public:
	
	vnx::bool_t is_chunked = 0;
	int32_t status = 0;
	std::string content_type;
	std::vector<std::pair<std::string, std::string>> headers;
	::vnx::Hash64 stream;
	int64_t total_size = -1;
	std::string error_text;
	
	typedef ::vnx::addons::HttpData Super;
	
	static const vnx::Hash64 VNX_TYPE_HASH;
	static const vnx::Hash64 VNX_CODE_HASH;
	
	static constexpr uint64_t VNX_TYPE_ID = 0xb907701490f0e0feull;
	
	vnx::Hash64 get_type_hash() const override;
	std::string get_type_name() const override;
	const vnx::TypeCode* get_type_code() const override;
	
	static std::shared_ptr<const ::vnx::addons::HttpResponse> from_status(const int32_t& status = 0);
	static std::shared_ptr<const ::vnx::addons::HttpResponse> from_text(const std::string& content = "");
	static std::shared_ptr<const ::vnx::addons::HttpResponse> from_text_ex(const std::string& content = "", const int32_t& status = 0);
	static std::shared_ptr<const ::vnx::addons::HttpResponse> from_string(const std::string& content = "", const std::string& mime_type = "");
	static std::shared_ptr<const ::vnx::addons::HttpResponse> from_string_ex(const std::string& content = "", const std::string& mime_type = "", const int32_t& status = 0);
	static std::shared_ptr<const ::vnx::addons::HttpResponse> from_string_json(const std::string& content = "");
	static std::shared_ptr<const ::vnx::addons::HttpResponse> from_string_json_ex(const std::string& content = "", const int32_t& status = 0);
	static std::shared_ptr<const ::vnx::addons::HttpResponse> from_object_json(const ::vnx::Object& value = ::vnx::Object());
	static std::shared_ptr<const ::vnx::addons::HttpResponse> from_object_json_ex(const ::vnx::Object& value = ::vnx::Object(), const int32_t& status = 0);
	static std::shared_ptr<const ::vnx::addons::HttpResponse> from_value_json(std::shared_ptr<const ::vnx::Value> value = nullptr);
	static std::shared_ptr<const ::vnx::addons::HttpResponse> from_value_json_ex(std::shared_ptr<const ::vnx::Value> value = nullptr, const int32_t& status = 0);
	static std::shared_ptr<const ::vnx::addons::HttpResponse> from_value_binary(std::shared_ptr<const ::vnx::Value> value = nullptr);
	static std::shared_ptr<const ::vnx::addons::HttpResponse> from_value_binary_ex(std::shared_ptr<const ::vnx::Value> value = nullptr, const int32_t& status = 0);
	
	static std::shared_ptr<HttpResponse> create();
	std::shared_ptr<vnx::Value> clone() const override;
	
	void read(vnx::TypeInput& _in, const vnx::TypeCode* _type_code, const uint16_t* _code) override;
	void write(vnx::TypeOutput& _out, const vnx::TypeCode* _type_code, const uint16_t* _code) const override;
	
	void read(std::istream& _in) override;
	void write(std::ostream& _out) const override;
	
	void accept(vnx::Visitor& _visitor) const override;
	
	vnx::Object to_object() const override;
	void from_object(const vnx::Object& object) override;
	
	vnx::Variant get_field(const std::string& name) const override;
	void set_field(const std::string& name, const vnx::Variant& value) override;
	
	friend std::ostream& operator<<(std::ostream& _out, const HttpResponse& _value);
	friend std::istream& operator>>(std::istream& _in, HttpResponse& _value);
	
	static const vnx::TypeCode* static_get_type_code();
	static std::shared_ptr<vnx::TypeCode> static_create_type_code();
	
};


} // namespace vnx
} // namespace addons


namespace vnx {

} // vnx

#endif // INCLUDE_vnx_addons_HttpResponse_HXX_
