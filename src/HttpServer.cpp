/*
 * HttpServer.cpp
 *
 *  Created on: Jul 8, 2020
 *      Author: mad
 */

#include <vnx/addons/HttpServer.h>
#include <vnx/addons/HttpRequest.hxx>
#include <vnx/addons/HttpResponse.hxx>
#include <vnx/addons/HttpComponentClient.hxx>
#include <vnx/addons/HttpComponentAsyncClient.hxx>
#include <vnx/OverflowException.hxx>
#include <vnx/PermissionDenied.hxx>
#include <vnx/vnx.h>

#include <atomic>

#include <url.h>
#include <microhttpd.h>

#if MHD_VERSION < 0x00097002
typedef int MHD_Result;
#endif


namespace vnx {
namespace addons {

class HttpServer : public HttpServerBase {
public:
	HttpServer(const std::string& _vnx_name);

protected:
	struct request_state_t {
		MHD_Connection* connection = nullptr;
		std::shared_ptr<HttpRequest> request;
		std::shared_ptr<const HttpResponse> response;
		std::shared_ptr<HttpComponentClient> httpclient;
		std::string sub_path;
	};

	void init() override;

	void main() override;

	void http_request_async(std::shared_ptr<const HttpRequest> request,
							const std::string& sub_path,
							const vnx::request_id_t& request_id) const override;

	void http_request_chunk_async(std::shared_ptr<const HttpRequest> request,
							const std::string& sub_path,
							const int64_t& offset,
							const int64_t& max_bytes,
							const vnx::request_id_t& _request_id) const override;

private:
	void process(request_state_t* state);

	void reply(	request_state_t* state,
				std::shared_ptr<const HttpResponse> result);

	void reply_error(	request_state_t* state,
						const vnx::exception& ex);

	std::shared_ptr<HttpSession> create_session() const;

	void add_session(std::shared_ptr<HttpSession> session) const;

	std::string get_session_cookie(std::shared_ptr<const HttpSession> session) const;

	static MHD_Result
	access_handler_callback(void* cls,
							MHD_Connection* connection,
							const char* url,
							const char* method,
							const char* version,
							const char* upload_data,
							size_t* upload_data_size,
							void** con_cls);

	static MHD_Result
	http_header_callback(void* cls, MHD_ValueKind kind, const char* key, const char* value);

	static MHD_Result
	cookie_callback(void* cls, MHD_ValueKind kind, const char* key, const char* value);

	static MHD_Result
	query_params_callback(void* cls, MHD_ValueKind kind, const char* key, const char* value);

	static void
	request_completed_callback(	void* cls,
								MHD_Connection* connection,
								void** con_cls,
								MHD_RequestTerminationCode term_code);

	static ssize_t chunked_transfer_callback(void *userdata, uint64_t offset, char *dest, size_t length);

private:
	struct http_clients_t {
		std::shared_ptr<HttpComponentClient> sync_client;
		std::shared_ptr<HttpComponentAsyncClient> async_client;
	};
	MHD_Daemon* m_daemon = nullptr;
	std::atomic<uint64_t> m_next_id {1};

	std::map<std::string, http_clients_t> m_client_map;									// [url path => clients]
	std::shared_ptr<const HttpSession> m_default_session;

