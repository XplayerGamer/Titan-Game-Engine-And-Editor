#pragma once

#include "Core.hpp"
#include <memory>
#include <vector>

namespace Titan {

// ============================================================================
// Weapon Types
// ============================================================================

enum class WeaponType {
    Pistol,
    SMG,
    Rifle,
    Sniper,
    Shotgun,
    Knife,
};

// ============================================================================
// Weapon Properties
// ============================================================================

struct WeaponStats {
    float damage{0.0f};
    float fireRate{0.0f};
    float accuracy{1.0f};
    float recoil{0.0f};
    int32_t magSize{30};
    float reloadTime{2.5f};
    float range{1000.0f};
    WeaponType type;
};

// ============================================================================
// Weapon Component
// ============================================================================

class WeaponComponent : public Component {
public:
    WeaponStats stats;
    int32_t ammoInMag{30};
    int32_t totalAmmo{120};
    float timeSinceLastShot{0.0f};
    bool isReloading{false};
    float reloadProgress{0.0f};

    WeaponComponent() = default;
    explicit WeaponComponent(const WeaponStats& s) : stats(s), ammoInMag(s.magSize), totalAmmo(s.magSize * 4) {}

    static constexpr ComponentID StaticID() { return 50; }
    ComponentID GetComponentID() const override { return StaticID(); }

    bool CanShoot() const {
        return ammoInMag > 0 && timeSinceLastShot >= (1.0f / stats.fireRate) && !isReloading;
    }

    void Shoot();
    void Reload();
    void Update(float deltaTime);
};

// ============================================================================
// Inventory Component
// ============================================================================

class InventoryComponent : public Component {
public:
    std::vector<std::shared_ptr<WeaponComponent>> weapons;
    int32_t currentWeaponIndex{0};
    uint32_t money{2400};
    bool hasDefuse{false};
    int32_t grenades{0};

    static constexpr ComponentID StaticID() { return 51; }
    ComponentID GetComponentID() const override { return StaticID(); }

    std::shared_ptr<WeaponComponent> GetCurrentWeapon();
    void AddWeapon(std::shared_ptr<WeaponComponent> weapon);
    void RemoveWeapon(int32_t index);
    void SwitchWeapon(int32_t index);
};

// ============================================================================
// Player Controller Component
// ============================================================================

class PlayerController : public Component {
public:
    float health{100.0f};
    float maxHealth{100.0f};
    float armor{0.0f};
    float maxArmor{100.0f};
    
    // Movement
    float moveSpeed{250.0f};
    float sprintSpeed{350.0f};
    float isSprinting{false};
    float crouchSpeed{150.0f};
    float isCrouching{false};
    
    // State
    bool isDead{false};
    uint32_t team{0};
    int32_t killCount{0};
    int32_t deathCount{0};
    int32_t assistCount{0};

    static constexpr ComponentID StaticID() { return 52; }
    ComponentID GetComponentID() const override { return StaticID(); }

    void TakeDamage(float amount);
    void Heal(float amount);
    void AddArmor(float amount);
    void Kill();
    void Respawn();
};

// ============================================================================
// Damage Info
// ============================================================================

struct DamageInfo {
    float amount;
    uint32_t attackerID;
    glm::vec3 hitPosition;
    glm::vec3 direction;
    WeaponType weaponType;
};

} // namespace Titan
