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
  // Get the zones
  const auto zones = monitoring.getZones();
  using namespace std;
  for (const raxpp::model::monitoring::Zone &value : zones)
    cout << "countryCode: " << value.countryCode << endl << "id: " << value.id
         << endl << "label: " << value.label << endl
         << "sourceIPRangeV4: " << value.sourceIPRangeV4 << endl
         << "sourceIPRangeV6: " << value.sourceIPRangeV6 << endl << endl;
  // Get the entities
  const auto entities = monitoring.getEntities();
  using namespace std;
  for (const raxpp::model::monitoring::Entity &value : entities) {
    cout << "id: " << value.id << endl
         << "label: " << value.label << endl
         << "uri: " << value.uri << endl
         << "updated_at: " << value.updated_at << endl
         // TODO: We need craete the model objects for these
         // << "scheduled_suppressions: " << value.scheduled_suppressions << endl
         // << "active_suppressions: " << value.active_suppressions << endl
         << "agent_id: " << value.agent_id << endl
         << "created_at: " << value.created_at << endl
         << "IP Addresses: " << endl;
    for (auto const &row : value.ip_addresses)
      cout << "    " << row.first << " - " << row.second << endl;
    cout << "managed: " << value.managed << endl
         << "metadata: " << value.metadata << endl << endl;
  }
  return 0;
}
