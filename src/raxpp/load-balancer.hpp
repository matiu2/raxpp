/// Provides help with using load balancers
#pragma once

#include <vector>
#include <map>

#include "model/Datacenter.hpp"
#include "model/LoadBalancer.hpp"
#include <raxpp/Rackspace.hpp>

namespace raxpp {

class LoadBalancerService {
private:
  Rackspace& rs;
  std::map<model::Datacenter, std::string> dc_to_url; // Convert a datacenter to the base url
  std::map<model::Datacenter, std::vector<model::LoadBalancer>>
      dc_to_lbs; // Store the load balancers by their home DC
  /// Updates a list of load balancers via the API
  const std::vector<model::LoadBalancer>& updateLoadBalancerList(model::Datacenter dc, bool forceRefresh);
public:
  LoadBalancerService(Rackspace& rs);
  /**
  * @brief List load balancers by datacenter
  *
  * @param DC Datacenter to list (one of SYD DFW ORD 
  * @param forceRefresh
  *
  * @return 
  */
  const std::vector<model::LoadBalancer>& list(model::Datacenter dc, bool forceRefresh = false);
  const model::LoadBalancer& findByName(const std::string& name, model::Datacenter dc, bool forceRefresh = false);
  const model::LoadBalancer& findById(int id, model::Datacenter dc, bool forceRefresh = false);
  /// Returns the access list for a load balancer .. gets it straight form the API (no caching)
  model::AccessList getAccessList(const model::LoadBalancer& lb);
  /// Delete items from an access list (will be run in batches of 10)
  void deleteAccessListItems(const model::LoadBalancer& lb, const std::vector<int> &itemsToDelete);


};
  
}
