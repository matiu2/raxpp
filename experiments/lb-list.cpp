/// Creates a load balancer

#include <raxpp/api/Rackspace.hpp>
#include <raxpp/LoadBalancer.hpp>
#include <raxpp/LoadBalancers.hpp>

int main(int argc, char** argv) {
  curl::GlobalSentry s;
  if (argc < 4) {
    std::cerr << "USAGE: " << argv[0] << " username apikey datacenter"
              << std::endl << "Username: Your rackspace login username"
              << std::endl << "apikey: Your rackspace apikey" << std::endl
              << "datacenter: ORD/DFW/SYD/IAD/HKG (No UK support)" << std::endl
              << std::endl;
    return 1;
  }
  std::string username = argv[1];
  std::string apikey = argv[2];
  std::string dcName = argv[3];
  // Login to rackspace
  raxpp::api::Rackspace rs(username, apikey);
  raxpp::Datacenter dc = raxpp::name2dc(dcName);
  raxpp::LoadBalancers lbs(rs);
  for (const auto& lb : lbs.list(dc)) {
    using namespace std;
    cout << "Name: " << lb.model.name << endl << "ID: " << lb.model.id << endl
         << "Status: " << lb.model.status << endl
         << "Created: " << lb.model.created << endl
         << "Updated: " << lb.model.updated << endl << endl;
  }
  return 0;
}
