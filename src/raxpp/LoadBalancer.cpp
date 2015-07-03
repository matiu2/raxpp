#include "LoadBalancer.hpp"

#include <raxpp/json-conversion/LoadBalancer.hpp>
#include <cassert>

namespace raxpp {

AccessList& LoadBalancer::getAccessList(bool forceRefresh) {
  if (!accessList || forceRefresh) {
    auto json = api.getAccessList(model.dc, model.id);
    model.setAccessList(json_conversion::json2accessList(json));
    accessList.reset(new AccessList(api, model));
  }
  return *accessList;
}

AccessList::AccessList(const api::LoadBalancer &api,
                       raxpp::model::LoadBalancer &model)
    : api(api), model(model) {
  // We can't do anything with an empty access list mate
  assert(model.accessList);
}

void AccessList::deleteItems(const std::vector<int> &itemsToDelete) {
  api.deleteAccessListItems(model.dc, model.id, itemsToDelete);
}
}
