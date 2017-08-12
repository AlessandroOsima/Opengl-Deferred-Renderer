#pragma once
#include <vector>
#include <memory>
#include "GameObjects/Component.h"

using ComponentLocation = size_t;
class Scene;

class Object
{
public:
	Object();
	virtual ~Object();

	//size_t AddComponent(std::unique_ptr<Component> && ComponentToAdd);
	template<typename T>
	T * Object::AddComponent(std::unique_ptr<Component> && ComponentToAdd)
	{
		Components.push_back(std::move(ComponentToAdd));
		ComponentLocation loc = Components.size() - 1;

		Components[loc]->OnAttached(this);
		Components[loc]->SetLogicScene(LogicScene);
		Components[loc]->Start();

		return static_cast<T*>(Components[loc].get());
	}

	void RemoveComponent(ComponentLocation Location);

	size_t GetLocation(const std::unique_ptr<Component> & ComponentToLocate);
	Component * GetComponentAtLocation(ComponentLocation Location);

	std::vector<Component *> GetComponentsOfType(ComponentsType Type);
	Component * GetComponentOfType(ComponentsType Type);


	template <typename T>
	T* GetComponentOfTypeAs(ComponentsType Type)
	{
		Component * cmp = GetComponentOfType(Type);

		if (cmp)
		{
			return static_cast<T*>(cmp);
		}

		return nullptr;
	}



	virtual void Start();
	virtual void Update(float DeltaTime);
	virtual void End();

	inline void SetLogicScene(Scene * NewLogicScene)
	{
		LogicScene = NewLogicScene;
	}


private:
	std::vector<std::unique_ptr<Component>> Components;

protected:
	//Can and sometimes will be nullptr
	Scene * LogicScene;
};

