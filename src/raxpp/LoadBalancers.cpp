#include "LoadBalancers.hpp"

#include <raxpp/json-conversion/LoadBalancer.hpp>

namespace raxpp {

std::vector<LoadBalancer> &
LoadBalancers::updateLoadBalancerList(Datacenter dc, bool forceRefresh) {
  // http://docs.rackspace.com/loadbalancers/api/v1.0/clb-devguide/content/GET_listLoadBalancers_v1.0__account__loadbalancers_load-balancers.html
  // Request: GET /loadbalancers
  // Fill in our results
  auto &destination = dc_to_lbs[dc];
  if (forceRefresh || (destination.size() == 0)) {
    // Do the request
    json::JList json = _api.list(dc);
    destination.clear();
    for (auto &lb : json) {
      destination.emplace_back(
          LoadBalancer(_api, json_conversion::json2lb(lb, dc)));
    }
  }
  return destination;
}

const std::vector<LoadBalancer> &LoadBalancers::list(Datacenter dc,
                                                           bool forceRefresh) {
  return updateLoadBalancerList(dc, forceRefresh);
}

LoadBalancer &LoadBalancers::findByName(const std::string &name, Datacenter dc,
                                        bool forceRefresh) {
  for (LoadBalancer &lb : updateLoadBalancerList(dc, forceRefresh))
    if (lb.model.name == name)
      return lb;
  std::stringstream msg;
  msg << "Load balancer with name " << name << " in datacenter "
      << dcNames.at(dc) << " couldn't be found";
  throw std::runtime_error(msg.str());
}

LoadBalancer &LoadBalancers::findById(int id, Datacenter dc,
                                      bool forceRefresh) {
  for (LoadBalancer &lb : updateLoadBalancerList(dc, forceRefresh))
    if (lb.model.id == id)
      return lb;
  std::stringstream msg;
  msg << "Load balancer with id " << id << " in datacenter "
      << dcNames.at(dc) << " couldn't be found";
  throw std::runtime_error(msg.str());
}

LoadBalancer& LoadBalancers:: create(const model::NewLoadBalancer& plan) {
  json::JMap upload;
  upload["loadBalancer"] = json_conversion::lb2json(plan);
  using namespace std;
  cout << upload << endl;
  json::JMap result = _api.create(plan.dc, upload);
  raxpp::model::LoadBalancer model = json_conversion::json2lb(result["loadBalancer"], plan.dc);
  auto& lbs = dc_to_lbs[plan.dc];
  lbs.push_back(LoadBalancer(_api, std::move(model)));
  return lbs.back();
}

}
