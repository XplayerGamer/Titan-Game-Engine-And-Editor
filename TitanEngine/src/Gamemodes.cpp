#include "../include/Gamemodes.hpp"
#include <iostream>

namespace Titan {

// ============================================================================
// Deathmatch Implementation
// ============================================================================

void DeathmatchGamemode::Initialize() {
    std::cout << "Deathmatch gamemode initialized" << std::endl;
}

void DeathmatchGamemode::Update(float deltaTime) {
    if (!roundActive) return;

    roundTime += deltaTime;

    if (roundTime >= maxRoundTime) {
        EndRound();
    }

    // Check if any player reached target score
    for (const auto& [playerID, score] : playerScores) {
        if (score >= targetScore) {
            EndRound();
            break;
        }
    }
}

void DeathmatchGamemode::Shutdown() {
    std::cout << "Deathmatch gamemode shutdown" << std::endl;
}

void DeathmatchGamemode::StartRound() {
    std::cout << "Deathmatch round started" << std::endl;
    roundActive = true;
    roundTime = 0.0f;
    playerScores.clear();
}

void DeathmatchGamemode::EndRound() {
    std::cout << "Deathmatch round ended" << std::endl;
    roundActive = false;
}

void DeathmatchGamemode::OnPlayerJoined(uint32_t playerID) {
    playerScores[playerID] = 0;
    std::cout << "Player " << playerID << " joined deathmatch" << std::endl;
}

void DeathmatchGamemode::OnPlayerLeft(uint32_t playerID) {
    playerScores.erase(playerID);
    std::cout << "Player " << playerID << " left deathmatch" << std::endl;
}

void DeathmatchGamemode::OnPlayerDeath(uint32_t playerID, uint32_t killerID) {
    if (playerScores.find(killerID) != playerScores.end()) {
        playerScores[killerID]++;
    }
    std::cout << "Player " << playerID << " eliminated by " << killerID << std::endl;
}

void DeathmatchGamemode::OnPlayerRespawn(uint32_t playerID) {
    std::cout << "Player " << playerID << " respawned" << std::endl;
}

const std::vector<Team>& DeathmatchGamemode::GetTeams() const {
    static std::vector<Team> empty;
    return empty;
}

// ============================================================================
// Team Deathmatch Implementation
// ============================================================================

TeamDeathmatchGamemode::TeamDeathmatchGamemode() {
    Team team1, team2;
    
    team1.id = 0;
    team1.name = "Team 1";
    team1.color = glm::vec4(1, 0, 0, 1);  // Red
    
    team2.id = 1;
    team2.name = "Team 2";
    team2.color = glm::vec4(0, 0, 1, 1);  // Blue
    
    teams.push_back(team1);
    teams.push_back(team2);
}

void TeamDeathmatchGamemode::Initialize() {
    std::cout << "Team Deathmatch gamemode initialized" << std::endl;
}

void TeamDeathmatchGamemode::Update(float deltaTime) {
    if (!roundActive) return;

    roundTime += deltaTime;

    if (roundTime >= maxRoundTime) {
        EndRound();
        return;
    }

    // Check if any team reached target score
    for (const auto& team : teams) {
        if (team.score >= targetTeamScore) {
            EndRound();
            break;
        }
    }
}

void TeamDeathmatchGamemode::Shutdown() {
    std::cout << "Team Deathmatch gamemode shutdown" << std::endl;
}

void TeamDeathmatchGamemode::StartRound() {
    std::cout << "Team Deathmatch round started" << std::endl;
    roundActive = true;
    roundTime = 0.0f;
}

void TeamDeathmatchGamemode::EndRound() {
    std::cout << "Team Deathmatch round ended" << std::endl;
    roundActive = false;
}

void TeamDeathmatchGamemode::OnPlayerJoined(uint32_t playerID) {
    std::cout << "Player " << playerID << " joined Team Deathmatch" << std::endl;
}

void TeamDeathmatchGamemode::OnPlayerLeft(uint32_t playerID) {
    std::cout << "Player " << playerID << " left Team Deathmatch" << std::endl;
}

void TeamDeathmatchGamemode::OnPlayerDeath(uint32_t playerID, uint32_t killerID) {
    std::cout << "Player " << playerID << " eliminated by " << killerID << std::endl;
}

void TeamDeathmatchGamemode::OnPlayerRespawn(uint32_t playerID) {
    std::cout << "Player " << playerID << " respawned" << std::endl;
}

int32_t TeamDeathmatchGamemode::GetWinningTeam() const {
    if (teams[0].score > teams[1].score) return 0;
    if (teams[1].score > teams[0].score) return 1;
    return -1;
}

// ============================================================================
// Bomb Defusal Implementation
// ============================================================================

BombDefusalGamemode::BombDefusalGamemode() {
    Team terrorist, counterTerrorist;
    
    terrorist.id = 0;
    terrorist.name = "Terrorists";
    terrorist.color = glm::vec4(1, 0.5f, 0, 1);  // Orange
    
    counterTerrorist.id = 1;
    counterTerrorist.name = "Counter-Terrorists";
    counterTerrorist.color = glm::vec4(0, 0.5f, 1, 1);  // Light Blue
    
    teams.push_back(terrorist);
    teams.push_back(counterTerrorist);

    // Set bomb sites
    bombSite_A = glm::vec3(100, 0, 100);
    bombSite_B = glm::vec3(-100, 0, -100);
}

void BombDefusalGamemode::Initialize() {
    std::cout << "Bomb Defusal gamemode initialized" << std::endl;
}

void BombDefusalGamemode::Update(float deltaTime) {
    if (!roundActive) return;

    roundTime += deltaTime;

    if (bombPlanted) {
        bombPlantTime += deltaTime;

        if (bombPlantTime >= bombDetonationTime) {
            // Bomb detonated, terrorists win
            teams[0].score++;
            std::cout << "Bomb detonated! Terrorists win round." << std::endl;
            EndRound();
        }
    }

    if (roundTime >= maxRoundTime) {
        // Time expired, counter-terrorists win
        teams[1].score++;
        EndRound();
    }
}

void BombDefusalGamemode::Shutdown() {
    std::cout << "Bomb Defusal gamemode shutdown" << std::endl;
}

void BombDefusalGamemode::StartRound() {
    std::cout << "Bomb Defusal round started" << std::endl;
    roundActive = true;
    roundTime = 0.0f;
    bombPlanted = false;
    bombPlantTime = 0.0f;
}

void BombDefusalGamemode::EndRound() {
    std::cout << "Bomb Defusal round ended" << std::endl;
    roundActive = false;
    bombPlanted = false;
}

void BombDefusalGamemode::OnPlayerJoined(uint32_t playerID) {
    std::cout << "Player " << playerID << " joined Bomb Defusal" << std::endl;
}

void BombDefusalGamemode::OnPlayerLeft(uint32_t playerID) {
    std::cout << "Player " << playerID << " left Bomb Defusal" << std::endl;
}

void BombDefusalGamemode::OnPlayerDeath(uint32_t playerID, uint32_t killerID) {
    std::cout << "Player " << playerID << " eliminated by " << killerID << std::endl;
}

void BombDefusalGamemode::OnPlayerRespawn(uint32_t playerID) {
    // No respawning during active bomb defusal round (except eco rounds)
    std::cout << "Player " << playerID << " will respawn next round" << std::endl;
}

void BombDefusalGamemode::PlantBomb(uint32_t playerID) {
    if (!bombPlanted && roundActive) {
        bombPlanted = true;
        bombPlantTime = 0.0f;
        std::cout << "Player " << playerID << " planted the bomb!" << std::endl;
    }
}

void BombDefusalGamemode::DefuseBomb(uint32_t playerID) {
    if (bombPlanted && roundActive) {
        bombPlanted = false;
        bombPlantTime = 0.0f;
        teams[1].score++;
        std::cout << "Player " << playerID << " defused the bomb! Counter-Terrorists win!" << std::endl;
        EndRound();
    }
}

float BombDefusalGamemode::GetBombPlantProgress() const {
    if (!bombPlanted) return 0.0f;
    return glm::clamp(bombPlantTime / bombDetonationTime, 0.0f, 1.0f);
}

int32_t BombDefusalGamemode::GetWinningTeam() const {
    if (teams[0].score > teams[1].score) return 0;
    if (teams[1].score > teams[0].score) return 1;
    return -1;
}

} // namespace Titan
