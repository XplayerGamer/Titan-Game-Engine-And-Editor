#include "../include/NetworkingUDP.hpp"
#include <iostream>
#include <thread>

using namespace Titan;

int main_server() {
    UDPServer server;
    if (!server.Start(30000, 20)) {
        std::cerr << "Failed to start server" << std::endl;
        return 1;
    }

    // Add a dummy client entry (loopback)
    int cid = server.AddClient("127.0.0.1", 30001);
    std::cout << "Server started, client id=" << cid << std::endl;

    // Simulate one player snapshot updating
    Snapshot s;
    s.tick = 0; s.x = 0; s.y = 0; s.z = 0; s.vx = 0; s.vy = 0; s.vz = 0; s.health = 100;

    for (int i = 0; i < 500; ++i) {
        s.x += 0.01f; s.tick++;
        server.PushSnapshot(0, s);
        server.Update();
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }

    server.Stop();
    return 0;
}

int main_client() {
    UDPClient client;
    if (!client.Start("127.0.0.1", 30000, 30001)) {
        std::cerr << "Failed to start client" << std::endl;
        return 1;
    }

    client.OnSnapshot = [](const Snapshot& s) {
        std::cout << "Snapshot tick=" << s.tick << " pos=(" << s.x << "," << s.y << "," << s.z << ") health=" << int(s.health) << std::endl;
    };

    for (int i = 0; i < 500; ++i) {
        client.Update();
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }

    client.Stop();
    return 0;
}

int main(int argc, char** argv) {
    if (argc > 1 && std::string(argv[1]) == "server") return main_server();
    return main_client();
}
