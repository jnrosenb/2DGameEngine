#ifndef PLAYER_H
#define PLAYER_H

#include "Character.h"
#include "../Managers.h"
#include "GL/glew.h"
#include "GL/gl.h"

class Event;
class GameObject;


class Player : public Character
{
public:
	Player(GameObject *owner, COMPONENT_TYPE type);
	virtual ~Player();

	void Update(unsigned int deltaTime);

	virtual Component *createNew(GameObject *owner);
	virtual void serialize(std::fstream& stream);
	virtual void deserialize(std::fstream& stream);
	virtual void handleEvent(Event *pEvent);
	
	void EquipJetpack();
	void UseJetpack();

	void TakeDamage(int damage = 1);
	void TakeDamage(Vector3D launchDir, int damage = 1);

private:
	int maxHealth;
	float maxGas;
	float gasRegenSpeed;

	int health;
	float gas;
	int score;
	int gold;

	float invulnerabilityTime;
	float timeSinceHit;

	bool hasJetpack;
};

#endif