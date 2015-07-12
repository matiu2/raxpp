/// Contains models for load balancer related objects
#pragma once

#include <string>
#include <vector>
#include <memory>
#include <functional>

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

struct NewNode {
  enum Condition {ENABLED, DRAINING};
  enum Type { PRIMARY, SECONDARY, NOT_SET };

  std::string address;
  int port;
  Condition condition = ENABLED;
  Type type = NOT_SET;
  int weight;
};

struct Node : public NewNode {
  enum Status {ONLINE, DRAINING, OFFLINE};
  int id;
  Status status; // Read-only attribute
};

using AccessList = std::vector<AccessListItem>;

/// This is the data passed to the 'create load balancer' api call.
struct NewLoadBalancer {
  // (Required) Name of the load balancer to create. The name must be 128
  // characters or fewer in length, and all UTF-8 characters are valid. See
  // http://www.utf8-chartable.de/ for information about the UTF-8 character
  // set.
  std::string name;
  /// Which DC should the LB be created in ?
  Datacenter dc;
  // (Optional) Nodes to be added to the load balancer.
  std::vector<NewNode> nodes;
  // (Required) Protocol of the service that is being load balanced.
  std::string protocol;
  // (Optional) Enables or disables Half-Closed support for the load balancer.
  // Half-Closed support provides the ability for one end of the connection to
  // terminate its output, while still receiving data from the other end./ Only
  // available for TCP/TCP_CLIENT_FIRST protocols.
  enum Bool {True=1, False=0, Absent=-1};
  Bool halfClosed = Absent;
  // (Required) Type of virtualIp to add with the creation of a load balancer.
  // See the virtual IP types table in the Chapter 4 section "Virtual IPs".
  std::vector<VirtualIP> virtualIps;
  // The access list (firewall like thing) - This is gotten through a second
  // request, so an empty pointer means it hasn't been retrieved yet
  // (Optional) The access list management feature allows fine-grained network
  // access controls to be applied to the load balancer virtual IP address.
  // Refer to the Chapter 4 section "Access lists" for information and
  // examples.
  std::unique_ptr<AccessList> accessList;
  /// Ensures that an accessList exists, and returns a reference to it
  void setAccessList(model::AccessList &&input) {
    if (!accessList)
      accessList.reset(new AccessList(input));
  }
  // (Optional) Algorithm that defines how traffic should be directed between
  // back-end nodes.
  std::string algorithm;
  // (Optional) Current connection logging configuration. Refer to the Chapter 4
  // section "Log connections" for information and examples.
  std::string connectionLogging;
  // (Optional) Specifies limits on the number of connections per IP address to
  // help mitigate malicious or abusive traffic to your applications. Refer to
  // the Chapter 4 section "Throttle connections" for information and examples.
  std::string connectionThrottle;
  // (Optional) The type of health monitor check to perform to ensure that the
  // service is performing properly
  std::string healthMonitor;
  // (Optional) Information (metadata) that can be associated with each load
  // balancer.
  std::string metadata;
  // (Optional) Port number for the service you are load balancing.
  std::string port;
  // (Optional) The timeout value for the load balancer and communications with
  // its nodes. Defaults to 30 seconds with a maximum of 120 seconds.
  std::string timeout;
  // (Optional) Specifies whether multiple requests from clients are directed to
  // the same node.
  std::string sessionPersistence;
  // (Optional) Enables or disables HTTP to HTTPS redirection for the load
  // balancer. When enabled, any HTTP request returns status code 301 (Moved
  // Permanently), and the requester is redirected to the requested URL via the
  // HTTPS protocol on port 443./ For example, http://example.com/page.html
  // would be redirected to https://example.com/page.html. Only available for
  // HTTPS protocol (port=443), or HTTP protocol with a properly configured SSL
  // termination (secureTrafficOnly=true, securePort=443).
  bool httpsRedirect;
};

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
        accessList.reset(new AccessList(input));
    }
};

}
}
