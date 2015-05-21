/// Provides help with using load balancers
#pragma once

#include <vector>
#include <map>

namespace raxpp {

struct VirtualIP {
  /// The ID for the IP address.
  int id;
  /// The IP address.
  std::string address;
  /// The IP address type. See the Virtual IP Types table in the Chapter 4
  /// section "Virtual IPs".
  std::string type;
  /// The IP version.
  std::string ipVersion;
  operator std::string& () { return address; }
  operator const std::string& () const { return address; }
};

struct LoadBalancer {
     /// Name of the load balancer to create. The name must be 128 characters or fewer in length, and all UTF-8 characters are valid. See http://www.utf8-chartable.de/ for information about the UTF-8 character set.
    std::string name;
    /// The ID for the load balancer.
    int id;
    /// Protocol of the service that is being load balanced.
    std::string protocol;
    /// Port number for the service you are load balancing.
    std::string port;
    /// Algorithm that defines how traffic should be directed between back-end nodes.
    std::string algorithm;
    /// The status of the load balancer.
    std::string status;
    /// The number of load balancer nodes.
    int nodeCount;
    /// The date and time what the load balancer was created.
    std::string created;
    /// The date and time what the load balancer was last updated.
    std::string updated;
    /// The list of virtualIps for a load balancer.
    std::vector<VirtualIP> virtualIps;
};

struct LoadBalancerService {
  std::map<std::string, std::string> dc_to_url;
  std::vector<LoadBalancer> lbs;
  std::map<int, LoadBalancer&> lbs_by_name;
  LoadBalancerService(Rackspace& rs) {
    using namespace json;
    const JList &endpoints = rs.getCatalog("cloudLoadBalancers").at("endpoints");
    for (const JMap& endpoint: endpoints) {
      dc_to_url.insert({endpoint.at("region"), endpoint.at("publicURL")});
    }
  }
};
  
}
