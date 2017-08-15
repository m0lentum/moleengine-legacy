#include <GameObject.hpp>

//=======================GAMEOBJECT_ANIMATED====================================
void me::GameObjectAnimated::update()
{
	//will fix this when remaking the GameObject class
	//graphic->update(sf::Time());
}

void me::GameObjectAnimated::setGraphic(const me::AnimatedSprite &theGraphic)
{
	graphic = std::unique_ptr<me::AnimatedSprite>(new me::AnimatedSprite(theGraphic));
}

me::GameObjectAnimated::GameObjectAnimated(const GameObjectAnimated &copy) :
	graphic(new me::AnimatedSprite(*copy.graphic))
{
	setPosition(copy.getPosition());
	setOrigin(copy.getOrigin());
	setRotation(copy.getRotation());
	setScale(copy.getScale());
}

me::GameObjectAnimated::GameObjectAnimated(const me::AnimatedSprite &graphic) :
	graphic(new me::AnimatedSprite(graphic))
{
}

void me::GameObjectAnimated::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
	target.draw(*graphic, states.transform.combine(this->getTransform()));
}



//===================GAMEOBJECT_STATIC=================================
void me::GameObjectStatic::update()
{
	//nothing to do here
}

void me::GameObjectStatic::setGraphic(sf::Drawable *theGraphic)
{
	graphic.reset(theGraphic);
}

me::GameObjectStatic::GameObjectStatic(GameObjectStatic &copy) :
	graphic(copy.graphic.get())
{
	setPosition(copy.getPosition());
	setOrigin(copy.getOrigin());
	setRotation(copy.getRotation());
	setScale(copy.getScale());
}

me::GameObjectStatic::GameObjectStatic(sf::Drawable *graphic) :
	graphic(graphic) //cannot copy abstract class, have to settle with this
{
}

void me::GameObjectStatic::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
	target.draw(*graphic, states.transform.combine(this->getTransform()));
}