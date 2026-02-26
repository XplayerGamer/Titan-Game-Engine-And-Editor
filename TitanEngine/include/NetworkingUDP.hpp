#pragma once

#include <cstdint>
#include <vector>
#include <string>
#include <functional>
#include <memory>
#include <chrono>

// Lightweight UDP networking layer for Windows (Winsock)
// Provides: UDP socket wrapper, reliable sequence for critical packets,
// ticked snapshot broadcast, simple server/client classes.

namespace Titan {

struct NetPacket {
    uint32_t sequence{};    // Sequence number for ordering
    uint32_t ack{};         // Ack of last-received sequence
    std::vector<uint8_t> payload;
};

struct Snapshot {
    uint32_t tick;
    float x, y, z; // Example: player position
    float vx, vy, vz;
    uint8_t health;
};

class IUDPTransport {
public:
    virtual ~IUDPTransport() = default;
    virtual bool Initialize(uint16_t port) = 0;
    virtual void Shutdown() = 0;
    virtual bool SendTo(const std::string& host, uint16_t port, const uint8_t* data, size_t len) = 0;
    virtual bool ReceiveFrom(std::string& outHost, uint16_t& outPort, std::vector<uint8_t>& outData) = 0;
};

class WinUDPTransport : public IUDPTransport {
public:
    WinUDPTransport();
    ~WinUDPTransport() override;
    bool Initialize(uint16_t port) override;
    void Shutdown() override;
    bool SendTo(const std::string& host, uint16_t port, const uint8_t* data, size_t len) override;
    bool ReceiveFrom(std::string& outHost, uint16_t& outPort, std::vector<uint8_t>& outData) override;

private:
    void* sock = nullptr; // opaque to avoid winsock includes in header
    uint16_t boundPort{0};
};

// Simple server that broadcasts snapshots at a fixed tick rate
class UDPServer {
public:
    UDPServer();
    ~UDPServer();
    bool Start(uint16_t listenPort, uint32_t tickRate = 60);
    void Stop();
    void Update(); // call regularly in a loop

    // Register a client (host:port). Returns client id.
    int AddClient(const std::string& host, uint16_t port);
    void RemoveClient(int clientId);

    // Push authoritative snapshot for a player id
    void PushSnapshot(int playerId, const Snapshot& snap);

private:
    std::unique_ptr<IUDPTransport> transport;
    struct ClientInfo { std::string host; uint16_t port; int id; uint32_t lastAck; };
    std::vector<ClientInfo> clients;
    uint32_t tickRate{60};
    uint32_t tickCounter{0};
    std::chrono::steady_clock::time_point lastTick;
    std::vector<Snapshot> snapshotBuffer; // indexed by playerId
};

// Simple client that connects to a server and receives snapshots
class UDPClient {
public:
    UDPClient();
    ~UDPClient();
    bool Start(const std::string& serverHost, uint16_t serverPort, uint16_t localPort = 0);
    void Stop();
    void Update();

    // Callback when a snapshot arrives
    std::function<void(const Snapshot&)> OnSnapshot;

    // Send local input or state to server
    bool SendInput(const uint8_t* data, size_t len);

private:
    std::unique_ptr<IUDPTransport> transport;
    std::string serverHost;
    uint16_t serverPort{0};
    uint32_t sequenceOut{0};
};

} // namespace Titan
