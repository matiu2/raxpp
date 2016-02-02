# C++ Rackspace API client

# Dependencies:

 + jsonpp11 - for JSON processing
 + libcurl - (the C version) for http
 + openssl - for https

# Design philosophy

You'll log in (with username and apikey) and get an API object. The API object has no power, but just stores the JSON returned from the login.

Each service of the library will be in it's own includes, so if you just need to work with the CDN, you'd go #include <rax/cdn.hpp>

Once you've included the cdn, you'll be able to create a CDNService object, and pass it the reference to the API object. That object will have all the methods you need.

Usage example:

    #include <rax/api.hpp>
    #include <rax/server.hpp>

    using namespace rax;

    API api(username, apikey);
    ServerService servers(api);

    for (auto &server : servers)
        server.reboot();

    auto& web01 = servers.getByName("Web01");
    web01.createSnapshot("daily");

The idea is that you only need to include the files you need, to make your compilation faster.
