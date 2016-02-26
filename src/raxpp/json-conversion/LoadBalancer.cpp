#include "LoadBalancer.hpp"

#include <parse_to_json_class.hpp>

namespace raxpp {
namespace json_conversion {

model::LoadBalancer json2lb(const json::JSON &json, Datacenter dc) {
  // Example from a listing:
  // { "loadBalancers":[
  //   {
  //      "name":"lb-site1",
  //      "id":71,
  //      "protocol":"HTTP",
  //       "port":80,
  //       "algorithm":"RANDOM",
  //       "status":"ACTIVE",
  //       "nodeCount":3,
  //       "virtualIps":[
  //       {
  //             "id":403,
  //             "address":"206.55.130.1",
  //             "type":"PUBLIC",
  //             "ipVersion":"IPV4"
  //       }
  //       ],
  //       "created":{ "time":"2010-11-30T03:23:42Z" },
  //       "updated":{ "time":"2010-11-30T03:23:44Z" }
  // },
  // Example from creating an LB:
  // {
  //   "loadBalancer": {
  //     "virtualIps": [
  //       {
  //         "type": "PUBLIC",
  //         "ipVersion": "IPV4",
  //         "id": 2099,
  //         "address": "119.9.56.59"
  //       },
  //       {
  //         "type": "PUBLIC",
  //         "ipVersion": "IPV6",
  //         "id": 9023010,
  //         "address": "2401:1801:7901:0000:9ff4:a092:0000:0004"
  //       }
  //     ],
  //     "updated": {
  //       "time": "2015-08-11T06:19:58Z"
  //     },
  //     "timeout": 30,
  //     "status": "BUILD",
  //     "sourceAddresses": {
  //       "ipv6Public": "2401:1801:7901::5/64",
  //       "ipv4Servicenet": "10.189.254.8",
  //       "ipv4Public": "119.9.8.8"
  //     },
  //     "protocol": "HTTP",
  //     "port": 80,
  //     "name": "matt.sherborne.test1",
  //     "algorithm": "LEAST_CONNECTIONS",
  //     "cluster": {
  //       "name": "ztm-n05.syd2.lbaas.rackspace.net"
  //     },
  //     "connectionLogging": {
  //       "enabled": false
  //     },
  //     "contentCaching": {
  //       "enabled": false
  //     },
  //     "created": {
  //       "time": "2015-08-11T06:19:58Z"
  //     },
  //     "halfClosed": false,
  //     "httpsRedirect": false,
  //     "id": 43991
  //   }
  // }
  model::LoadBalancer result;
  using namespace std;
  cout << "Result: " << json << endl;
  const json::JMap &jmap(json);
  result.dc = dc;
  auto getString = [&jmap](const std::string &key) -> std::string {
    auto found = jmap.find(key);
    return found == jmap.end() ? "" : found->second;
  };
  auto getInt = [&jmap](const std::string &key) -> int {
    auto found = jmap.find(key);
    return found == jmap.end() ? 0 : (int)found->second;
  };
  result.name = getString("name");
  result.id = getInt("id");
  result.protocol = getString("protocol");
  result.port = getInt("port");
  result.algorithm = getString("algorithm");
  result.status = getString("status");
  result.nodeCount = getInt("nodeCount");
  result.created = jmap.at("created").at("time");
  result.updated = jmap.at("updated").at("time");
  for (const json::JMap &vip :
       static_cast<const json::JList &>(jmap.at("virtualIps"))) {
    model::VirtualIP ip;
    ip.id = (int)vip.at("id");
    ip.address = vip.at("address");
    // IP Type
    const std::string &type = vip.at("type");
    using Type = model::VirtualIP::Type;
    if (type == "PUBLIC")
      ip.type = Type::PUBLIC;
    else if (type == "PRIVATE")
      ip.type = Type::PRIVATE;
    else
      throw std::runtime_error(
          std::string("Unknown Load Balancer Virtual IP type '") + type +
          "'. Expected PUBLIC or PRIVATE");
    // IP Version
    using IPVersion = model::VirtualIP::Version;
    ip.ipVersion = IPVersion::IPV4;
    auto version = vip.find("ipVersion");
    if ((version != vip.end()) && (version->second == "IPV6"))
      ip.ipVersion = IPVersion::IPV6;
    result.virtualIps.emplace_back(ip);
  }
  return std::move(result);
}

json::JList newNodes2json(const std::vector<model::NewNode> &nodes) {
  json::JList result;
  for (const auto &node : nodes) {
    json::JMap data{
        {"address", node.address},
        {"port", node.port},
        {"condition", node.condition == model::NewNode::Condition::ENABLED
                          ? "ENABLED"
                          : "DRAINING"},
        {"weight", node.weight}};
    using Type = model::NewNode::Type;
    switch (node.type) {
    case Type::NOT_SET:
      break;
    case Type::PRIMARY:
      data["type"] = "PRIMARY";
      break;
    case Type::SECONDARY:
      data["type"] = "SECONDARY";
      break;
    };
    result.emplace_back(std::move(data));
  }
  return std::move(result);
}

json::JMap accessList2json(const model::AccessList& accessList) {
  json::JList innards;
  for (const model::AccessListItem &item : accessList) {
    json::JMap jItem{{"address", item.address},
                     {"type", item.allow ? "ALLOW" : "DENY"}};
    // if item.id == 0, that means it was never set (yes, we're assuming that
    // the API never sets an actual item id to 0)
    if (item.id)
      jItem.insert({"id", item.id});
    innards.emplace_back(std::move(jItem));
  }
  return json::JMap{{"accessList", std::move(innards)}};
}


std::string algorithm2string(model::NewLoadBalancer::Algorithm algorithm) {
  using Algorithm = model::NewLoadBalancer::Algorithm;
  switch (algorithm) {
  case Algorithm::LEAST_CONNECTIONS:
    return "LEAST_CONNECTIONS";
  case Algorithm::RANDOM:
    return "RANDOM";
  case Algorithm::ROUND_ROBIN:
    return "ROUND_ROBIN";
  case Algorithm::WEIGHTED_LEAST_CONNECTIONS:
    return "WEIGHTED_LEAST_CONNECTIONS";
  case Algorithm::WEIGHTED_ROUND_ROBIN:
    return "WEIGHTED_ROUND_ROBIN";
  };
}

json::JMap lb2json(const model::NewLoadBalancer& model) {
  // http://docs.rackspace.com/loadbalancers/api/v1.0/clb-devguide/content/POST_createLoadBalancer_v1.0__account__loadbalancers_load-balancers.html#POST_createLoadBalancer_v1.0__account__loadbalancers_load-balancers-Request
  // { "loadBalancer": {
  //      "name": "a-new-loadbalancer",
  //      "port": 80,
  //      "protocol": "HTTP",
  //      "virtualIps": [
  //          {
  //              "type": "PUBLIC"
  //          }
  //      ],
  //      "nodes": [
  //          {
  //              "address": "10.1.1.1",
  //              "port": 80,
  //              "condition": "ENABLED"
  //          }
  //      ]
  //  }
  //}
  // Name	Type	Description
  // name String (Required) Name of the load balancer to create. The name must be 128 characters or fewer in length, and all UTF-8 characters are valid. See http://www.utf8-chartable.de/ for information about the UTF-8 character set.
  // nodes Object (Optional) Nodes to be added to the load balancer.  
  // protocol String (Required) Protocol of the service that is being load balanced.  
  // halfClosed Boolean (Optional) Enables or disables Half-Closed support for the load balancer. Half-Closed support provides the ability for one end of the connection to terminate its output, while still receiving data from the other end. Only available for TCP/TCP_CLIENT_FIRST protocols.  
  // virtualIps Object (Required) Type of virtualIp to add with the creation of a load balancer. See the virtual IP types table in the Chapter 4 section "Virtual IPs".  
  // accessList String (Optional) The access list management feature allows fine-grained network access controls to be applied to the load balancer virtual IP address. Refer to the Chapter 4 section "Access lists" for information and examples.  
  // algorithm String (Optional) Algorithm that defines how traffic should be directed between back-end nodes.  
  // connectionLogging String (Optional) Current connection logging configuration. Refer to the Chapter 4 section "Log connections" for information and examples.  
  // connectionThrottle String (Optional) Specifies limits on the number of connections per IP address to help mitigate malicious or abusive traffic to your applications. Refer to the Chapter 4 section "Throttle connections" for information and examples.  
  // healthMonitor String (Optional) The type of health monitor check to perform to ensure that the service is performing properly.  
  // metadata String (Optional) Information (metadata) that can be associated with each load balancer.  
  // port String (Optional) Port number for the service you are load balancing.  
  // timeout String (Optional) The timeout value for the load balancer and communications with its nodes. Defaults to 30 seconds with a maximum of 120 seconds.  
  // sessionPersistence String (Optional) Specifies whether multiple requests from clients are directed to the same node.  
  // httpsRedirect Boolean (Optional) Enables or disables HTTP to HTTPS redirection for the load balancer. When enabled, any HTTP request returns status code 301 (Moved Permanently), and the requester is redirected to the requested URL via the HTTPS protocol on port 443. For example, http://example.com/page.html would be redirected to https://example.com/page.html. Only available for HTTPS protocol (port=443), or HTTP protocol with a properly configured SSL termination (secureTrafficOnly=true, securePort=443).
  using namespace json;
  json::JMap result = {JMap{{"name", model.name}}};
  if (!model.nodes.empty())
    result.insert({"nodes", newNodes2json(model.nodes)});
  if (!model.protocol.empty())
    result.insert({"protocol", model.protocol});
  using Bool = model::NewLoadBalancer::Bool;
  if (model.halfClosed != Bool::Absent)
    result.insert(
        {"halfClosed", model.halfClosed == Bool::True ? true : false});
  if (!model.virtualIps.empty())
    result.insert({"virtualIps", newVirtualIPs2json(model.virtualIps)});
  if (!model.accessList.empty())
    result.insert({"accessList", accessList2json(model.accessList)});
  result.insert({"algorithm", algorithm2string(model.algorithm)});
  // Connection Logging
  using Logging = model::NewLoadBalancer::ConnectionLogging;
  if (model.connectionLogging != Logging::NONE)
    result.insert({"connectionLogging",
                   json::JMap{{"enabled", json::JBool(model.connectionLogging ==
                                                              Logging::Enabled
                                                          ? true
                                                          : false)}}});
  // Connection Throttling
  if (model.maxConnections > 0)
    result.insert(
        {"connectionThrottle",
         json::JMap{{"connectionThrottle",
                     json::JMap{{"maxConnections", model.maxConnections}}}}});
  // Health Monitor
  using Monitor = model::NewLoadBalancer::HealthMonitor;
  switch (model.healthMonitor) {
  case Monitor::NONE:
    break;
  case Monitor::CONNECT:
    result.insert({"healthMonitor", "CONNECT"});
    break;
  case Monitor::HTTP:
    result.insert({"healthMonitor", "HTTP"});
    break;
  case Monitor::HTTPS:
    result.insert({"healthMonitor", "HTTPS"});
    break;
  };
  // Metadata
  if (!model.metadata.empty()) {
    json::JMap metadata;
    for (const auto &pair : model.metadata)
      metadata.insert(pair);
    result.insert({"metadata", metadata});
  }
  if (model.port > 0)
    result.insert({"port", model.port});
  if (model.timeout > 0)
    result.insert({"timeout", model.timeout});
  using Persistence = model::NewLoadBalancer::SessionPersistence;
  switch (model.sessionPersistence) {
  case Persistence::NONE:
    break;
  case Persistence::HTTP_COOKIE:
    result.insert({"sessionPersistence", "HTTP_COOKIE"});
    break;
  case Persistence::SOURCE_IP:
    result.insert({"sessionPersistence", "SOURCE_IP"});
    break;
  };
  if (model.httpsRedirect)
    result.insert({"httpsRedirect", json::JBool(model.httpsRedirect)});
  return result;
}

model::AccessList json2accessList(const json::JMap &json) {
  model::AccessList result;
  const json::JList &accessList(json.at("accessList"));
  for (const auto &item : accessList) {
    model::AccessListItem output;
    output.id = (int)item.at("id");
    output.address = item.at("address");
    std::string type = item.at("type");
    if (type == "DENY")
      output.allow = false;
    else if (type == "ALLOW")
      output.allow = true;
    else {
      std::stringstream msg;
      msg << "Unknown access list item type: " << type
          << " for AccessList Item ID " << output.id;
      throw std::runtime_error(msg.str());
    }
    result.emplace_back(std::move(output));
  }
  return std::move(result);
}

json::JMap newVirtualIP2json(const model::NewVirtualIP &vip) {
  json::JMap result;
  if (!vip.address.empty())
    result["address"] = vip.address;
  using Type = model::NewVirtualIP::Type;
  switch (vip.type) {
  case Type::PUBLIC:
    result["type"] = "PUBLIC";
    break;
  case Type::PRIVATE:
    result["type"] = "PRIVATE";
    break;
  };
  if (vip.ipVersion == model::NewVirtualIP::Version::IPV6)
    result["version"] = "IPv6";
  return std::move(result);
}

json::JList newVirtualIPs2json(const std::vector<model::NewVirtualIP> &vips) {
  json::JList result;
  for (const auto &vip : vips)
    result.push_back(newVirtualIP2json(vip));
  return std::move(result);
}
}}