	mutable std::unordered_map<std::string, std::shared_ptr<const HttpSession>> m_session_map;	// [http session id => session]
	mutable std::multimap<int64_t, std::string> m_session_timeout_queue;						// [deadline => http session id]

};


HttpServer::HttpServer(const std::string& _vnx_name)
	:	HttpServerBase(_vnx_name)
{
	vnx_clean_exit = true;		// process remaining requests on exit
}

void HttpServer::init()
{
	vnx::open_pipe(vnx_name, this, 500);
}

void HttpServer::main()
{
	if(show_info) {
		show_warnings = true;
	}
	{
		auto session = HttpSession::create();
		session->vnx_session_id = vnx::get_auth_server()->login_anonymous(default_access)->id;	// TODO: use default_group here
		m_default_session = session;
	}
	m_daemon = MHD_start_daemon(
			MHD_USE_SELECT_INTERNALLY | MHD_USE_SUSPEND_RESUME | (use_epoll ? MHD_USE_EPOLL_LINUX_ONLY : MHD_USE_POLL),
			port, NULL, NULL,
			&HttpServer::access_handler_callback, this,
			MHD_OPTION_NOTIFY_COMPLETED, &HttpServer::request_completed_callback, this,
			MHD_OPTION_END);

	if(!m_daemon) {
		log(ERROR) << "Failed to start MHD daemon!";
		return;
	}
	log(INFO) << "Running on port " << port;

	for(const auto& entry : components)
	{
		const auto& path = entry.first;
		const auto& module = entry.second;
		if(path.empty() || path.back() != '/') {
			log(ERROR) << "Path needs to end with a '/': '" << path << "'";
			continue;
		}
		const bool is_ourself = module == vnx_name;
		{
			auto async_client = std::make_shared<HttpComponentAsyncClient>(module);
			async_client->vnx_set_non_blocking(is_ourself ? true : non_blocking);
			add_async_client(async_client);
			m_client_map[path].async_client = async_client;
		}
		if(!is_ourself) {
			auto sync_client = std::make_shared<HttpComponentClient>(module);
			m_client_map[path].sync_client = sync_client;
		}
		log(INFO) << "Got component '" << module << "' for path '" << path << "'";
	}

	Super::main();

	if(m_daemon) {
		MHD_stop_daemon(m_daemon);
	}
}

void HttpServer::http_request_async(std::shared_ptr<const HttpRequest> request,
									const std::string& sub_path,
									const vnx::request_id_t& request_id) const
{
	if(sub_path == login_path)
	{
		auto session = create_session();

		auto user = request->query_params.find("user");
		if(user != request->query_params.end())
		{
			auto passwd_hex = request->query_params.find("passwd_hex");
			auto passwd_plain = request->query_params.find("passwd_plain");

			if(passwd_hex != request->query_params.end())
			{
				throw std::logic_error("not implemented yet");
			}
			else if(passwd_plain != request->query_params.end())
			{
				auto vnx_session = vnx::get_auth_server()->login(user->second, passwd_plain->second);
				if(!vnx_session || vnx_session->user != user->second) {
					// login failed
					http_request_async_return(request_id, HttpResponse::from_status(403));
					return;
				}
				session->user = user->second;
				session->vnx_session_id = vnx_session->id;
			}
			else {
				// no password = no bueno
				http_request_async_return(request_id, HttpResponse::from_status(403));
				return;
			}
		} else {
			// anonymous login
			session->vnx_session_id = m_default_session->vnx_session_id;
		}
		add_session(session);

		auto response = HttpResponse::create();
		response->status = 200;
		response->headers.emplace_back("Set-Cookie", get_session_cookie(session));

		auto redirect = request->query_params.find("redirect");
		if(redirect != request->query_params.end()) {
			response->status = 303;
			response->headers.emplace_back("Location", redirect->second);
		}
		http_request_async_return(request_id, response);
	}
	else if(sub_path == logout_path)
	{
		// TODO
	}
	else {
		throw std::logic_error("invalid request");
	}
}

void HttpServer::http_request_chunk_async(	std::shared_ptr<const HttpRequest> request,
											const std::string& sub_path,
											const int64_t& offset,
											const int64_t& max_bytes,
											const vnx::request_id_t& _request_id) const
{
	throw std::logic_error("invalid request");
}

void HttpServer::process(request_state_t* state)
{
	auto request = state->request;
	request->session = m_default_session;
	{
		auto cookie = request->cookies.find(session_coookie_name);
		if(cookie != request->cookies.end()) {
			auto iter = m_session_map.find(cookie->second);
			if(iter != m_session_map.end()) {
				request->session = iter->second;
			}
		}
	}
	if(output_request) {
		publish(request, output_request);
	}
	if(request->method == "OPTIONS")
	{
		auto result = HttpResponse::create();
		result->status = 204;
		result->headers.emplace_back("Allow", "OPTIONS, GET, HEAD, POST, PUT, DELETE");
		result->headers.emplace_back("Access-Control-Allow-Methods", "DELETE, PUT, POST, GET, OPTIONS");
		result->headers.emplace_back("Access-Control-Allow-Headers", "Content-Type");
		result->headers.emplace_back("Access-Control-Max-Age", "86400");
		reply(state, result);
		return;
	}
	std::string prefix;
	std::string sub_path;
	size_t best_match_length = 0;
	http_clients_t clients;
	for(const auto& entry : m_client_map)
	{
		const auto entry_size = entry.first.size();
		if(request->path.size() >= entry_size
			&& entry_size > best_match_length
			&& request->path.substr(0, entry_size) == entry.first)
		{
			prefix = entry.first;
			sub_path = "/" + request->path.substr(entry_size);
			best_match_length = entry_size;
			clients = entry.second;
		}
	}
	if(clients.async_client) {
		if(show_info) {
			log(INFO) << request->method << " '" << request->path
					<< "' => '" << components[prefix] << sub_path << "'";
		}
		state->sub_path = sub_path;
		state->httpclient = clients.sync_client;
		clients.async_client->http_request(request, sub_path,
				std::bind(&HttpServer::reply, this, state, std::placeholders::_1),
				std::bind(&HttpServer::reply_error, this, state, std::placeholders::_1));
	} else {
		reply(state, HttpResponse::from_status(404));
	}
}

void HttpServer::reply(	request_state_t* state,
						std::shared_ptr<const HttpResponse> result)
{
	auto request = state->request;
	if(show_warnings && result->status >= 400) {
		log(WARN) << request->method << " '" << request->path << "' failed with: HTTP " << result->status << " (" << result->error_text << ")";
	}
	publish(result, output_response);
	state->response = result;

	MHD_Response *response;
	if(result->is_chunked){
		response = MHD_create_response_from_callback(result->chunked_total_size, chunk_size, chunked_transfer_callback, state, NULL);
	}else{
		response = MHD_create_response_from_buffer(result->payload.size(), (void*)result->payload.data(), MHD_RESPMEM_PERSISTENT);
	}
	MHD_add_response_header(response, "Server", "vnx.addons.HttpServer");
	if(!access_control_allow_origin.empty()) {
		MHD_add_response_header(response, "Access-Control-Allow-Origin", access_control_allow_origin.c_str());
	}
	if(!result->content_type.empty()) {
		MHD_add_response_header(response, "Content-Type", result->content_type.c_str());
	}
	for(const auto& entry : result->headers) {
		MHD_add_response_header(response, entry.first.c_str(), entry.second.c_str());
	}
	if(auto_session && request->session == m_default_session)
	{
		auto session = create_session();
		session->vnx_session_id = m_default_session->vnx_session_id;
		add_session(session);
		MHD_add_response_header(response, "Set-Cookie", get_session_cookie(session).c_str());
	}
	const auto ret = MHD_queue_response(state->connection, result->status, response);
	if(ret != MHD_YES) {
		log(WARN) << "Failed to queue MHD response!";
	}
	MHD_resume_connection(state->connection);	// after this, 'state' may be deleted at any time
	MHD_destroy_response(response);
}

void HttpServer::reply_error(	request_state_t* state,
								const vnx::exception& ex)
{
	auto response = HttpResponse::create();
	if(std::dynamic_pointer_cast<const OverflowException>(ex.value())) {
		response->status = 503;
	} else if(std::dynamic_pointer_cast<const PermissionDenied>(ex.value())) {
		response->status = 403;
	} else {
		response->status = 500;
	}
	response->error_text = ex.what();
	reply(state, response);
}

std::shared_ptr<HttpSession> HttpServer::create_session() const
{
	auto session = HttpSession::create();
	session->login_time = vnx::get_time_seconds();
	for(int i = 0; i < session_size; ++i) {
		if(i) { session->http_session_id += "-"; }
		session->http_session_id += vnx::to_hex_string(vnx::rand64());
	}
	return session;
}

void HttpServer::add_session(std::shared_ptr<HttpSession> session) const
{
	if(!m_session_map.count(session->http_session_id)) {
		m_session_timeout_queue.emplace(session->login_time + session_expire, session->http_session_id);
	}
	m_session_map[session->http_session_id] = session;
}

std::string HttpServer::get_session_cookie(std::shared_ptr<const HttpSession> session) const
{
	return session_coookie_name + "=" + session->http_session_id + "; SameSite=" + cookie_policy;
}

MHD_Result HttpServer::access_handler_callback(	void* cls,
												MHD_Connection* connection,
												const char* url,
												const char* method,
												const char* version,
												const char* upload_data,
												size_t* upload_data_size,
												void** con_cls)
{
	HttpServer* self = (HttpServer*)cls;
	request_state_t* state = (request_state_t*)(*con_cls);
	if(!state) {
		state = new request_state_t();
		state->connection = connection;
		auto request = HttpRequest::create();
		request->id = self->m_next_id++;
		request->url = std::string(url);
		request->method = std::string(method);
		try {
			// convert relative paths to absolute
			request->path = Url::Url(request->url).abspath().path();
		} catch(...) {
			return MHD_NO;
		}
		state->request = request;
		*con_cls = state;
		return MHD_YES;
	}
	const size_t length = *upload_data_size;

	if(length) {
		auto& payload = state->request->payload;
		const size_t offset = payload.size();
		if(self->max_payload_size >= 0 && offset + length > self->max_payload_size) {
			return MHD_NO;
		}
		payload.resize(offset + length);
		::memcpy(payload.data(offset), upload_data, length);
		*upload_data_size = 0;
	} else {
		MHD_get_connection_values(connection, MHD_HEADER_KIND, &HttpServer::http_header_callback, state);
		MHD_get_connection_values(connection, MHD_COOKIE_KIND, &HttpServer::cookie_callback, state);
		MHD_get_connection_values(connection, MHD_ValueKind(MHD_POSTDATA_KIND | MHD_GET_ARGUMENT_KIND), &HttpServer::query_params_callback, state);

		if(self->add_task(std::bind(&HttpServer::process, self, state))) {
			MHD_suspend_connection(connection);
		} else {
			return MHD_NO;
		}
	}
	return MHD_YES;
}

MHD_Result HttpServer::http_header_callback(void* cls, MHD_ValueKind kind, const char* key, const char* value)
{
	request_state_t* state = (request_state_t*)cls;
	auto request = state->request;
	if(key && value) {
		const auto key_ = ascii_tolower(std::string(key));
		if(key_ == "content-type") {
			request->content_type = ascii_tolower(std::string(value));
		}
		request->headers.emplace_back(key_, std::string(value));
	}
	return MHD_YES;
}

MHD_Result HttpServer::cookie_callback(void* cls, MHD_ValueKind kind, const char* key, const char* value)
{
	request_state_t* state = (request_state_t*)cls;
	if(key && value) {
		state->request->cookies.emplace(std::string(key), std::string(value));
	}
	return MHD_YES;
}

MHD_Result HttpServer::query_params_callback(void* cls, MHD_ValueKind kind, const char* key, const char* value)
{
	request_state_t* state = (request_state_t*)cls;
	if(key && value) {
		state->request->query_params[std::string(key)] = std::string(value);
	}
	return MHD_YES;
}

void HttpServer::request_completed_callback(void* cls,
											MHD_Connection* connection,
											void** con_cls,
											MHD_RequestTerminationCode term_code)
{
	request_state_t* state = (request_state_t*)(*con_cls);
	if(state) {
		delete state;
	}
}

ssize_t HttpServer::chunked_transfer_callback(void *userdata, uint64_t offset, char *dest, size_t length)
{
	request_state_t *state = (request_state_t *)userdata;
	if(!state->httpclient) {
		return MHD_CONTENT_READER_END_OF_STREAM;
	}
	std::shared_ptr<const HttpResponse> response = state->httpclient->http_request_chunk(state->request, state->sub_path, offset, length);
	const size_t size = response->payload.size();
	if(size == 0){
		return MHD_CONTENT_READER_END_OF_STREAM;
	}
	::memcpy(dest, response->payload.data(), size);
	return size;
}

HttpServerBase* new_HttpServer(const std::string& name) {
	return new HttpServer(name);
}


} // addons
} // vnx
