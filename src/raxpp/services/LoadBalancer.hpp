/// Provides help with using load balancers
#pragma once

#include <vector>
#include <map>

#include <raxpp/model/LoadBalancer.hpp>
#include <raxpp/base/Datacenter.hpp>
#include <raxpp/api/Rackspace.hpp>

namespace raxpp {
namespace services {

class LoadBalancer {
private:
  api::Rackspace& rs;
  std::map<Datacenter, std::vector<model::LoadBalancer>>
      dc_to_lbs; // Store the load balancers by their home DC
  /// Updates a list of load balancers via the API
  const std::vector<model::LoadBalancer>& updateLoadBalancerList(Datacenter dc, bool forceRefresh);
public:
  LoadBalancer(api::Rackspace& rs);
  /**
  * @brief List load balancers by datacenter
  *
  * @param DC Datacenter to list (one of SYD DFW ORD 
  * @param forceRefresh
  *
  * @return 
  */
  const std::vector<model::LoadBalancer>& list(Datacenter dc, bool forceRefresh = false);
  const model::LoadBalancer& findByName(const std::string& name, Datacenter dc, bool forceRefresh = false);
  const model::LoadBalancer& findById(int id, Datacenter dc, bool forceRefresh = false);
  /// Returns the access list for a load balancer .. gets it straight form the API (no caching)
  model::AccessList getAccessList(const model::LoadBalancer& lb);
  /// Delete items from an access list (will be run in batches of 10)
  void deleteAccessListItems(const model::LoadBalancer& lb, const std::vector<int> &itemsToDelete);


};
  
}
}
