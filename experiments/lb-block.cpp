/// Blocks and opens IP ranges in a load balancer

#include <raxpp/login.hpp>

#include <string>
#include <iostream>
#include <vector>

int main(int argc, char** argv) {
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
  std::string dc = argv[3];
  std::string lbName = argv[4];
  // Make lists of IPs to block/open
  std::vector<std::string> toBlock;
  std::vector<std::string> toOpen;
  auto &currentList = toBlock;
  for (int i = 5; i < argc; ++i) {
    const char *arg = argv[i];
    if (arg == "DENY")
      currentList = toBlock;
    else if (arg == "ALLOW")
      currentList = toOpen;
    else
      currentList.push_back(arg);
  }

  return 0;
}
