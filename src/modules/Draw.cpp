#include <utility>
#include <string>
#include <imgui.h>

#include "Draw.h"
#include "render/Font.h"
#include "render/Draw.h"
#include "game/Game.h"
#include "instance/Instances.h"
#include "instance/Enemy.h"
#include "level/Markup.h"
#include "level/Trigger.h"

#include "cdc/math/Math.h"

template <typename T>
static inline cdc::Vector3 GetVertice(unsigned int vertice, Mesh* mesh, cdc::Vector* offset)
{
	auto vertex = &((T*)mesh->m_vertices)[vertice];
	auto position = cdc::Vector3{ static_cast<float>(vertex->x), static_cast<float>(vertex->y), static_cast<float>(vertex->z) };

	position += offset;

	return position;
}

static inline cdc::Vector3 GetVertice(unsigned int vertice, Mesh* mesh, cdc::Vector* offset)
{
	if (mesh->m_vertexType == VERTEX_INT16) return GetVertice<MeshVertex16>(vertice, mesh, offset);
	return GetVertice<MeshVertex32>(vertice, mesh, offset);
}

static std::pair<unsigned int, const char*> s_mudFlags[]
{
	{ MUD_FLAG_INSTANCE, "INSTANCE | " },
	{ MUD_FLAG_WRAPS, "WRAPS | " },
	{ MUD_FLAG_PATTERN_INTERACT, "PATTERN_INTERACT | " },
	{ MUD_FLAG_ENEMY_JUMP_ACROSS, "ENEMY_JUMP_ACROSS | " },
	{ MUD_FLAG_ENEMY_JUMP_DOWN, "ENEMY_JUMP_DOWN | " },
	{ MUD_FLAG_ENEMY_JUMP_UP, "ENEMY_JUMP_UP | " },
	{ MUD_FLAG_ATTACHPOINTS, "ATTACHPOINTS | " },
	{ MUD_FLAG_PERCH, "PERCH | " },
	{ MUD_FLAG_ROPE, "ROPE | " },
	{ MUD_FLAG_WATER, "WATER | " },
	{ MUD_FLAG_CLOSEJUMPTARGET, "CLOSEJUMPTARGET | " },
	{ MUD_FLAG_LADDER, "LADDER | " },
	{ MUD_FLAG_MANTLEDOWN, "MANTLEDOWN | " },
	{ MUD_FLAG_ZIPLINE, "ZIPLINE | " },
	{ MUD_FLAG_HORIZPOLE, "HORIZPOLE | " },
	{ MUD_FLAG_VERTPOLE, "VERTPOLE | " },
	{ MUD_FLAG_WALLVERTPOLE, "WALLVERTPOLE | " },
	{ MUD_FLAG_ENEMYDROPDOWN, "ENEMYDROPDOWN | " },
	{ MUD_FLAG_JUMPLANDING, "JUMPLANDING | " },
	{ MUD_FLAG_LEDGEGRAB, "LEDGEGRAB | " },
	{ MUD_FLAG_DISABLED, "DISABLED | " }
};

void Draw::OnMenu()
{
	if (ImGui::BeginMenu("Draw"))
	{
		ImGui::MenuItem("Draw instances", nullptr, &m_drawInstances);
		ImGui::MenuItem("Draw markup", nullptr, &m_drawMarkUp);
		ImGui::MenuItem("Draw enemy route", nullptr, &m_drawEnemyRouting);
		ImGui::MenuItem("Draw collision", nullptr, &m_drawCollision);
		ImGui::MenuItem("Draw portals", nullptr, &m_drawPortals);
		ImGui::MenuItem("Draw signals", nullptr, &m_drawSignals);
		ImGui::MenuItem("Draw triggers", nullptr, &m_drawTriggers);

		ImGui::EndMenu();
	}
}

void Draw::OnFrame()
{
	auto gameTracker = Game::GetGameTracker();
	auto level = gameTracker->level;

	if (m_drawInstances || m_drawEnemyRouting)
	{
		DrawInstances();
	}

	if (m_drawMarkUp)
	{
		DrawMarkUp();
	}

	if (m_drawTriggers)
	{
		DrawTriggers();
	}

	if (level)
	{
		if (m_drawCollision)
		{
			DrawCollision(level);
		}

		if (m_drawPortals)
		{
			DrawPortals(level);
		}

		if (m_drawSignals)
		{
			DrawSignals(level);
		}
	}
}

