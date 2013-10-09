#include "Music.hpp"

Music::Music()
{

}

Music::~Music()
{
    for (std::unordered_map<std::string, sf::Music*>::iterator it = mMusic.begin(); it != mMusic.end(); ++it)
    {
        delete it->second;
    }
}


void Music::addSong(const std::string& name, const std::string& file)
{
    sf::Music* song = new sf::Music();
    
    song->setLoop(true);
    song->openFromFile(file);

    mMusic[name] = song;
}


void Music::setVolume(const std::string& name, float volume, float time)
{
    sf::Music* m = mMusic.at(name);

    if (time == 0)
        setVolume(*m, volume);
    else
    {
        volumeChange change;
        change.begin = m->getVolume();
        change.end = volume;
        change.duration = time;
        change.spent = 0;

        mChanges[m] = change;
    }
}


void Music::update(float dt)
{
    for (auto it = mChanges.begin(); it != mChanges.end();)
    {
        float perc = it->second.spent / it->second.duration;

        setVolume(*it->first, it->second.begin + (it->second.end - it->second.begin) * perc);

        it->second.spent += dt;

        if (perc >= 1)
            it = mChanges.erase(it);
        else
            ++it;
    }
}


inline void Music::setVolume(sf::Music& mus, float volume)
{
    if (volume > 0.5 && mus.getStatus() != sf::Music::Playing)
        mus.play();
    else if (volume < 0.5 && mus.getStatus() == sf::Music::Playing)
        mus.stop();

    mus.setVolume(volume);
}