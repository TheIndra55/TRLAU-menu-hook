#pragma once

class EnemyProcess
{
public:
	char pad1[20];
};

#pragma pack(push, 1)
class EnemyRouting : public EnemyProcess
{
public:
	class PolyLinePath
	{
	public:
		cdc::Vector3 m_path[16];
		cdc::Vector3 m_dest;

		char pad1[148];

		int m_index;
		int m_pathlength;

		char pad2[4];
	};

	char pad1[124];

	PolyLinePath m_plPath;

	char pad2[272];
};
#pragma pack(pop)

class EnemyHealth
{
public:
	float m_hitPoints;
	float m_damageSinceMove;
};

#pragma pack(push, 1)
class EnemyData
{
public:
#ifdef TR7
	char pad1[4192];
#else
	char pad1[4432];
#endif

	EnemyRouting m_routing;
	EnemyHealth m_health;
};
#pragma pack(pop)