#pragma once

#include <map>

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
  json::JList getAccessList(Datacenter dc, int loadBalancerID) const;
  /// Delete Access List Items from a load balancer
  void deleteAccessListItems(Datacenter dc, int loadBalancerID,
                             int accessListID,
                             const std::vector<int> &idsToDelete) const;
};



}
}
