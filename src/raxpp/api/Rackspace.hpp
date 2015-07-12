#pragma once

#include <json_class.hpp>
#include <string>
#include <curlpp11.hpp>
#include <functional>

namespace raxpp {
namespace api {

using CodeProcessor = std::function<void(int)>;

/// Default code processor does nothing
inline void defaultCodeProcessor(int code) {}

class Rackspace {
private:
  json::JSON _json;
  std::string _token;
  std::string _authHeader;
  curl::Easy client;
  bool haveSentAuth = false;
  std::string login(const std::string &username, const std::string &apikey);
public:
  Rackspace(const std::string& username, const std::string& apikey);
  const std::string& token() const { return _token; }
  const json::JSON &JSON() const { return _json; }
  const json::JMap &getCatalog(const std::string &name) const;
  /// Make a request with the auth headers already
  curl::Easy& request(const std::string& url);
  /// Perform a get to a URL, and get the reply in json format
  json::JSON get(const std::string& url, CodeProcessor responseHandler);
  /// Perform an HTTP delete. Returns the http response code.
  void del(const std::string& url, CodeProcessor responseHandler);
  /// Perform an HTTP POST
  json::JSON POST(const std::string& url, const json::JSON& data, CodeProcessor responseHandler);
};


}
}
