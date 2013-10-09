#pragma once

#include <SFML/Audio/Music.hpp>

#include <unordered_map>
#include <string>

class Music
{
public:
    Music();
    ~Music();

    void addSong(const std::string& name, const std::string& file);
    // void addSong(const std::string& name, const PhysFS& file);

    void setVolume(const std::string& name, float volume, float time = 0.f);

    void update(float dt);

private:
    void setVolume(sf::Music&, float volume);

    struct volumeChange
    {
        float begin;
        float end;

        float duration;
        float spent;
    };

    std::unordered_map<std::string, sf::Music*> mMusic;
    std::unordered_map<sf::Music*, volumeChange> mChanges;
};
