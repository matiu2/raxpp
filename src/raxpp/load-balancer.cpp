#include "load-balancer.hpp"

namespace raxpp {

LoadBalancerService::LoadBalancerService(Rackspace &rs) : rs(rs) {
  using namespace json;
  const JList &endpoints = rs.getCatalog("cloudLoadBalancers").at("endpoints");
  for (const JMap &endpoint : endpoints) {
    dc_to_url.insert(
        {dcVals.at(endpoint.at("region")), endpoint.at("publicURL")});
  }
}

std::vector<LoadBalancer *> LoadBalancerService::list(Datacenter dc, bool forceRefresh) {

}

}
