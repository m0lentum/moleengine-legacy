#ifndef GRAPHIC_CONTROLLER_HPP
#define GRAPHIC_CONTROLLER_HPP

#include "../IController.hpp"
#include <unordered_map>
#include <memory>

namespace me
{
    class Graphic;

    class GraphicController : public IController
    {
    private:

        

    public:

        virtual std::string getType();
		virtual void draw(sf::RenderTarget& target, sf::RenderStates states);


        GraphicController();
        virtual ~GraphicController();
    };
}

#endif // GRAPHIC_CONTROLLER_HPP