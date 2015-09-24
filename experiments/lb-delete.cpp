/// Creates a load balancer

#include <raxpp/api/Rackspace.hpp>
#include <raxpp/LoadBalancer.hpp>
#include <raxpp/LoadBalancers.hpp>

#include <string>
#include <iostream>

int main(int argc, char** argv) {
  curl::GlobalSentry s;
  if (argc < 5) {
    std::cerr << "USAGE: " << argv[0]
              << " username apikey datacenter load-balancer-name" << std::endl
              << "Username: Your rackspace login username" << std::endl
              << "apikey: Your rackspace apikey" << std::endl
              << "datacenter: ORD/DFW/SYD/IAD/HKG (No UK support)" << std::endl
              << "load-balancer-id: A space separated list of IDs to to delete"
              << std::endl << std::endl;
    return 1;
  }
  std::string username = argv[1];
  std::string apikey = argv[2];
  std::string dcName = argv[3];
  // Login to rackspace
  raxpp::api::Rackspace rs(username, apikey);
  raxpp::Datacenter dc = raxpp::name2dc(dcName);
  raxpp::LoadBalancers lbs(rs);
  using raxpp::model::NewLoadBalancer;
  std::vector<int> ids;
  for (int i = 4; i != argc; ++i)
    ids.push_back(std::stoi(argv[i]));
  lbs.bulk_delete(dc, ids);
  return 0;
}
