#include "../include/Audio.hpp"
#include <iostream>

namespace Titan {

// ============================================================================
// SimpleAudioSystem Implementation
// ============================================================================

void SimpleAudioSystem::Initialize() {
    std::cout << "Audio system initialized" << std::endl;
}

void SimpleAudioSystem::Update(float deltaTime) {
    // Update all audio clips
}

void SimpleAudioSystem::Shutdown() {
    std::cout << "Audio system shutdown" << std::endl;
    audioClips.clear();
}

uint32_t SimpleAudioSystem::LoadAudio(const std::string& audioPath) {
    std::cout << "Loading audio: " << audioPath << std::endl;
    
    AudioClip clip;
    clip.path = audioPath;
    
    uint32_t audioID = nextAudioID++;
    audioClips[audioID] = clip;
    
    return audioID;
}

void SimpleAudioSystem::UnloadAudio(uint32_t audioID) {
    audioClips.erase(audioID);
    std::cout << "Unloaded audio: " << audioID << std::endl;
}

void SimpleAudioSystem::PlayAudio(uint32_t audioID, bool loop) {
    auto it = audioClips.find(audioID);
    if (it != audioClips.end()) {
        it->second.playing = true;
        std::cout << "Playing audio: " << audioID << std::endl;
    }
}

void SimpleAudioSystem::StopAudio(uint32_t audioID) {
    auto it = audioClips.find(audioID);
    if (it != audioClips.end()) {
        it->second.playing = false;
        std::cout << "Stopped audio: " << audioID << std::endl;
    }
}

void SimpleAudioSystem::PauseAudio(uint32_t audioID) {
    auto it = audioClips.find(audioID);
    if (it != audioClips.end()) {
        it->second.playing = false;
        std::cout << "Paused audio: " << audioID << std::endl;
    }
}

void SimpleAudioSystem::ResumeAudio(uint32_t audioID) {
    auto it = audioClips.find(audioID);
    if (it != audioClips.end()) {
        it->second.playing = true;
        std::cout << "Resumed audio: " << audioID << std::endl;
    }
}

void SimpleAudioSystem::SetVolume(uint32_t audioID, float volume) {
    auto it = audioClips.find(audioID);
    if (it != audioClips.end()) {
        it->second.volume = glm::clamp(volume, 0.0f, 1.0f);
    }
}

float SimpleAudioSystem::GetVolume(uint32_t audioID) const {
    auto it = audioClips.find(audioID);
    if (it != audioClips.end()) {
        return it->second.volume;
    }
    return 0.0f;
}

void SimpleAudioSystem::Set3DPosition(uint32_t audioID, const glm::vec3& position) {
    auto it = audioClips.find(audioID);
    if (it != audioClips.end()) {
        it->second.position = position;
    }
}

} // namespace Titan
