/// Contains models for load balancer related objects
#pragma once

#include <string>
#include <vector>
#include <memory>

#include <raxpp/base/Datacenter.hpp>

namespace raxpp {
namespace model {

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

struct AccessListItem {
  enum Type {DENY, ALLOW};
  int id;
  std::string address;
  Type type;
};

using AccessList = std::vector<AccessListItem>;

struct LoadBalancer {
    /// Which DC is the LB from
    Datacenter dc;
    /// Name of the load balancer to create. The name must be 128 characters or fewer in length, and all UTF-8 characters are valid. See http://www.utf8-chartable.de/ for information about the UTF-8 character set.
    std::string name;
    /// The ID for the load balancer.
    int id;
    /// Protocol of the service that is being load balanced.
    std::string protocol;
    /// Port number for the service you are load balancing.
    int port;
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
    /// The access list (firewall like thing) - This is gotten through a second
    /// request, so an empty pointer means it hasn't been retrieved yet
    std::unique_ptr<AccessList> accessList;
    /// Ensures that an accessList exists, and returns a reference to it
    void setAccessList(model::AccessList&& input) {
      if (!accessList)
        accessList.reset(new AccessList);
      *accessList = input;
    }
};

}
}
