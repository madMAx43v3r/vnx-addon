
// AUTO GENERATED by vnxcppcodegen

#include <vnx/addons/package.hxx>
#include <vnx/addons/file_info_t.hxx>

#include <vnx/vnx.h>


namespace vnx {
namespace addons {


const vnx::Hash64 file_info_t::VNX_TYPE_HASH(0x7a1208fc71e8e919ull);
const vnx::Hash64 file_info_t::VNX_CODE_HASH(0x2d88c73b68266d87ull);

vnx::Hash64 file_info_t::get_type_hash() const {
	return VNX_TYPE_HASH;
}

std::string file_info_t::get_type_name() const {
	return "vnx.addons.file_info_t";
}

const vnx::TypeCode* file_info_t::get_type_code() const {
	return vnx::addons::vnx_native_type_code_file_info_t;
}

std::shared_ptr<file_info_t> file_info_t::create() {
	return std::make_shared<file_info_t>();
}

std::shared_ptr<file_info_t> file_info_t::clone() const {
	return std::make_shared<file_info_t>(*this);
}

void file_info_t::read(vnx::TypeInput& _in, const vnx::TypeCode* _type_code, const uint16_t* _code) {
	vnx::read(_in, *this, _type_code, _code);
}

void file_info_t::write(vnx::TypeOutput& _out, const vnx::TypeCode* _type_code, const uint16_t* _code) const {
	vnx::write(_out, *this, _type_code, _code);
}

void file_info_t::accept(vnx::Visitor& _visitor) const {
	const vnx::TypeCode* _type_code = vnx::addons::vnx_native_type_code_file_info_t;
	_visitor.type_begin(*_type_code);
	_visitor.type_field(_type_code->fields[0], 0); vnx::accept(_visitor, name);
	_visitor.type_field(_type_code->fields[1], 1); vnx::accept(_visitor, mime_type);
	_visitor.type_field(_type_code->fields[2], 2); vnx::accept(_visitor, size);
	_visitor.type_field(_type_code->fields[3], 3); vnx::accept(_visitor, last_modified);
	_visitor.type_field(_type_code->fields[4], 4); vnx::accept(_visitor, is_directory);
	_visitor.type_end(*_type_code);
}

void file_info_t::write(std::ostream& _out) const {
	_out << "{";
	_out << "\"name\": "; vnx::write(_out, name);
	_out << ", \"mime_type\": "; vnx::write(_out, mime_type);
	_out << ", \"size\": "; vnx::write(_out, size);
	_out << ", \"last_modified\": "; vnx::write(_out, last_modified);
	_out << ", \"is_directory\": "; vnx::write(_out, is_directory);
	_out << "}";
}

void file_info_t::read(std::istream& _in) {
	std::map<std::string, std::string> _object;
	vnx::read_object(_in, _object);
	for(const auto& _entry : _object) {
		if(_entry.first == "is_directory") {
			vnx::from_string(_entry.second, is_directory);
		} else if(_entry.first == "last_modified") {
			vnx::from_string(_entry.second, last_modified);
		} else if(_entry.first == "mime_type") {
			vnx::from_string(_entry.second, mime_type);
		} else if(_entry.first == "name") {
			vnx::from_string(_entry.second, name);
		} else if(_entry.first == "size") {
			vnx::from_string(_entry.second, size);
		}
	}
}

vnx::Object file_info_t::to_object() const {
	vnx::Object _object;
	_object["__type"] = "vnx.addons.file_info_t";
	_object["name"] = name;
	_object["mime_type"] = mime_type;
	_object["size"] = size;
	_object["last_modified"] = last_modified;
	_object["is_directory"] = is_directory;
	return _object;
}

void file_info_t::from_object(const vnx::Object& _object) {
	for(const auto& _entry : _object.field) {
		if(_entry.first == "is_directory") {
			_entry.second.to(is_directory);
		} else if(_entry.first == "last_modified") {
			_entry.second.to(last_modified);
		} else if(_entry.first == "mime_type") {
			_entry.second.to(mime_type);
		} else if(_entry.first == "name") {
			_entry.second.to(name);
		} else if(_entry.first == "size") {
			_entry.second.to(size);
		}
	}
}

vnx::Variant file_info_t::get_field(const std::string& _name) const {
	if(_name == "name") {
		return vnx::Variant(name);
	}
	if(_name == "mime_type") {
		return vnx::Variant(mime_type);
	}
	if(_name == "size") {
		return vnx::Variant(size);
	}
	if(_name == "last_modified") {
		return vnx::Variant(last_modified);
	}
	if(_name == "is_directory") {
		return vnx::Variant(is_directory);
	}
	return vnx::Variant();
}

void file_info_t::set_field(const std::string& _name, const vnx::Variant& _value) {
	if(_name == "name") {
		_value.to(name);
	} else if(_name == "mime_type") {
		_value.to(mime_type);
	} else if(_name == "size") {
		_value.to(size);
	} else if(_name == "last_modified") {
		_value.to(last_modified);
	} else if(_name == "is_directory") {
		_value.to(is_directory);
	} else {
		throw std::logic_error("no such field: '" + _name + "'");
	}
}

/// \private
std::ostream& operator<<(std::ostream& _out, const file_info_t& _value) {
	_value.write(_out);
	return _out;
}

/// \private
std::istream& operator>>(std::istream& _in, file_info_t& _value) {
	_value.read(_in);
	return _in;
}

const vnx::TypeCode* file_info_t::static_get_type_code() {
	const vnx::TypeCode* type_code = vnx::get_type_code(VNX_TYPE_HASH);
	if(!type_code) {
		type_code = vnx::register_type_code(static_create_type_code());
	}
	return type_code;
}

std::shared_ptr<vnx::TypeCode> file_info_t::static_create_type_code() {
	std::shared_ptr<vnx::TypeCode> type_code = std::make_shared<vnx::TypeCode>();
	type_code->name = "vnx.addons.file_info_t";
	type_code->type_hash = vnx::Hash64(0x7a1208fc71e8e919ull);
	type_code->code_hash = vnx::Hash64(0x2d88c73b68266d87ull);
	type_code->is_native = true;
	type_code->create_value = []() -> std::shared_ptr<vnx::Value> { return std::make_shared<vnx::Struct<file_info_t>>(); };
	type_code->fields.resize(5);
	{
		vnx::TypeField& field = type_code->fields[0];
		field.is_extended = true;
		field.name = "name";
		field.code = {32};
	}
	{
		vnx::TypeField& field = type_code->fields[1];
		field.is_extended = true;
		field.name = "mime_type";
		field.code = {32};
	}
	{
		vnx::TypeField& field = type_code->fields[2];
		field.name = "size";
		field.code = {8};
	}
	{
		vnx::TypeField& field = type_code->fields[3];
		field.name = "last_modified";
		field.code = {8};
	}
	{
		vnx::TypeField& field = type_code->fields[4];
		field.name = "is_directory";
		field.code = {31};
	}
	type_code->build();
	return type_code;
}


} // namespace vnx
} // namespace addons


