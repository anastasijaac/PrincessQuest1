#ifndef MONSTER
#define MONSTER

#include <cstdlib>
#include "LebeEntity.h"

class Monster : public LebeEntity {
public:
	static const string MONSTER_ANIM_RIGHT;
	static const string MONSTER_ANIM_LEFT;
	static const string MONSTER_ANIM_IDLELEFT;
	static const string MONSTER_ANIM_IDLERIGHT;
	static const string MONSTER_ANIM_IDLEUP;
	static const string MONSTER_ANIM_ATTACK;
	static const string MONSTER_ANIM_RIP_RIGHT;
	static const string MONSTER_ANIM_RIP_LEFT;

	static const int MONSTER_STATE_IDLE;
	static const int MONSTER_STATE_BEWEGUNG;
	static const int MONSTER_STATE_ATTACK;
	static const int MONSTER_STATE_DEAD;

	static const int MONSTER_AI_NORMAL;
	static const int MONSTER_AI_CHASE;

	static int MonsterOpfer;

	float denkTimer = 0; //AI
	LebeEntity* ziel = nullptr;

	int AIStatus = MONSTER_AI_NORMAL;

	Monster(AnimationSet* animSet);
	void update();
	void denken();
	void verfolgen();
	void attacke();
	void sterben();
	void sucheZiel();
	void wechselAnimation(int neuerStatus, bool neustartFrameZumStart);
	void updateAnimation();
	void updateSchaeden();

};
#endif 

