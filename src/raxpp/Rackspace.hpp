#pragma once

#include <json_class.hpp>
#include <string>

namespace raxpp {

class Rackspace {
private:
  json::JSON _json;
  std::string _token;
public:
  Rackspace(const std::string& username, const std::string& apikey);
  const std::string& token() const { return _token; }
  const json::JSON &JSON() const { return _json; }
  const json::JMap &getCatalog(const std::string &name) const;
};

}
