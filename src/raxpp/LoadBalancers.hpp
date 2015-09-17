#pragma once

#include "LoadBalancer.hpp"

#include <raxpp/api/LoadBalancer.hpp>

namespace raxpp {

// Stores model::LoadBalancer and does the initial retrieve of their data. This is a LoadBalancer factory.
class LoadBalancers {
private:
  raxpp::api::Rackspace& rs;
  raxpp::api::LoadBalancer _api;
  std::map<Datacenter, std::vector<LoadBalancer>>
      dc_to_lbs; // Store the load balancers by their home DC
  /// Updates a list of load balancers via the API
  std::vector<LoadBalancer> &updateLoadBalancerList(Datacenter dc,
                                                    bool forceRefresh);

public:
  LoadBalancers(api::Rackspace &rs) : rs(rs), _api(rs) {}
  /**
  * @brief List load balancers by datacenter
  *
  * @param DC Datacenter to list (one of SYD DFW ORD 
  * @param forceRefresh
  *
  * @return 
  */
  const std::vector<LoadBalancer>& list(Datacenter dc, bool forceRefresh = false);
  LoadBalancer& create(const model::NewLoadBalancer& model);
  LoadBalancer& findByName(const std::string& name, Datacenter dc, bool forceRefresh = false);
  LoadBalancer& findById(int id, Datacenter dc, bool forceRefresh = false);
  const raxpp::api::LoadBalancer& api() const { return _api; }
  void bulk_delete(Datacenter dc, std::vector<int> ids);
};

}
