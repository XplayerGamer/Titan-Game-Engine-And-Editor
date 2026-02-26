#pragma once

#include "Core.hpp"

namespace Titan {

// ============================================================================
// Audio System Interface
// ============================================================================

class AudioSystem : public ISystem {
public:
    virtual ~AudioSystem() = default;

    virtual void Initialize() override = 0;
    virtual void Update(float deltaTime) override = 0;
    virtual void Shutdown() override = 0;

    virtual uint32_t LoadAudio(const std::string& audioPath) = 0;
    virtual void UnloadAudio(uint32_t audioID) = 0;

    virtual void PlayAudio(uint32_t audioID, bool loop = false) = 0;
    virtual void StopAudio(uint32_t audioID) = 0;
    virtual void PauseAudio(uint32_t audioID) = 0;
    virtual void ResumeAudio(uint32_t audioID) = 0;

    virtual void SetVolume(uint32_t audioID, float volume) = 0;
    virtual float GetVolume(uint32_t audioID) const = 0;

    virtual void Set3DPosition(uint32_t audioID, const glm::vec3& position) = 0;
};

// ============================================================================
// Simple Audio System Implementation
// ============================================================================

class SimpleAudioSystem : public AudioSystem {
private:
    struct AudioClip {
        std::string path;
        float volume{1.0f};
        bool playing{false};
        glm::vec3 position{0.0f};
    };

    std::unordered_map<uint32_t, AudioClip> audioClips;
    uint32_t nextAudioID{1};

public:
    void Initialize() override;
    void Update(float deltaTime) override;
    void Shutdown() override;

    uint32_t LoadAudio(const std::string& audioPath) override;
    void UnloadAudio(uint32_t audioID) override;

    void PlayAudio(uint32_t audioID, bool loop = false) override;
    void StopAudio(uint32_t audioID) override;
    void PauseAudio(uint32_t audioID) override;
    void ResumeAudio(uint32_t audioID) override;

    void SetVolume(uint32_t audioID, float volume) override;
    float GetVolume(uint32_t audioID) const override;

    void Set3DPosition(uint32_t audioID, const glm::vec3& position) override;
};

} // namespace Titan
