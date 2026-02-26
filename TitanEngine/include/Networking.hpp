#pragma once

#include "Core.hpp"
#include <memory>
#include <deque>
#include <unordered_set>

namespace Titan {

// ============================================================================
// Networking Types
// ============================================================================

enum class NetMessageType : uint8_t {
    PlayerJoined,
    PlayerLeft,
    PlayerSpawned,
    PlayerDeath,
    PlayerMove,
    PlayerShoot,
    ServerInfo,
    GameEvent,
    Chat,
    MapChange,
};

struct NetMessage {
    NetMessageType type;
    uint32_t senderID;
    std::vector<uint8_t> data;
};

enum class ConnectionState {
    Disconnected,
    Connecting,
    Connected,
    Playing,
};

// ============================================================================
// Network Player Info
// ============================================================================

struct NetworkPlayer {
    uint32_t playerID;
    std::string name;
    glm::vec3 position;
    glm::vec3 velocity;
    glm::quat rotation;
    float health{100.0f};
    uint32_t team{0};
    int32_t score{0};
    int32_t kills{0};
    int32_t deaths{0};
    bool alive{true};
};

// ============================================================================
// Network Manager
// ============================================================================

class NetworkManager : public ISystem {
public:
    virtual ~NetworkManager() = default;

    virtual void Initialize() override = 0;
    virtual void Update(float deltaTime) override = 0;
    virtual void Shutdown() override = 0;

    // Connection
    virtual bool StartServer(uint16_t port, uint32_t maxPlayers) = 0;
    virtual bool ConnectToServer(const std::string& serverIP, uint16_t port, const std::string& playerName) = 0;
    virtual void Disconnect() = 0;

    // Messaging
    virtual void SendMessage(const NetMessage& message, bool reliable = true) = 0;
    virtual void BroadcastMessage(const NetMessage& message, bool reliable = true) = 0;
    virtual std::vector<NetMessage> ReceiveMessages() = 0;

    // Player Management
    virtual const std::unordered_map<uint32_t, std::shared_ptr<NetworkPlayer>>& GetConnectedPlayers() const = 0;
    virtual std::shared_ptr<NetworkPlayer> GetPlayer(uint32_t playerID) = 0;
    virtual uint32_t GetLocalPlayerID() const = 0;
    virtual bool IsServer() const = 0;
    virtual bool IsConnected() const = 0;

    // Server only
    virtual void SpawnPlayer(uint32_t playerID, const glm::vec3& position) = 0;
    virtual void KillPlayer(uint32_t playerID, uint32_t killerID) = 0;
};

// ============================================================================
// Simple Network Manager Implementation
// ============================================================================

class SimpleNetworkManager : public NetworkManager {
private:
    bool isServer{false};
    uint32_t localPlayerID{0};
    ConnectionState connectionState{ConnectionState::Disconnected};
    
    std::unordered_map<uint32_t, std::shared_ptr<NetworkPlayer>> players;
    std::deque<NetMessage> incomingMessages;
    uint32_t nextPlayerID{1};

    // Timing
    float tickRate{0.016f};  // 60 ticks per second
    float accumulatedTime{0.0f};

public:
    void Initialize() override;
    void Update(float deltaTime) override;
    void Shutdown() override;

    bool StartServer(uint16_t port, uint32_t maxPlayers) override;
    bool ConnectToServer(const std::string& serverIP, uint16_t port, const std::string& playerName) override;
    void Disconnect() override;

    void SendMessage(const NetMessage& message, bool reliable = true) override;
    void BroadcastMessage(const NetMessage& message, bool reliable = true) override;
    std::vector<NetMessage> ReceiveMessages() override;

    const std::unordered_map<uint32_t, std::shared_ptr<NetworkPlayer>>& GetConnectedPlayers() const override { return players; }
    std::shared_ptr<NetworkPlayer> GetPlayer(uint32_t playerID) override;
    uint32_t GetLocalPlayerID() const override { return localPlayerID; }
    bool IsServer() const override { return isServer; }
    bool IsConnected() const override { return connectionState != ConnectionState::Disconnected; }

    void SpawnPlayer(uint32_t playerID, const glm::vec3& position) override;
    void KillPlayer(uint32_t playerID, uint32_t killerID) override;

private:
    void ProcessTick();
    void UpdatePlayerPositions();
};

} // namespace Titan
