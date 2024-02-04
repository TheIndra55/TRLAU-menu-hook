#pragma once

class EnemyHealth
{
public:
	float m_hitPoints;
	float m_damageSinceMove;
};

class EnemyData
{
public:
#ifdef TR7
	char pad1[5040];
#else
	char pad1[5280];
#endif

	EnemyHealth m_health;
};