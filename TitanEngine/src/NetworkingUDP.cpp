#include "../include/NetworkingUDP.hpp"
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdexcept>
#include <iostream>

#pragma comment(lib, "Ws2_32.lib")

namespace Titan {

// Helper for converting socket addr to string
static std::string SockaddrToString(const sockaddr_in& addr) {
    char buf[INET_ADDRSTRLEN] = {0};
    inet_ntop(AF_INET, &addr.sin_addr, buf, sizeof(buf));
    return std::string(buf);
}

// -------------------- WinUDPTransport --------------------
struct WinUDPTransport::Impl {
    SOCKET sock = INVALID_SOCKET;
    uint16_t port = 0;
};

WinUDPTransport::WinUDPTransport() {
    sock = nullptr;
}

WinUDPTransport::~WinUDPTransport() {
    Shutdown();
}

bool WinUDPTransport::Initialize(uint16_t port) {
    WSADATA wsa;
    if (WSAStartup(MAKEWORD(2,2), &wsa) != 0) {
        std::cerr << "WSAStartup failed" << std::endl;
        return false;
    }

    Impl* impl = new Impl();
    impl->sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (impl->sock == INVALID_SOCKET) {
        std::cerr << "Failed to create socket" << std::endl;
        delete impl;
        return false;
    }

    sockaddr_in service;
    service.sin_family = AF_INET;
    service.sin_addr.s_addr = INADDR_ANY;
    service.sin_port = htons(port);

    if (bind(impl->sock, (sockaddr*)&service, sizeof(service)) == SOCKET_ERROR) {
        std::cerr << "bind() failed: " << WSAGetLastError() << std::endl;
        closesocket(impl->sock);
        delete impl;
        return false;
    }

    sock = impl;
    boundPort = port;
    return true;
}

void WinUDPTransport::Shutdown() {
    if (!sock) return;
    Impl* impl = static_cast<Impl*>(sock);
    if (impl->sock != INVALID_SOCKET) {
        closesocket(impl->sock);
        impl->sock = INVALID_SOCKET;
    }
    delete impl;
    sock = nullptr;
    WSACleanup();
}

bool WinUDPTransport::SendTo(const std::string& host, uint16_t port, const uint8_t* data, size_t len) {
    if (!sock) return false;
    Impl* impl = static_cast<Impl*>(sock);

    sockaddr_in dest;
    dest.sin_family = AF_INET;
    inet_pton(AF_INET, host.c_str(), &dest.sin_addr);
    dest.sin_port = htons(port);

    int sent = sendto(impl->sock, reinterpret_cast<const char*>(data), static_cast<int>(len), 0,
                      (sockaddr*)&dest, sizeof(dest));
    return sent == static_cast<int>(len);
}

bool WinUDPTransport::ReceiveFrom(std::string& outHost, uint16_t& outPort, std::vector<uint8_t>& outData) {
    if (!sock) return false;
    Impl* impl = static_cast<Impl*>(sock);

    char buffer[1500];
    sockaddr_in from;
    int fromLen = sizeof(from);
    int recvd = recvfrom(impl->sock, buffer, sizeof(buffer), 0, (sockaddr*)&from, &fromLen);
    if (recvd == SOCKET_ERROR || recvd == 0) return false;

    outHost = SockaddrToString(from);
    outPort = ntohs(from.sin_port);
    outData.assign(buffer, buffer + recvd);
    return true;
}

// -------------------- UDPServer --------------------
UDPServer::UDPServer() {
    transport = std::make_unique<WinUDPTransport>();
}

UDPServer::~UDPServer() {
    Stop();
}

bool UDPServer::Start(uint16_t listenPort, uint32_t tickRate_) {
    if (!transport->Initialize(listenPort)) return false;
    tickRate = tickRate_;
    lastTick = std::chrono::steady_clock::now();
    tickCounter = 0;
    snapshotBuffer.clear();
    return true;
}

void UDPServer::Stop() {
    transport->Shutdown();
    clients.clear();
}

int UDPServer::AddClient(const std::string& host, uint16_t port) {
    int id = static_cast<int>(clients.size());
    clients.push_back({host, port, id, 0});
    return id;
}

void UDPServer::RemoveClient(int clientId) {
    clients.erase(std::remove_if(clients.begin(), clients.end(), [clientId](const ClientInfo& c){ return c.id == clientId; }), clients.end());
}

void UDPServer::PushSnapshot(int playerId, const Snapshot& snap) {
    if (playerId >= static_cast<int>(snapshotBuffer.size())) snapshotBuffer.resize(playerId + 1);
    snapshotBuffer[playerId] = snap;
}

void UDPServer::Update() {
    // Receive incoming packets and handle acks/inputs
    std::string host; uint16_t port; std::vector<uint8_t> data;
    while (transport->ReceiveFrom(host, port, data)) {
        // Basic: ignore payload processing for now
        (void)host; (void)port; (void)data;
    }

    // Tick logic: broadcast snapshots at tickRate
    auto now = std::chrono::steady_clock::now();
    auto elapsed = std::chrono::duration<float>(now - lastTick).count();
    float targetSec = 1.0f / static_cast<float>(tickRate);
    if (elapsed >= targetSec) {
        // Build a simple packet with tick and all player snapshots
        for (const auto& client : clients) {
            std::vector<uint8_t> out;
            // packet format: tick(uint32), count(uint32), repeated snapshots
            uint32_t tick = ++tickCounter;
            out.resize(8);
            memcpy(out.data(), &tick, 4);
            uint32_t count = static_cast<uint32_t>(snapshotBuffer.size());
            memcpy(out.data() + 4, &count, 4);
            for (const auto& s : snapshotBuffer) {
                size_t old = out.size();
                out.resize(old + sizeof(Snapshot));
                memcpy(out.data() + old, &s, sizeof(Snapshot));
            }
            transport->SendTo(client.host, client.port, out.data(), out.size());
        }
        lastTick = now;
    }
}

// -------------------- UDPClient --------------------
UDPClient::UDPClient() {
    transport = std::make_unique<WinUDPTransport>();
}

UDPClient::~UDPClient() {
    Stop();
}

bool UDPClient::Start(const std::string& srvHost, uint16_t srvPort, uint16_t localPort) {
    if (!transport->Initialize(localPort)) return false;
    serverHost = srvHost;
    serverPort = srvPort;
    return true;
}

void UDPClient::Stop() {
    transport->Shutdown();
}

void UDPClient::Update() {
    // Poll for incoming packets
    std::string host; uint16_t port; std::vector<uint8_t> data;
    while (transport->ReceiveFrom(host, port, data)) {
        if (data.size() < 8) continue;
        uint32_t tick = 0; uint32_t count = 0;
        memcpy(&tick, data.data(), 4);
        memcpy(&count, data.data() + 4, 4);
        size_t expected = 8 + count * sizeof(Snapshot);
        if (data.size() != expected) continue;
        const uint8_t* ptr = data.data() + 8;
        for (uint32_t i = 0; i < count; ++i) {
            Snapshot s;
            memcpy(&s, ptr, sizeof(Snapshot));
            ptr += sizeof(Snapshot);
            if (OnSnapshot) OnSnapshot(s);
        }
    }
}

bool UDPClient::SendInput(const uint8_t* data, size_t len) {
    if (!transport) return false;
    // Prepend sequence
    uint32_t seq = ++sequenceOut;
    std::vector<uint8_t> out(len + 4);
    memcpy(out.data(), &seq, 4);
    memcpy(out.data() + 4, data, len);
    return transport->SendTo(serverHost, serverPort, out.data(), out.size());
}

} // namespace Titan
