#include "LoadBalancer.hpp"

namespace raxpp {
namespace json_conversion {

model::LoadBalancer json2lb(json::JSON &json, Datacenter dc) {
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
  model::LoadBalancer result;
  result.dc = dc;
  result.name = json.at("name");
  result.id = (int)json.at("id");
  result.protocol = json.at("protocol");
  result.port = (int)json.at("port");
  result.algorithm = json.at("algorithm");
  result.status = json.at("status");
  result.nodeCount = (int)json.at("nodeCount");
  result.created = json.at("created").at("time");
  result.updated = json.at("updated").at("time");
  for (json::JMap &vip : static_cast<json::JList &>(json.at("virtualIps"))) {
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

json::JList
virtualIPs2json(const std::vector<model::NewVirtualIP> &virtualIps) {
  json::JList result;
  result.reserve(virtualIps.size());
  using Type = model::VirtualIP::Type;
  using Version = model::VirtualIP::Version;
  for (const auto &IP : virtualIps) {
    result.emplace_back(json::JMap{
        {"address", IP.address},
        {"type", IP.type == Type::PUBLIC ? "PUBLIC" : "PRIVATE"},
        {"ipVersion", IP.ipVersion == Version::IPV6 ? "IPV6" : "IPV4"}});
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

json::JList
accessList2json(const std::vector<model::NewAccessListItem> &accessList) {
  json::JList result;
  result.reserve(accessList.size());
  for (auto &item : accessList) {
    result.emplace_back(json::JMap{
        {"address", item.address},
        {"type", item.type == model::NewAccessListItem::Type::DENY ? "DENY"
                                                                   : "ALLOW"}});
  }
  return result;
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
  json::JMap result = {{"loadBalancer", JMap{{"name", model.name}}}};
  if (!model.nodes.empty())
    result.insert({"nodes", newNodes2json(model.nodes)});
  if (!model.protocol.empty())
    result.insert({"protocol", model.protocol});
  using Bool = model::NewLoadBalancer::Bool;
  if (model.halfClosed != Bool::Absent)
    result.insert(
        {"halfClosed", model.halfClosed == Bool::True ? true : false});
  if (!model.virtualIps.empty())
    result.insert({"virtualIps", virtualIPs2json(model.virtualIps)});
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
  if (port)
    result.insert({"port", port});
  if (model.timeout)
    result.insert({"timeout", model.timeout});
  if (model.sessionPersistence)
    result.insert({"sessionPersistence", model.sessionPersistence});
  if (model.httpsRedirect)
    result.insert({"httpsRedirect", model.httpsRedirect});
  return result;
}

model::AccessList json2accessList(json::JList &json) {
  model::AccessList result;
  for (auto &item : json) {
    model::AccessListItem output;
    output.id = (int)item.at("id");
    output.address = item.at("address");
    std::string type = item.at("type");
    if (type == "DENY")
      output.type = model::AccessListItem::DENY;
    else if (type == "ALLOW")
      output.type = model::AccessListItem::ALLOW;
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
}}
