#ifndef COMPONENT_STORAGE_UNIT_HPP
#define COMPONENT_STORAGE_UNIT_HPP

namespace me
{
	class GameObject;

	// base class for pointer storage in GameObjects
	struct ComponentStorageUnitBase
	{
		GameObject *parent;
		bool isAlive;

		ComponentStorageUnitBase(GameObject *parent) :
			parent(parent),
			isAlive(true)
		{
		}
			
	};

	template <typename T>
	struct ComponentStorageUnit : ComponentStorageUnitBase
	{
		T component;

		template <typename... Args>
		ComponentStorageUnit(GameObject *parent, Args&&... args) :
			ComponentStorageUnitBase(parent),
			component(args...)
		{
		}
	};
}


#endif // COMPONENT_STORAGE_UNIT_HPP