namespace vnx {

void read(TypeInput& in, ::vnx::addons::file_info_t& value, const TypeCode* type_code, const uint16_t* code) {
	if(code) {
		switch(code[0]) {
			case CODE_OBJECT:
			case CODE_ALT_OBJECT: {
				Object tmp;
				vnx::read(in, tmp, type_code, code);
				value.from_object(tmp);
				return;
			}
			case CODE_DYNAMIC:
			case CODE_ALT_DYNAMIC:
				vnx::read_dynamic(in, value);
				return;
		}
	}
	if(!type_code) {
		vnx::skip(in, type_code, code);
		return;
	}
	if(code) {
		switch(code[0]) {
			case CODE_STRUCT: type_code = type_code->depends[code[1]]; break;
			case CODE_ALT_STRUCT: type_code = type_code->depends[vnx::flip_bytes(code[1])]; break;
			default: {
				vnx::skip(in, type_code, code);
				return;
			}
		}
	}
	const char* const _buf = in.read(type_code->total_field_size);
	if(type_code->is_matched) {
		{
			const vnx::TypeField* const _field = type_code->field_map[2];
			if(_field) {
				vnx::read_value(_buf + _field->offset, value.size, _field->code.data());
			}
		}
		{
			const vnx::TypeField* const _field = type_code->field_map[3];
			if(_field) {
				vnx::read_value(_buf + _field->offset, value.last_modified, _field->code.data());
			}
		}
		{
			const vnx::TypeField* const _field = type_code->field_map[4];
			if(_field) {
				vnx::read_value(_buf + _field->offset, value.is_directory, _field->code.data());
			}
		}
	}
	for(const vnx::TypeField* _field : type_code->ext_fields) {
		switch(_field->native_index) {
			case 0: vnx::read(in, value.name, type_code, _field->code.data()); break;
			case 1: vnx::read(in, value.mime_type, type_code, _field->code.data()); break;
			default: vnx::skip(in, type_code, _field->code.data());
		}
	}
}

void write(TypeOutput& out, const ::vnx::addons::file_info_t& value, const TypeCode* type_code, const uint16_t* code) {
	if(code && code[0] == CODE_OBJECT) {
		vnx::write(out, value.to_object(), nullptr, code);
		return;
	}
	if(!type_code || (code && code[0] == CODE_ANY)) {
		type_code = vnx::addons::vnx_native_type_code_file_info_t;
		out.write_type_code(type_code);
		vnx::write_class_header<::vnx::addons::file_info_t>(out);
	}
	if(code && code[0] == CODE_STRUCT) {
		type_code = type_code->depends[code[1]];
	}
	char* const _buf = out.write(17);
	vnx::write_value(_buf + 0, value.size);
	vnx::write_value(_buf + 8, value.last_modified);
	vnx::write_value(_buf + 16, value.is_directory);
	vnx::write(out, value.name, type_code, type_code->fields[0].code.data());
	vnx::write(out, value.mime_type, type_code, type_code->fields[1].code.data());
}

void read(std::istream& in, ::vnx::addons::file_info_t& value) {
	value.read(in);
}

void write(std::ostream& out, const ::vnx::addons::file_info_t& value) {
	value.write(out);
}

void accept(Visitor& visitor, const ::vnx::addons::file_info_t& value) {
	value.accept(visitor);
}

} // vnx