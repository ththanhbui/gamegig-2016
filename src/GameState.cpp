#include "stdafx.h"
#include "GameState.h"
#include "EntityManager.h"
#include "Player.h"
#include "StaticObj.h"
#include "ParticleEngine.h"
#include "Pattern.h"
#include "PatternData.h"
#include "SoundManager.h"

GameState::GameState() = default;
GameState::~GameState() = default;

void GameState::sfmlEvent(sf::Event evt) {
	entityManager_->sfmlEvent(evt);
}

void GameState::gotoNext() {
	beat_count = beats_between_pattern;
}

void GameState::start() {
	SoundManager::addMusic("main", "media/music/gjmusic.wav");
	SoundManager::add("enemy_laser", "media/sounds/enemy_laser.wav");
	SoundManager::add("player_gun", "media/sounds/player_gun.wav");
	SoundManager::add("enemy_explosion", "media/sounds/enemy_explosion.wav");
	SoundManager::add("player_explosion", "media/sounds/player_explosion.wav");
	resourceManager_.setDirectory("media/images/");
	resourceManager_.load("player", "player.png");
	resourceManager_.load("enemy", "enemy.png");
	resourceManager_.load("bullet", "bullet.png");
	resourceManager_.load("enemy_shoot", "enemy-charging.png");
	resourceManager_.load("background", "background.png");
	resourceManager_.load("laser-texture", "laser-texture.bmp");

	beat_timer = 0;
	beats_between_pattern = 16;
	beat_count = beats_between_pattern;

	particleEngine_ = std::unique_ptr<ParticleEngine>(new ParticleEngine());
	entityManager_ = std::unique_ptr<EntityManager>(new EntityManager(&resourceManager_, particleEngine_.get()));
	entityManager_->setGameState(this);
	/*Pattern* test = new Pattern(entityManager_.get(), &resourceManager_);
	std::vector<Enemy::Action> actions;
	actions.push_back(Enemy::ACTION_DOWN);
	actions.push_back(Enemy::ACTION_LEFT);
	actions.push_back(Enemy::ACTION_DOWN);
	actions.push_back(Enemy::ACTION_LEFT);
	actions.push_back(Enemy::ACTION_SHOOT);
	actions.push_back(Enemy::ACTION_DOWN);
	actions.push_back(Enemy::ACTION_RIGHT);
	actions.push_back(Enemy::ACTION_DOWN);
	actions.push_back(Enemy::ACTION_RIGHT);

	for (int x = 40; x <= 1000; x += 3*TILE_SIZE) {
		PatternData data(actions, sfld::Vector2f(x, 50), "e");
		test->addData(data);
	}

	patternList1_.push_back(std::unique_ptr<Pattern>(test));*/

	for (int i = 1; i <= 3; i++) {
		Pattern* pat = new Pattern(entityManager_.get(), &resourceManager_);
		pat->loadFile("pe" + std::to_string(i) + ".txt");
		patternList1_.push_back(std::unique_ptr<Pattern>(pat));
	}

	for (int i = 1; i <= 4; i++) {
		Pattern* pat = new Pattern(entityManager_.get(), &resourceManager_);
		pat->loadFile("pm" + std::to_string(i) + ".txt");
		patternList2_.push_back(std::unique_ptr<Pattern>(pat));
	}
	for (int i = 1; i <= 2; i++) {
		Pattern* pat = new Pattern(entityManager_.get(), &resourceManager_);
		pat->loadFile("ph" + std::to_string(i) + ".txt");
		patternList3_.push_back(std::unique_ptr<Pattern>(pat));
	}

	Player* player = new Player(&resourceManager_, entityManager_.get());
	entityManager_->setPlayer(player);
	entityManager_->addEntity(player);
	SoundManager::playMusic("main");
	//entityManager_->addEntity(new StaticObj(&resourceManager_, entityManager_.get(), "test2", sfld::Vector2f(200, 200), Entity::SHAPE_SQUARE, Entity::TYPE_DEFAULT));
}

void GameState::pause() {

}

void GameState::resume() {

}

void GameState::exit() {

}

void GameState::runRandomPattern() {
	PatternList* patternList;
	if (entityManager_->getScore() < 5) {
		beats_between_pattern = 5;
		patternList = &patternList1_;
	}
	else if (entityManager_->getScore() < 10) {
		beats_between_pattern = 10;
		patternList = &patternList2_;
	}
	else{
		beats_between_pattern = 16;
		patternList = &patternList3_;
	}

	if (entityManager_->getScore() > 30) {
		EntityManager::bpm = 120;
	}
	int r = rand() % patternList->size();
	patternList->at(r)->runPattern();
}

void GameState::update(int frame_time) {
	beat_timer += frame_time;
	if (beat_timer > (60.0f / (float)EntityManager::bpm) * 1000.0f) {
		entityManager_->beat();
		beat_timer = 0;
		beat_count++;
	}
	if (beat_count > beats_between_pattern) {
		beat_count = 0;
		runRandomPattern();
	}
	entityManager_->update(frame_time);
}

void GameState::render(sf::RenderTarget* target) {
	entityManager_->render(target);
}

