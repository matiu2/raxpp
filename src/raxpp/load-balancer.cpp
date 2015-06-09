#include "load-balancer.hpp"

namespace raxpp {

LoadBalancerService::LoadBalancerService(Rackspace &rs) : rs(rs) {
  using namespace json;
  const JList &endpoints = rs.getCatalog("cloudLoadBalancers").at("endpoints");
  for (const JMap &endpoint : endpoints) {
    dc_to_url.insert(
        {dcVals.at(endpoint.at("region")), endpoint.at("publicURL")});
  }
}

/**
* @brief Takes a json reference and creates a load a balancer
*
* @param json json file input
*
* @return A new LoadBalancer object
*/
LoadBalancer json2lb(json::JSON &json, Datacenter dc) {
  LoadBalancer result;
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
    VirtualIP ip;
    ip.id = (int)vip.at("id");
    ip.address = vip.at("address");
    ip.type = vip.at("type");
    ip.ipVersion = vip.at("ipVersion");
    result.virtualIps.emplace_back(ip);
  }
  return std::move(result);
}

const std::vector<LoadBalancer>& LoadBalancerService::list(Datacenter dc, bool forceRefresh) {
  // http://docs.rackspace.com/loadbalancers/api/v1.0/clb-devguide/content/GET_listLoadBalancers_v1.0__account__loadbalancers_load-balancers.html
  // Request: GET /loadbalancers
  // Response: 
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
  // Do the request
  auto& url = dc_to_url.at(dc);
  auto lbs = rs.get(url + "/loadbalancers");
  // Fill in our results
  auto& destination = dc_to_lbs[dc];
  for (auto &lb : (json::JList &)lbs.at("loadBalancers")) {
    destination.emplace_back(json2lb(lb, dc));
  }
  return destination;
}

}
