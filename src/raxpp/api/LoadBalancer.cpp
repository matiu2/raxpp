#include "LoadBalancer.hpp"

namespace raxpp {
namespace api {

LoadBalancer::LoadBalancer(Rackspace &rs) : rs(rs) {
  const json::JList &endpoints = rs.getCatalog("cloudLoadBalancers").at("endpoints");
  for (const json::JMap &endpoint : endpoints) {
    dc_to_url.insert(
        {name2dc(endpoint.at("region")), endpoint.at("publicURL")});
  }
}

json::JList LoadBalancer::list(Datacenter dc) const {
    auto &url = dc_to_url.at(dc);
    return rs.get(url + "/loadbalancers").at("loadBalancers");
}


}
}
