/// Provides help with using load balancers
#pragma once

#include <vector>
#include <map>

#include <raxpp/model/LoadBalancer.hpp>
#include <raxpp/base/Datacenter.hpp>
#include <raxpp/api/LoadBalancer.hpp>

namespace raxpp {

class AccessList {
private:
  const api::LoadBalancer& api;
public:
  AccessList(const api::LoadBalancer &api, raxpp::model::LoadBalancer &model);
  raxpp::model::LoadBalancer& model;
  /// Delete items from an access list (will be run in batches of 10)
  void deleteItems(const std::vector<int> &itemsToDelete);
};

class LoadBalancer {
private:
  const api::LoadBalancer& api;
  std::unique_ptr<AccessList> accessList;
public:
  LoadBalancer(const api::LoadBalancer& api, raxpp::model::LoadBalancer&& model) : api(api), model(std::move(model)) {}
  LoadBalancer(LoadBalancer&& other) = default;
  raxpp::model::LoadBalancer model;
  /// Returns the access list for a load balancer .. gets it straight form the API (no caching)
  AccessList &getAccessList(bool forceRefresh = false);
};


}