void Draw::OnDraw()
{
	if (m_drawInstances || m_drawCollision)
	{
		ImGui::Begin("Draw options");

		if (ImGui::CollapsingHeader("Instance"))
		{
			// Filter
			ImGui::InputText("Filter", m_filter, sizeof(m_filter));

			// Options
			ImGui::Checkbox("Draw intro", &m_drawIntro);
			ImGui::Checkbox("Draw family", &m_drawFamily);
			ImGui::Checkbox("Draw animations", &m_drawAnimation);
			ImGui::Checkbox("Draw health", &m_drawHealth);
		}

		if (ImGui::CollapsingHeader("Collision"))
		{
			ImGui::Checkbox("Player collision", &m_drawPlayerCollision);
			ImGui::Checkbox("Enemy collision", &m_drawEnemyCollision);

			ImGui::InputInt("Terrain group", &m_terrainGroup);
		}

		ImGui::End();
	}
}

void Draw::DrawInstances()
{
	// Iterate through all instances
	Instances::Iterate([this](Instance* instance)
		{
			// Draw the instance info
			if (m_drawInstances)
			{
				DrawInstance(instance);
			}

			// Draw the route
			if (m_drawEnemyRouting)
			{
				DrawEnemyRoute(instance);
			}
		});
}

void Draw::DrawInstance(Instance* instance)
{
#ifndef TR8
	auto name = instance->object->name;
	auto data = (ObjectData*)instance->data;

	// Check filter
	if (strlen(m_filter) > 0 && strstr(name, m_filter) == 0)
	{
		return;
	}

	cdc::Vector3 position;
	TRANS_RotTransPersVectorf(&instance->position, &position);

	// Check if the instance position is on screen
	if (position.z > 16.f)
	{
		auto font = Font::GetMainFont();
		auto heigth = font->GetHeight();

		// Draw the instance name
		font->SetCursor(position.x, position.y);
		font->Print("%s", name);

		// Draw intro
		if (m_drawIntro)
		{
			position.y += heigth;

			font->SetCursor(position.x, position.y);
			font->Print("Intro: %d", instance->introUniqueID);
		}

		// Draw family
		if (m_drawFamily && data)
		{
			position.y += heigth;

			font->SetCursor(position.x, position.y);
			font->Print("Family: %d", data->family);
		}

		// Draw enemy health
		if (m_drawHealth && data && data->family == 0xDAF0)
		{
			position.y += heigth;

			auto extraData = (EnemyData*)instance->extraData;

			font->SetCursor(position.x, position.y);
			font->Print("Health: %g", extraData->m_health.m_hitPoints);
		}

		// Draw animations
		if (m_drawAnimation)
		{
			// Draw all allocated sections
			for (int section = 0; section < instance->animComponent->mAnimProcessor->mSectionsAllocated; section++)
			{
				// Go to the next line
				position.y += heigth;
				font->SetCursor(position.x, position.y);

				// Query the current animation
				auto currentAnim = G2EmulationInstanceQueryAnimation(instance, section);

				// Can be null for some reason
				if (instance->object->animList)
				{
					// (Section): (Anim index) (Anim ID)
					font->Print("Anim %d: %d (%X)", section, currentAnim, instance->object->animList[currentAnim].animationID);
				}
				else
				{
					font->Print("Anim %d: %d", section, currentAnim);
				}
			}
		}
	}
#endif
}

void Draw::DrawEnemyRoute(Instance* instance)
{
#ifndef TR8
	auto data = (ObjectData*)instance->data;

	// Check if the instance is an enemy
	if (!data || data->family != 0xDAF0)
	{
		return;
	}

	// Get the enemy routing process
	auto extraData = (EnemyData*)instance->extraData;
	auto path = extraData->m_routing.m_plPath;

	// Draw the path
	auto x = &path.m_path[0];
	for (int i = 0; i < path.m_pathlength - 1; i++)
	{
		auto y = &path.m_path[i + 1];

		DrawLine(x, y, RGB(255, 0, 0));

		x = y;
	}
#endif
}

