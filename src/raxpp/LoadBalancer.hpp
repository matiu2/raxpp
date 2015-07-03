/// Provides help with using load balancers
#pragma once

#include <vector>
#include <map>

#include <raxpp/model/LoadBalancer.hpp>
#include <raxpp/base/Datacenter.hpp>
#include <raxpp/api/LoadBalancer.hpp>

namespace raxpp {

class LoadBalancer {
private:
  const api::LoadBalancer& api;
public:
  LoadBalancer(const api::LoadBalancer& api, raxpp::model::LoadBalancer&& model) : api(api), model(std::move(model)) {}
  LoadBalancer(LoadBalancer&& other) = default;
  raxpp::model::LoadBalancer model;
  /// Returns the access list for a load balancer .. gets it straight form the API (no caching)
  raxpp::model::AccessList &getAccessList(bool forceRefresh = false);
  void deleteAccessListItems(const std::vector<int> &itemsToDelete);
};


}
