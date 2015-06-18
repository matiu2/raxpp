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
    ip.type = vip.at("type");
    ip.ipVersion = vip.at("ipVersion");
    result.virtualIps.emplace_back(ip);
  }
  return std::move(result);
}

}}
