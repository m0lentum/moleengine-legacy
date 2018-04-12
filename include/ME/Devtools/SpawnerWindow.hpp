#ifndef SPAWNER_WINDOW_HPP
#define SPAWNER_WINDOW_HPP

#include <SFML/System/Time.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Window/Window.hpp>
#include <SFML/Graphics/RenderStates.hpp>
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

        /// the space that objects spawned from this window will be put in
        Space *m_space;

        std::vector<SpawnerItem> m_items;
        SpawnerItem *m_currentItem;

        bool m_isPlacingItem;

        me::Graphic m_defaultGraphic;

    public:

        void continuousUpdate(sf::Time timeElapsed);
        void draw(sf::Window &target, sf::RenderStates states) const;

        /// Add an object spawning button to the window
        void addObject(std::string name, std::function<GameObject*(Space*)> createFunction, Graphic previewGraphic);
        /// Same with default graphic
        void addObject(std::string name, std::function<GameObject*(Space*)> createFunction);

        inline void setDefaultGraphic(me::Graphic &&graphic) { m_defaultGraphic = std::move(graphic); }

        void onMousePressed(const sf::Event::MouseButtonEvent &evt);


        SpawnerWindow(Space *space);
        SpawnerWindow(SpawnerWindow &copy) = delete;
        SpawnerWindow(SpawnerWindow &&move);
        ~SpawnerWindow();

        SpawnerWindow& operator=(SpawnerWindow &&other);
    };
}


#endif // SPAWNER_WINDOW_HPP