#include "LoadBalancer.hpp"

#include <raxpp/json-conversion/LoadBalancer.hpp>
#include <cassert>

namespace raxpp {

model::AccessList& LoadBalancer::getAccessList(bool forceRefresh) {
  if (!model.accessList || forceRefresh) {
    auto json = api.getAccessList(model.dc, model.id);
    model.setAccessList(json_conversion::json2accessList(json));
  }
  return *model.accessList;
}

void LoadBalancer::deleteAccessListItems(const std::vector<int> &itemsToDelete) {
  api.deleteAccessListItems(model.dc, model.id, itemsToDelete);
}

void LoadBalancer::updateAccessList(const model::AccessList& accessList) {
  api.updateAccessList(model.dc, model.id,
                       json_conversion::accessList2json(accessList));
}

}
