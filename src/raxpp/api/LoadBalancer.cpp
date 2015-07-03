#include "LoadBalancer.hpp"

#include "HTTPCodeHandler.hpp"
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

HTTPCodeHandler const check_deleteAccessListItems {
    {202}, // Good codes
    {{400, "Load Balancer Fault / Bad Request"},
     {500, "Load Balancer Fault"},
     {503, "Service Unavailable"},
     {401, "Unauthorized - Check authentication token"},
     {413, "Over Limit - Wait, and try later"},
     {404, "Load balancer / Access List not found - Check the name"}}};

void
LoadBalancer::deleteAccessListItems(Datacenter dc, int loadBalancerID,
                                    const std::vector<int> &idsToDelete) const {
  // http://docs.rackspace.com/loadbalancers/api/v1.0/clb-devguide/content/DELETE_bulkDeleteNetworks_v1.0__account__loadbalancers__loadBalancerId__accesslist_Access_Lists-d1e3160.html
  // Delete IDs in batches of 10
  constexpr int batchSize = 10;
  std::stringstream urlBase;
  urlBase << dc_to_url.at(dc) << "/loadbalancers/" << loadBalancerID
          << "/accesslist" 
          << "?networkItemId=";
  auto item = idsToDelete.cbegin();
  auto end = idsToDelete.cend();
  while (item != end) {
    std::stringstream url;
    url << urlBase.str();
    // We have to delete in batches of 10 or the API freaks out
    for (int i=0; i<batchSize; ++i)  {
      if (item == end)
        break;
      url << (*item) << ',';
    }
    int response = rs.del(url.str());
    if (!check_deleteAccessListItems.isGoodCode(response)) {
      std::stringstream msg;
      msg << "From sending this url: " << url.str();
      check_deleteAccessListItems(response, msg.str());
    } else {
      check_deleteAccessListItems(response);
    }
  }
}

}
}
