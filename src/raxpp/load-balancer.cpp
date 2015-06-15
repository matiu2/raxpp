#include "load-balancer.hpp"

#include <raxpp/json-conversion/LoadBalancer.hpp>

namespace raxpp {

using model::LoadBalancer;
using model::AccessList;
using model::AccessListItem;
using model::Datacenter;

LoadBalancerService::LoadBalancerService(Rackspace &rs) : rs(rs) {
  using namespace json;
  const JList &endpoints = rs.getCatalog("cloudLoadBalancers").at("endpoints");
  for (const JMap &endpoint : endpoints) {
    dc_to_url.insert(
        {model::dcVals.at(endpoint.at("region")), endpoint.at("publicURL")});
  }
}

const std::vector<LoadBalancer> &
LoadBalancerService::updateLoadBalancerList(Datacenter dc, bool forceRefresh) {
  // http://docs.rackspace.com/loadbalancers/api/v1.0/clb-devguide/content/GET_listLoadBalancers_v1.0__account__loadbalancers_load-balancers.html
  // Request: GET /loadbalancers
  // Fill in our results
  auto &destination = dc_to_lbs[dc];
  if (forceRefresh || (destination.size() == 0)) {
    // Do the request
    auto &url = dc_to_url.at(dc);
    auto lbs = rs.get(url + "/loadbalancers");
    for (auto &lb : (json::JList &)lbs.at("loadBalancers")) {
      destination.emplace_back(json_conversion::json2lb(lb, dc));
    }
  }
  return destination;
}

const std::vector<LoadBalancer> &LoadBalancerService::list(Datacenter dc,
                                                           bool forceRefresh) {
  const auto &destination = updateLoadBalancerList(dc, forceRefresh);
  return destination;
}

const LoadBalancer &LoadBalancerService::findByName(const std::string &name,
                                                    Datacenter dc,
                                                    bool forceRefresh) {
  const auto &destination = updateLoadBalancerList(dc, forceRefresh);
  for (const LoadBalancer &lb : destination)
    if (lb.name == name)
      return lb;
  std::stringstream msg;
  msg << "Load balancer with name " << name << " in datacenter " << model::dcNames.at(dc)
      << " couldn't be found";
  throw std::runtime_error(msg.str());
}

const LoadBalancer &LoadBalancerService::findById(int id, Datacenter dc,
                                                  bool forceRefresh) {
  const auto &destination = updateLoadBalancerList(dc, forceRefresh);
  for (const LoadBalancer &lb : destination)
    if (lb.id == id)
      return lb;
  std::stringstream msg;
  msg << "Load balancer with id " << id << " in datacenter " << model::dcNames.at(dc)
      << " couldn't be found";
  throw std::runtime_error(msg.str());
}

AccessList LoadBalancerService::getAccessList(const LoadBalancer& lb) {
  /* Result format:
      {
        "accessList": [
          {
            "type": "DENY",
            "id": 23,
            "address": "206.160.163.21"
          },
          ...
        ]
      }
  */
  AccessList result;
  auto &baseUrl = dc_to_url.at(lb.dc);
  std::stringstream url;
  url << baseUrl << "/loadbalancers/" << lb.id << "/accesslist";
  auto response = rs.get(url.str());
  json::JList& items = response.at("accessList");
  for (auto& item : items) {
    AccessListItem output;
    output.id = (int)item.at("id");
    output.address = item.at("address");
    std::string type = item.at("type");
    if (type == "DENY")
      output.type = AccessListItem::DENY;
    else if (type == "ALLOW")
      output.type = AccessListItem::ALLOW;
    else {
      std::stringstream msg;
      msg << "Unknown access list item type: " << type << " for AccessList Item ID " << output.id;
      throw std::runtime_error(msg.str());
    }
    result.emplace_back(std::move(output));
  }
  return result;
}

void LoadBalancerService::deleteAccessListItems(const LoadBalancer& lb, const std::vector<int> &itemsToDelete) {
  std::vector<int> batch;

}

}
