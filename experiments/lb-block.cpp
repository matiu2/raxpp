/// Blocks and opens IP ranges in a load balancer

#include <raxpp/api/Rackspace.hpp>
#include <raxpp/LoadBalancers.hpp>

#include <string>
#include <iostream>
#include <vector>

int main(int argc, char** argv) {
  curl::GlobalSentry s;
  if (argc < 5) {
    std::cerr << "USAGE: " << argv[0] << " username apikey datacenter load-balancer-name ([DENY/ALLOW] network ...)" << std::endl
    << "Username: Your rackspace login username" << std::endl
    << "apikey: Your rackspace apikey" << std::endl
    << "datacenter: ORD/DFW/SYD/IAD/HKG (No UK support)" << std::endl
    << "load-balancer-name: The name of the load balancer to apply the DENY / ALLOW" << std::endl
    << "DENY: all network specifications after this will be blocked (DENY is optional because it is the default" << std::endl
    << "ALLOW: all network specs after this will be un-blocked" << std::endl
    << "network: An IP address or a network block eg 1.2.3.4 or 192.168.1.0/24" << std::endl
    << std::endl
    << "the 'DENY net1 net2 ALLOW net3 DENY net4 net5 net6' can carry on for ages, remember that the LB can only hold 100 (or 500 addresses)" << std::endl << std::endl;
    return 1;
  }
  std::string username = argv[1];
  std::string apikey = argv[2];
  std::string dcName = argv[3];
  std::string lbName = argv[4];
  raxpp::Datacenter dc = raxpp::name2dc(dcName);
  // Make lists of IPs to block/open
  std::vector<std::string> toBlock;
  std::vector<std::string> toOpen;
  auto &currentList = toBlock;
  for (int i = 5; i < argc; ++i) {
    std::string arg = argv[i];
    if (arg == "DENY")
      currentList = toBlock;
    else if (arg == "ALLOW")
      currentList = toOpen;
    else
      currentList.push_back(arg);
  }
  // Login to rackspace
  raxpp::api::Rackspace rs(username, apikey);
  // Create the load balancer service
  raxpp::LoadBalancers loadBalancers(rs);
  // Get the load balancer we need
  auto& lb = loadBalancers.findByName(lbName, dc);
  // Get the existing access list
  auto& accessList = lb.getAccessList();
  std::map<std::string, raxpp::model::AccessListItem*> accessMap;
  for (auto& item : accessList.model)
    accessMap[item.address] = &item;
  std::vector<int> itemsToDelete; // Things we need to delete from the list
  std::vector<std::string> addressesToAdd; // Things we need to add to the list
  // Check the IPs we have to deny
  for (const std::string &ip : toBlock) {
    // If it's already in the list
    auto found = accessMap.find(ip);
    if (found != accessMap.end()) {
      // And the list has it as allow,
      if (found->second->type == raxpp::model::AccessListItem::ALLOW)
        // Remove it
        itemsToDelete.push_back(found->second->id);
    } else {
      // If it's not in the list, add it later
      addressesToAdd.push_back(ip);
    }
  }
  // Go through the list of IPs to allow
  for (const std::string &ip : toOpen) {
    // If it's in the list already
    auto found = accessMap.find(ip);
    if (found != accessMap.end()) {
      // And the list has it as DENY,
      if (found->second->type == raxpp::model::AccessListItem::DENY)
        // Remove it
        itemsToDelete.push_back(found->second->id);
    }
  }
  // Now run through our actions
  accessList.deleteItems(itemsToDelete);
  for (auto& item : accessList.model) {
    std::cout << "ID: " << item.id << std::endl
    << "address: " << item.address << std::endl
    << "Type: ";
    switch (item.type) {
      case raxpp::model::AccessListItem::DENY: {
        std::cout << "Deny";
        break;
      }
      case raxpp::model::AccessListItem::ALLOW: {
        std::cout << "Allow";
        break;
      }
    }
    std::cout << std::endl << std::endl;
  }
  // 
  return 0;
}
