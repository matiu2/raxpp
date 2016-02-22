#include <iostream>

#include <raxpp/api/Rackspace.hpp>
#include <raxpp/api/Monitoring.hpp>

int main(int argc, char** argv) {
  curl::GlobalSentry s;
  if (argc < 3) {
    std::cerr << "USAGE: " << argv[0] << " username apikey datacenter"
              << std::endl << "Username: Your rackspace login username"
              << std::endl << "apikey: Your rackspace apikey" << std::endl
              << std::endl;
    return 1;
  }
  std::string username = argv[1];
  std::string apikey = argv[2];
  // Login to rackspace
  raxpp::api::Rackspace rs(username, apikey);
  raxpp::api::Monitoring monitoring(rs);
  const auto output = monitoring.listMonitoringZones();
  auto& values = output.at("values");
  auto& metadata = output.at("metadata");
  using namespace std;
  cout << values << endl;
  cout << metadata << endl;
  return 0;
}