void Draw::DrawMarkUp()
{
	auto manager = MarkupManager::GetInstance();
	auto font = Font::GetMainFont();

	// Get all markup
	for (auto node = manager->m_staticBoxList.next; node->next != nullptr; node = node->next)
	{
		auto box = (MarkUpBox*)node;
		auto markup = box->markup;

		if (!box->markup)
		{
			continue;
		}

		cdc::Vector3 position = { markup->pos[0], markup->pos[1], markup->pos[2] };

		// Add the instance position
		if (box->instance && (box->flags & MUD_FLAG_WATER) == 0)
		{
			position += &box->instance->position;
		}

#ifndef TR8
		TRANS_RotTransPersVectorf(&position, &position);

		// Check if the text is on screen
		if (position.z > 16.f)
		{
			font->SetCursor(position.x, position.y);
			font->PrintFormatted(FlagsToString(box->flags).c_str());
		}
#endif

		// Draw the poly line
		if (markup->polyLine)
		{
			cdc::Vector3 x, y;
			box->GetSegmentPos(&x, 0);

			for (int i = 0; i < box->markup->polyLine->numPoints - 1; i++)
			{
				box->GetSegmentPos(&y, i + 1);

				DrawLine(&x, &y, RGB(255, 0, 0));

				x = y;
			}
		}

		// Draw the bounding box for water and perch markup
		if (box->flags & (MUD_FLAG_WATER | MUD_FLAG_PERCH))
		{
			auto bbox = markup->bbox;

			auto min = cdc::Vector3{ markup->pos[0] + static_cast<float>(bbox[0]), markup->pos[1] + static_cast<float>(bbox[1]), markup->pos[2] + static_cast<float>(bbox[2]) };
			auto max = cdc::Vector3{ markup->pos[0] + static_cast<float>(bbox[3]), markup->pos[1] + static_cast<float>(bbox[4]), markup->pos[2] + static_cast<float>(bbox[5]) };

			DrawBoundingBox(&min, &max, RGB(255, 0, 0));
		}
	}
}

void Draw::DrawCollision(Level* level)
{
	auto terrain = level->terrain;

	// Draw the collision mesh for all terrain groups
	for (int i = 0; i < terrain->numTerrainGroups; i++)
	{
		auto terrainGroup = &terrain->terrainGroups[i];

		// Filter on terrain group
		if (m_terrainGroup >= 0 && m_terrainGroup != i)
		{
			continue;
		}

		// Filter on player/enemy collision
		auto flag = terrainGroup->flags & 0x4000;
		auto filter = (m_drawPlayerCollision && flag == 0) || (m_drawEnemyCollision && flag != 0);

		if (terrainGroup->mesh && filter)
		{
			DrawCollision(terrainGroup);
		}
	}
}

void Draw::DrawCollision(TerrainGroup* terrainGroup)
{
	auto mesh = terrainGroup->mesh;

	// Draw all mesh faces
	for (int i = 0; i < mesh->m_numFaces; i++)
	{
		auto face = &mesh->m_faces[i];

		// Get the position of every vertice in world coordinates
		auto x = GetVertice(face->i0, mesh, &mesh->m_position);
		auto y = GetVertice(face->i1, mesh, &mesh->m_position);
		auto z = GetVertice(face->i2, mesh, &mesh->m_position);

		// Draw the face
		auto color = terrainGroup->flags & 0x4000 ? RGBA(255, 0, 255, 10) : RGBA(0, 255, 0, 10);
		DrawTriangle(&x, &y, &z, color);

		// Draw the face outlines
		DrawLine(&x, &y, RGB(255, 0, 0));
		DrawLine(&y, &z, RGB(255, 0, 0));
		DrawLine(&z, &x, RGB(255, 0, 0));
	}
}

void Draw::DrawPortals(Level* level)
{
	auto terrain = level->terrain;
	auto font = Font::GetMainFont();

	for (int i = 0; i < terrain->numStreamUnitPortals; i++)
	{
		auto portal = &terrain->streamUnitPortals[i];
		auto position = portal->min;

		position += &portal->max;
		position /= 2;

#ifndef TR8
		TRANS_RotTransPersVectorf(&position, &position);

		// Check if the portal is on screen
		if (position.z > 16.f)
		{
			// Draw the portal destination
			font->SetCursor(position.x, position.y);
			font->PrintCentered("Portal to %s", portal->tolevelname);
		}
#endif

		// Draw the portal bounds
		DrawPlane(&portal->min, &portal->max, RGBA(0, 0, 255, 10));
	}
}

