#pragma once

#include <string>

#include <json_class.hpp>

namespace raxpp {

class Rackspace {
private:
  const json::JSON _json;
  std::string _token;
public:
  Rackspace(const std::string& reply);
  const std::string& token() const { return _token; }
  const json::JSON &JSON() const { return _json; }
  const json::JMap &getCatalog(const std::string &name) const;
};

/**
* @brief Log in to rackspace (US)
*
* @param username rackspace username
* @param apikey rackspace apikey
*
* @return The rackspace object that can be used to get all the other services
*/
Rackspace login(const std::string& username, const std::string& apikey);

}
