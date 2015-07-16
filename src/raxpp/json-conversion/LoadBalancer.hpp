#pragma once

#include <raxpp/base/Datacenter.hpp>
#include <raxpp/model/LoadBalancer.hpp>

#include <json_class.hpp>
#include <parse_to_json_class.hpp>

namespace raxpp {
namespace json_conversion {

/**
* @brief Takes a json reference and creates a load a balancer
*
* @param json json file input
*
* @return A new LoadBalancer object
*/
model::LoadBalancer json2lb(const json::JSON &json, Datacenter dc);

/// Converts a load balancer to json
json::JMap lb2json(const model::NewLoadBalancer& model);

model::AccessList json2accessList(const json::JMap &json);
json::JMap accessList2json(const model::AccessList& accessList);

json::JList newAccessList2json(const model::AccessList& accessList);


}
}
