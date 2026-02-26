#pragma once

#include "Core.hpp"
#include <memory>
#include <vector>

namespace Titan {

// ============================================================================
// Gamemode Types
// ============================================================================

enum class GamemodeType {
    Deathmatch,
    TeamDeathmatch,
    BombDefusal,
    HostageRescue,
    Custom,
};

// ============================================================================
// Team
// ============================================================================

struct Team {
    uint32_t id;
    std::string name;
    glm::vec4 color;
    std::vector<uint32_t> playerIDs;
    int32_t score{0};
};

// ============================================================================
// Gamemode Interface
// ============================================================================

class Gamemode : public ISystem {
public:
    virtual ~Gamemode() = default;

    virtual void Initialize() override = 0;
    virtual void Update(float deltaTime) override = 0;
    virtual void Shutdown() override = 0;

    // Gamemode lifecycle
    virtual void StartRound() = 0;
    virtual void EndRound() = 0;
    virtual void OnPlayerJoined(uint32_t playerID) = 0;
    virtual void OnPlayerLeft(uint32_t playerID) = 0;
    virtual void OnPlayerDeath(uint32_t playerID, uint32_t killerID) = 0;
    virtual void OnPlayerRespawn(uint32_t playerID) = 0;

    // Game state
    virtual GamemodeType GetGamemodeType() const = 0;
    virtual bool IsRoundActive() const = 0;
    virtual float GetRoundTimeRemaining() const = 0;
    virtual const std::vector<Team>& GetTeams() const = 0;
    virtual int32_t GetWinningTeam() const = 0;
};

// ============================================================================
// Deathmatch Gamemode
// ============================================================================

class DeathmatchGamemode : public Gamemode {
private:
    bool roundActive{false};
    float roundTime{0.0f};
    float maxRoundTime{600.0f};  // 10 minutes
    int32_t targetScore{50};
    std::unordered_map<uint32_t, int32_t> playerScores;

public:
    void Initialize() override;
    void Update(float deltaTime) override;
    void Shutdown() override;

    void StartRound() override;
    void EndRound() override;
    void OnPlayerJoined(uint32_t playerID) override;
    void OnPlayerLeft(uint32_t playerID) override;
    void OnPlayerDeath(uint32_t playerID, uint32_t killerID) override;
    void OnPlayerRespawn(uint32_t playerID) override;

    GamemodeType GetGamemodeType() const override { return GamemodeType::Deathmatch; }
    bool IsRoundActive() const override { return roundActive; }
    float GetRoundTimeRemaining() const override { return maxRoundTime - roundTime; }
    const std::vector<Team>& GetTeams() const override;
    int32_t GetWinningTeam() const override { return -1; }
};

// ============================================================================
// Team Deathmatch Gamemode
// ============================================================================

class TeamDeathmatchGamemode : public Gamemode {
private:
    bool roundActive{false};
    float roundTime{0.0f};
    float maxRoundTime{600.0f};
    int32_t targetTeamScore{50};
    std::vector<Team> teams;

public:
    TeamDeathmatchGamemode();

    void Initialize() override;
    void Update(float deltaTime) override;
    void Shutdown() override;

    void StartRound() override;
    void EndRound() override;
    void OnPlayerJoined(uint32_t playerID) override;
    void OnPlayerLeft(uint32_t playerID) override;
    void OnPlayerDeath(uint32_t playerID, uint32_t killerID) override;
    void OnPlayerRespawn(uint32_t playerID) override;

    GamemodeType GetGamemodeType() const override { return GamemodeType::TeamDeathmatch; }
    bool IsRoundActive() const override { return roundActive; }
    float GetRoundTimeRemaining() const override { return maxRoundTime - roundTime; }
    const std::vector<Team>& GetTeams() const override { return teams; }
    int32_t GetWinningTeam() const override;
};

// ============================================================================
// Bomb Defusal Gamemode (CS-Style)
// ============================================================================

class BombDefusalGamemode : public Gamemode {
private:
    bool roundActive{false};
    float roundTime{0.0f};
    float maxRoundTime{135.0f};  // 2:15 round time
    
    std::vector<Team> teams;  // Team 0 = T, Team 1 = CT
    glm::vec3 bombSite_A{0, 0, 0};
    glm::vec3 bombSite_B{0, 0, 0};
    
    bool bombPlanted{false};
    float bombPlantTime{0.0f};
    float bombDetonationTime{40.0f};
    
    int32_t teamATerroristWins{0};
    int32_t teamBCTWins{0};
    int32_t maxWins{16};

public:
    BombDefusalGamemode();

    void Initialize() override;
    void Update(float deltaTime) override;
    void Shutdown() override;

    void StartRound() override;
    void EndRound() override;
    void OnPlayerJoined(uint32_t playerID) override;
    void OnPlayerLeft(uint32_t playerID) override;
    void OnPlayerDeath(uint32_t playerID, uint32_t killerID) override;
    void OnPlayerRespawn(uint32_t playerID) override;

    // Bomb specific
    void PlantBomb(uint32_t playerID);
    void DefuseBomb(uint32_t playerID);
    bool IsBombPlanted() const { return bombPlanted; }
    float GetBombPlantProgress() const;

    GamemodeType GetGamemodeType() const override { return GamemodeType::BombDefusal; }
    bool IsRoundActive() const override { return roundActive; }
    float GetRoundTimeRemaining() const override { return maxRoundTime - roundTime; }
    const std::vector<Team>& GetTeams() const override { return teams; }
    int32_t GetWinningTeam() const override;
};

} // namespace Titan
