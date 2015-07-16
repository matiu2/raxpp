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
  auto currentList = &toBlock;
  for (int i = 5; i < argc; ++i) {
    std::string arg = argv[i];
    if (arg == "DENY")
      currentList = &toBlock;
    else if (arg == "ALLOW")
      currentList = &toOpen;
    else
      currentList->push_back(arg);
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
  for (auto& item : accessList)
    accessMap[item.address] = &item;
  std::vector<int> toDeleteFromList; // Things we need to delete from the list
  raxpp::model::AccessList
      toAppendToList; // Deny entries we need to append to the end
                      // of the accessList
  // Check the IPs we have to deny
  for (const std::string &ip : toBlock) {
    auto found = accessMap.find(ip);
    if (found != accessMap.end()) {
      // If the deny IP is already in the list,
      // and it's listed as allow,
      if (found->second->allow)
        // Remove it from the LB's list
        toDeleteFromList.push_back(found->second->id);
    } else {
      // If the deny IP is not already in the list .. add it as a blocker
      raxpp::model::AccessListItem newItem;
      newItem.address = ip;
      newItem.allow = false;
      toAppendToList.emplace_back(std::move(newItem));
    }
  }
  // Go through the list of IPs that need access
  for (const std::string &ip : toOpen) {
    // If it's in the list already
    auto found = accessMap.find(ip);
    if (found != accessMap.end()) {
      // And the list has it as DENY,
      if (!found->second->allow)
        // Remove it
        toDeleteFromList.push_back(found->second->id);
    }
  }
  // Now run through our actions
  lb.deleteAccessListItems(toDeleteFromList);
  // Add the items that need adding to the list
  lb.appendToAccessList(toAppendToList);
  // Now see what we got
  accessList = lb.getAccessList(true);
  for (auto& item : accessList) {
    std::cout << "ID: " << item.id << std::endl
    << "address: " << item.address << std::endl
    << "Type: ";
    std::cout << (item.allow ? "Allow" : "Deny");
    std::cout << std::endl << std::endl;
  }
  return 0;
}
