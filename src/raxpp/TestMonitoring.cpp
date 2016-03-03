#include <raxpp/Monitoring.hpp>

int main(int argc, char** argv)
{
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
  raxpp::Monitoring monitoring(rs);
  const auto values = monitoring.getMonitoringZones();
  using namespace std;
  for (const raxpp::model::monitoring::Zone &value : values)
    cout << "countryCode: " << value.countryCode << endl << "id: " << value.id
         << endl << "label: " << value.label << endl
         << "sourceIPRangeV4: " << value.sourceIPRangeV4 << endl
         << "sourceIPRangeV6: " << value.sourceIPRangeV6 << endl << endl;
  return 0;
}