void Draw::DrawSignals(Level* level)
{
#ifndef TR8
	auto terrain = level->terrain;
	auto terrainGroup = terrain->signalTerrainGroup;

	// Make sure there is a signal mesh
	if (!terrainGroup || !terrainGroup->mesh)
	{
		return;
	}

	auto mesh = terrainGroup->mesh;

	// Draw all mesh faces
	for (int i = 0; i < mesh->m_numFaces; i++)
	{
		auto face = (SignalFace*)&mesh->m_faces[i];

		// Get the position of every vertice in world coordinates
		auto x = GetVertice<MeshVertex32>(face->i0, mesh, &mesh->m_position);
		auto y = GetVertice<MeshVertex32>(face->i1, mesh, &mesh->m_position);
		auto z = GetVertice<MeshVertex32>(face->i2, mesh, &mesh->m_position);

		// Draw the face
		DrawTriangle(&x, &y, &z, RGBA(255, 0, 0, 10));
	}
#endif
}

void Draw::DrawTriggers()
{
#ifdef TR8
	// Draw all trigger planes
	auto numPlanes = *(int*)0xDBA21C;
	auto planes = (NsTriggerPlaneBase**)0xDA61C8;

	for (int i = 0; i < numPlanes; i++)
	{
		auto plane = planes[i];
		auto instance = plane->m_instance;
		auto data = (IntroDataTrigger*)instance->intro->data;

		if (data)
		{
			auto extendX = cdc::Vector3{};
			extendX.x += data->LocalXExtent;
			extendX.z += data->LocalZExtent;

			auto extendY = cdc::Vector3{};
			extendY.x -= data->LocalXExtent;
			extendY.z -= data->LocalZExtent;

			cdc::Matrix mat;
			mat.Build_XYZOrder(&instance->rotation);
			mat.col3 = instance->position;

			auto x = cdc::Mul3x4(&mat, &extendX);
			auto y = cdc::Mul3x4(&mat, &extendY);

			DrawPlane(&x, &y, RGBA(255, 0, 0, 10));
		}
	}

	// Draw all trigger volumes
	auto numVolumes = *(int*)0xDBA220;
	auto volumes = (NsTriggerVolumeBase**)0xDB6BE0;

	for (int i = 0; i < numVolumes; i++)
	{
		auto volume = volumes[i];
		auto instance = volume->m_instance;
		auto data = (IntroDataTrigger*)instance->intro->data;

		if (data)
		{
			if(data->shape == TriggerShape_Box)
			{
				auto extendX = cdc::Vector3{};
				extendX.x += data->LocalXExtent;
				extendX.y += data->LocalYExtent;
				extendX.z += data->LocalZExtent;

				auto extendY = cdc::Vector3{};
				extendY.x -= data->LocalXExtent;
				extendY.y -= data->LocalYExtent;
				extendY.z -= data->LocalZExtent;

				auto extendZ = cdc::Vector3{};
				extendZ.x -= data->LocalXExtent;
				extendZ.y += data->LocalYExtent;

				auto extendW = cdc::Vector3{};
				extendW.x += data->LocalXExtent;
				extendW.y -= data->LocalYExtent;

				cdc::Matrix mat;
				mat.Build_XYZOrder(&instance->rotation);
				mat.col3 = instance->position;

				auto x = cdc::Mul3x4(&mat, &extendX);
				auto y = cdc::Mul3x4(&mat, &extendY);
				auto z = cdc::Mul3x4(&mat, &extendZ);
				auto w = cdc::Mul3x4(&mat, &extendW);

				DrawBoundingBox(&x, &y, &z, &w, RGB(255, 0, 0));
				DrawBox(&x, &y, &z, &w, RGBA(255, 0, 0, 10));
			}
			else if (data->shape == TriggerShape_Sphere)
			{
				auto x = instance->position;
				auto y = instance->position;

				auto radius = data->Radius;

				x.x += radius;
				x.y += radius;
				x.z += radius;

				y.x -= radius;
				y.y -= radius;
				y.z -= radius;

				DrawBoundingBox(&x, &y, RGB(255, 0, 0));
			}
		}
	}
#endif
}

std::string Draw::FlagsToString(unsigned int flags)
{
	std::string result;

	for (int i = 0; i < sizeof(s_mudFlags) / sizeof(s_mudFlags[0]); i++)
	{
		if (flags & s_mudFlags[i].first)
		{
			result += s_mudFlags[i].second;
		}
	}

	// Somewhat stolen from https://stackoverflow.com/a/37795988/9398242
	if (result.empty())
		result = "NONE";
	else
		result.erase(result.end() - 3, result.end());
	
	return result;
}