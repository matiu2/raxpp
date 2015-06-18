#pragma once

#include <map>

#include <raxpp/base/Datacenter.hpp>
#include <raxpp/api/Rackspace.hpp>

namespace raxpp {
namespace api {

class LoadBalancer {
private:
  Rackspace& rs;
  std::map<Datacenter, std::string> dc_to_url; // Convert a datacenter to the base url
public:
  LoadBalancer(Rackspace& rs);
  json::JList list(Datacenter dc) const;
};



}
}
