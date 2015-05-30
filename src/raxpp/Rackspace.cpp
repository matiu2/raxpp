#include "Rackspace.hpp"

#include <json_class.hpp>
#include <parse_to_json_class.hpp>

#include "utils.hpp"

namespace raxpp {

Rackspace::Rackspace(const std::string& reply) : _json(json::readValue(reply.begin(), reply.end())) {
  _token = _json.at("access").at("token").at("id");
}

const json::JMap& Rackspace::getCatalog(const std::string& name) const {
  const json::JList& catalog = _json.at("access").at("serviceCatalog");
  return findInList(catalog, "name", name);
}

}
