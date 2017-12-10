#pragma once

#include "BaseState.h"
#include "ResourceManager.h"

class EntityManager;
class ParticleEngine;
class Pattern;

typedef std::vector<std::unique_ptr<Pattern>> PatternList;

class GameState : public BaseState {
public:
	GameState();
	~GameState();
	void sfmlEvent(sf::Event evt);
	void start();
	void pause();
	void resume();
	void exit();

	void update(int frame_time);
	void render(sf::RenderTarget* target);

	void gotoNext();
private:
	void runRandomPattern();

	int beat_timer;
	int beats_between_pattern;
	int beat_count;

	ResourceManager<sf::Texture, std::string> resourceManager_;
	std::unique_ptr<EntityManager> entityManager_;
	std::unique_ptr<ParticleEngine> particleEngine_;

	PatternList patternList1_;
	PatternList patternList2_;
	PatternList patternList3_;
};
