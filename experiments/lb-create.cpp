/// Creates a load balancer

#include <raxpp/api/Rackspace.hpp>
#include <raxpp/LoadBalancer.hpp>
#include <raxpp/LoadBalancers.hpp>

int main(int argc, char** argv) {
  curl::GlobalSentry s;
  if (argc < 5) {
    std::cerr << "USAGE: " << argv[0]
              << " username apikey datacenter load-balancer-name ([DENY/ALLOW] "
                 "network ...)" << std::endl
              << "Username: Your rackspace login username" << std::endl
              << "apikey: Your rackspace apikey" << std::endl
              << "datacenter: ORD/DFW/SYD/IAD/HKG (No UK support)" << std::endl
              << "load-balancer-name: The name of the load balancer to apply "
                 "the DENY / ALLOW" << std::endl << std::endl;
    return 1;
  }
  std::string username = argv[1];
  std::string apikey = argv[2];
  std::string dcName = argv[3];
  std::string lbName = argv[4];
  // Login to rackspace
  raxpp::api::Rackspace rs(username, apikey);
  raxpp::Datacenter dc = raxpp::name2dc(dcName);
  raxpp::LoadBalancers lbs(rs);
  using raxpp::model::NewLoadBalancer;
  NewLoadBalancer parameters;
  parameters.name = lbName;
  parameters.dc = dc;
  parameters.protocol = "HTTP";
  parameters.port = 80;
  lbs.create(parameters);
  return 0;
}
