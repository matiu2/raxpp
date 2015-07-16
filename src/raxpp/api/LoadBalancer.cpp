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

HTTPCodeHandler const check_lb_read{
    {200}, // Good codes
    {
     {400, "Load Balancer Fault / Bad Request"},
     {500, "Load Balancer Fault"},
     {503, "Service Unavailable"},
     {401, "Unauthorized - Check authentication token"},
     {413, "Over Limit - Wait, and try later"},
    }};

json::JList LoadBalancer::list(Datacenter dc) const {
    auto &url = dc_to_url.at(dc);
    return rs.get(url + "/loadbalancers", check_lb_read).at("loadBalancers");
}

HTTPCodeHandler const check_lb_update {
    {202}, // Good codes
    {{400, "Load Balancer Fault / Bad Request"},
     {500, "Load Balancer Fault"},
     {503, "Service Unavailable"},
     {401, "Unauthorized - Check authentication token"},
     {413, "Over Limit - Wait, and try later"},
     {404, "Load balancer / Access List not found - Check the name"}}};

json::JMap LoadBalancer::getAccessList(Datacenter dc, int loadBalancerID) const {
  // http://docs.rackspace.com/loadbalancers/api/v1.0/clb-devguide/content/GET_showAccessList_v1.0__account__loadbalancers__loadBalancerId__accesslist_Access_Lists-d1e3160.html
  std::stringstream url;
  url << dc_to_url.at(dc) << "/loadbalancers/" << loadBalancerID << "/accesslist";
  return rs.get(url.str(), addContext(check_lb_read, [loadBalancerID](){
    std::stringstream msg;
    msg << "Getting AccessList for lb id: " << loadBalancerID;
    return msg.str();
  }));
}

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
      ++item;
      if (item == end)
        break;
    }
    rs.del(url.str(), addContext(check_lb_update, [&url]() {
      std::stringstream msg;
      msg << "From sending this url: " << url.str();
      return msg.str();
    }));
  }
}

void LoadBalancer::appendToAccessList(Datacenter dc, int loadBalancerID,
                                      const json::JMap &accessList) const {
  // http://docs.rackspace.com/loadbalancers/api/v1.0/clb-devguide/content/POST_createAccessList_v1.0__account__loadbalancers__loadBalancerId__accesslist_Access_Lists-d1e3160.html
  std::stringstream url;
  url << dc_to_url.at(dc) << "/loadbalancers/" << loadBalancerID
      << "/accesslist";
  std::cout << "Access List: " << accessList << std::endl;
  rs.POST(url.str(), accessList,
          addContext(check_lb_update,
                     [&url, loadBalancerID, &accessList]() {
            std::stringstream msg;
            msg << "From appending to accessList on load balancer "
                << loadBalancerID << " :" << accessList;
            return msg.str();
          }));
}

json::JMap LoadBalancer::create(Datacenter dc, const json::JMap& data) {
  // http://docs.rackspace.com/loadbalancers/api/v1.0/clb-devguide/content/POST_createLoadBalancer_v1.0__account__loadbalancers_load-balancers.html#POST_createLoadBalancer_v1.0__account__loadbalancers_load-balancers-Request
  return rs.POST("/loadbalancers", data, check_lb_update);
}

}
}
