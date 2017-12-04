#ifndef COMPONENT_STORAGE_UNIT_HPP
#define COMPONENT_STORAGE_UNIT_HPP

namespace me
{
	class GameObject;

	// base class for pointer storage in GameObjects
	struct ComponentStorageUnitBase
	{
		GameObject *parent;
		bool alive = true;
	};

	template <typename T>
	struct ComponentStorageUnit : ComponentStorageUnitBase
	{
		T component;
	};
}


#endif // COMPONENT_STORAGE_UNIT_HPP