#pragma once

#include "Entity.h"

class Player : public Entity {
public:
	Player(ResourceManager<sf::Texture, std::string>* resourceManager, EntityManager* entityManager);

	void update(int frame_time);
	void sfmlEvent(sf::Event evt);
	void takeDamage(int amount);
	int getHealth() const;
	void render(sf::RenderTarget* target);
private:
	bool dead;
	int timer_;
	int reload_;

	void switchColumn(bool left);
};