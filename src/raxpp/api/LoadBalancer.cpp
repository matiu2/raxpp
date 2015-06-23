#include "LoadBalancer.hpp"

#include <sstream>

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


json::JList LoadBalancer::getAccessList(Datacenter dc, int loadBalancerID) const {
  // http://docs.rackspace.com/loadbalancers/api/v1.0/clb-devguide/content/GET_showAccessList_v1.0__account__loadbalancers__loadBalancerId__accesslist_Access_Lists-d1e3160.html
  std::stringstream url;
  url << dc_to_url.at(dc) << "/loadbalancers/" << loadBalancerID << "/accesslist";
  return rs.get(url.str()).at("accessList");
}


}
}
