#ifndef SPAWNER_WINDOW_HPP
#define SPAWNER_WINDOW_HPP

#include <SFML/System/Time.hpp>
#include <vector>
#include <functional>
#include "../Graphics/Graphic.hpp"
#include "../Space.hpp"

namespace me
{
    struct SpawnerItem
    {
        std::string name;
        std::function<GameObject*(Space*)> createFunction;
        Graphic previewGraphic;

        SpawnerItem(std::string name, std::function<GameObject*(Space*)> createFunction, Graphic previewGraphic) :
            name(name), createFunction(createFunction), previewGraphic(previewGraphic)
        { }
    };


    class SpawnerWindow
    {
    private:

        Space *m_space; // the space that objects spawned from this window will be put in

        std::vector<SpawnerItem> m_items;

        me::Graphic m_defaultGraphic;

    public:

        void continuousUpdate(sf::Time timeElapsed);

        /// Add an object spawning button to the window
        void addObject(std::string name, std::function<GameObject*(Space*)> createFunction, Graphic previewGraphic);
        /// Same with default graphic
        void addObject(std::string name, std::function<GameObject*(Space*)> createFunction);

        void setDefaultGraphic(me::Graphic graphic);

        SpawnerWindow(Space *space);
        SpawnerWindow(SpawnerWindow &copy);
        SpawnerWindow(SpawnerWindow &&move);
        ~SpawnerWindow();

        SpawnerWindow& operator=(SpawnerWindow &&other);
    };
}


#endif // SPAWNER_WINDOW_HPP