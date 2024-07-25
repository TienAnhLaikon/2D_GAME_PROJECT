#include "EnemyFactory.h"

Enemy *EnemyFactory::createEnemy(std::string enemyType)
{
	Enemy *result = nullptr;

	if (enemyType == "Skeleton")
	{
		result = Skeleton::create();
	}
	else if (enemyType == "Slime")
	{
		result = Slime::create();
	}
	if (enemyType == "ShadowSentinels")
	{
		result = ShadowSentinels::create();
	}
	if (enemyType == "Angelos")
	{
		result = Angelos::create();
	}
	if (enemyType == "BabySpider")
	{
		result = BabySpider::create();
	}
	if (enemyType == "HellBeast")
	{
		result = HellBeast::create();
	}
	else if (enemyType == "Okyanus")
	{
		result = Okyanus::create();
	}
	else if (enemyType == "Paradiso")
	{
		result = Paradiso::create();
	}

	return result;
}