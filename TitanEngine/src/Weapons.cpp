#include "../include/Networking.hpp"
#include "../include/Weapons.hpp"
#include <iostream>
#include <chrono>

namespace Titan {

// ============================================================================
// Weapon Component Implementation
// ============================================================================

void WeaponComponent::Shoot() {
    if (CanShoot() && ammoInMag > 0) {
        ammoInMag--;
        timeSinceLastShot = 0.0f;
    }
}

void WeaponComponent::Reload() {
    if (!isReloading && totalAmmo > 0) {
        isReloading = true;
        reloadProgress = 0.0f;
    }
}

void WeaponComponent::Update(float deltaTime) {
    timeSinceLastShot += deltaTime;

    if (isReloading) {
        reloadProgress += deltaTime / stats.reloadTime;
        if (reloadProgress >= 1.0f) {
            int32_t ammoToReload = glm::min(stats.magSize - ammoInMag, totalAmmo);
            ammoInMag += ammoToReload;
            totalAmmo -= ammoToReload;
            isReloading = false;
            reloadProgress = 0.0f;
        }
    }
}

// ============================================================================
// Inventory Component Implementation
// ============================================================================

std::shared_ptr<WeaponComponent> InventoryComponent::GetCurrentWeapon() {
    if (currentWeaponIndex < weapons.size()) {
        return weapons[currentWeaponIndex];
    }
    return nullptr;
}

void InventoryComponent::AddWeapon(std::shared_ptr<WeaponComponent> weapon) {
    weapons.push_back(weapon);
}

void InventoryComponent::RemoveWeapon(int32_t index) {
    if (index >= 0 && index < static_cast<int32_t>(weapons.size())) {
        weapons.erase(weapons.begin() + index);
    }
}

void InventoryComponent::SwitchWeapon(int32_t index) {
    if (index >= 0 && index < static_cast<int32_t>(weapons.size())) {
        currentWeaponIndex = index;
    }
}

// ============================================================================
// Player Controller Implementation
// ============================================================================

void PlayerController::TakeDamage(float amount) {
    if (isDead) return;

    // Armor reduces damage
    float armorReduction = armor * 0.75f;
    float finalDamage = amount * (1.0f - (armorReduction / 100.0f));

    health -= finalDamage;
    armor = glm::max(0.0f, armor - amount * 0.5f);

    if (health <= 0.0f) {
        Kill();
    }
}

void PlayerController::Heal(float amount) {
    health = glm::min(health + amount, maxHealth);
}

void PlayerController::AddArmor(float amount) {
    armor = glm::min(armor + amount, maxArmor);
}

void PlayerController::Kill() {
    isDead = true;
    health = 0.0f;
    deathCount++;
}

void PlayerController::Respawn() {
    isDead = false;
    health = maxHealth;
    armor = 0.0f;
}

// ============================================================================
// SimpleNetworkManager Implementation
// ============================================================================

void SimpleNetworkManager::Initialize() {
    std::cout << "Network Manager initialized" << std::endl;
}

void SimpleNetworkManager::Update(float deltaTime) {
    accumulatedTime += deltaTime;

    while (accumulatedTime >= tickRate) {
        ProcessTick();
        accumulatedTime -= tickRate;
    }
}

void SimpleNetworkManager::Shutdown() {
    std::cout << "Network Manager shutdown" << std::endl;
    players.clear();
    incomingMessages.clear();
}

bool SimpleNetworkManager::StartServer(uint16_t port, uint32_t maxPlayers) {
    std::cout << "Starting server on port " << port << " (max " << maxPlayers << " players)" << std::endl;
    isServer = true;
    connectionState = ConnectionState::Connected;
    return true;
}

bool SimpleNetworkManager::ConnectToServer(const std::string& serverIP, uint16_t port, const std::string& playerName) {
    std::cout << "Connecting to server at " << serverIP << ":" << port << " as " << playerName << std::endl;
    connectionState = ConnectionState::Connecting;
    connectionState = ConnectionState::Connected;
    localPlayerID = nextPlayerID++;
    
    auto player = std::make_shared<NetworkPlayer>();
    player->playerID = localPlayerID;
    player->name = playerName;
    players[localPlayerID] = player;
    
    return true;
}

void SimpleNetworkManager::Disconnect() {
    std::cout << "Disconnecting from server" << std::endl;
    connectionState = ConnectionState::Disconnected;
    players.clear();
}

void SimpleNetworkManager::SendMessage(const NetMessage& message, bool reliable) {
    std::cout << "Sending message from player " << message.senderID << std::endl;
}

void SimpleNetworkManager::BroadcastMessage(const NetMessage& message, bool reliable) {
    std::cout << "Broadcasting message type " << static_cast<int>(message.type) << std::endl;
}

std::vector<NetMessage> SimpleNetworkManager::ReceiveMessages() {
    std::vector<NetMessage> messages;
    while (!incomingMessages.empty()) {
        messages.push_back(incomingMessages.front());
        incomingMessages.pop_front();
    }
    return messages;
}

std::shared_ptr<NetworkPlayer> SimpleNetworkManager::GetPlayer(uint32_t playerID) {
    auto it = players.find(playerID);
    return (it != players.end()) ? it->second : nullptr;
}

void SimpleNetworkManager::SpawnPlayer(uint32_t playerID, const glm::vec3& position) {
    auto player = GetPlayer(playerID);
    if (player) {
        player->position = position;
        player->alive = true;
        std::cout << "Player " << playerID << " spawned at (" << position.x << "," << position.y << "," << position.z << ")" << std::endl;
    }
}

void SimpleNetworkManager::KillPlayer(uint32_t playerID, uint32_t killerID) {
    auto player = GetPlayer(playerID);
    if (player) {
        player->alive = false;
        player->deaths++;
    }

    auto killer = GetPlayer(killerID);
    if (killer) {
        killer->kills++;
    }

    std::cout << "Player " << playerID << " killed by player " << killerID << std::endl;
}

void SimpleNetworkManager::ProcessTick() {
    UpdatePlayerPositions();
}

void SimpleNetworkManager::UpdatePlayerPositions() {
    for (auto& [playerID, player] : players) {
        // Update player state based on velocity
        if (player->alive) {
            player->position += player->velocity * tickRate;
        }
    }
}

} // namespace Titan
