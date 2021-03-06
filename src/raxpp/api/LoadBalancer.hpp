#pragma once

#include <map>
#include <vector>

#include <raxpp/base/Datacenter.hpp>
#include <raxpp/api/Rackspace.hpp>

namespace raxpp {
namespace api {

class LoadBalancer {
private:
  Rackspace& rs;
  std::map<Datacenter, std::string> dc_to_url; // Convert a datacenter to the base url
public:
  LoadBalancer(Rackspace& rs);
  /** URL: http://goo.gl/hKP4de
      Format: [{
                "name" : "lb-site1",
                "id" : 71,
                "protocol" : "HTTP",
                "port" : 80,
                "algorithm" : "RANDOM",
                "status" : "ACTIVE",
                "nodeCount" : 3,
                "virtualIps" : [{
                  "id" : 403,
                  "address" : "206.55.130.1",
                  "type" : "PUBLIC",
                  "ipVersion" : "IPV4"
                }],
                "created" : {"time" : "2010-11-30T03:23:42Z"},
                "updated" : {"time" : "2010-11-30T03:23:44Z"}
              }, ...]
  */
  json::JList list(Datacenter dc) const;
  /// URL: http://goo.gl/AvIQ9S
  /// Format: [{ "address": "206.160.163.21", "id": 23, "type": "DENY" }, ...]
  json::JMap getAccessList(Datacenter dc, int loadBalancerID) const;
  /// Delete Access List Items from a load balancer
  void deleteAccessListItems(Datacenter dc, int loadBalancerID,
                             const std::vector<int> &idsToDelete) const;
  void appendToAccessList(Datacenter dc, int loadBalancerID,
                          const json::JMap &accessList) const;
  json::JMap create(Datacenter dc, const json::JMap& data);
  /// Bulk delete nodes from the load balancer
  void bulkDeleteNodes(Datacenter dc, int loadBalancerID, std::vector<int> ids);
  /// Bulk delete load balancers
  void bulkDeleteLoadBalancers(Datacenter dc, std::vector<int> ids);
};



}
}
