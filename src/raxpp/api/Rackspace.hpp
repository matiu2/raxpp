#pragma once

#include <json_class.hpp>
#include <string>
#include <curlpp11.hpp>

namespace raxpp {
namespace api {

class Rackspace {
private:
  json::JSON _json;
  std::string _token;
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
  json::JSON get(const std::string& url);
  /// Perform an HTTP delete. Returns the http response code.
  int del(const std::string& url);
};

}
}
