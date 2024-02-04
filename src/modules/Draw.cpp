#ifndef TR8

#include <string>
#include <imgui.h>

#include "Draw.h"
#include "render/Font.h"
#include "render/Draw.h"
#include "game/Game.h"
#include "instance/Instances.h"
#include "instance/Enemy.h"

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
	if (m_drawInstances)
	{
		ImGui::Begin("Draw options");

		// Filter
		ImGui::InputText("Filter", m_filter, sizeof(m_filter));

		// Options
		ImGui::Checkbox("Draw intro", &m_drawIntro);
		ImGui::Checkbox("Draw family", &m_drawFamily);
		ImGui::Checkbox("Draw animations", &m_drawAnimation);
		ImGui::Checkbox("Draw health", &m_drawHealth);

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
}

void Draw::DrawEnemyRoute(Instance* instance)
{
	auto data = (ObjectData*)instance->data;

	// Check if the instance is an enemy
	if (!data || data->family != 0xDAF0)
	{
		return;
	}
}

void Draw::DrawMarkUp()
{
}

void Draw::DrawCollision(Level* level)
{
}

void Draw::DrawCollision(TerrainGroup* terrainGroup)
{
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

		TRANS_RotTransPersVectorf(&position, &position);

		// Check if the portal is on screen
		if (position.z > 16.f)
		{
			// Draw the portal destination
			font->SetCursor(position.x, position.y);
			font->PrintCentered("Portal to %s", portal->tolevelname);

			// Draw the portal bounds
			DrawPlane(&portal->min, &portal->max, RGBA(0, 0, 255, 10));
		}
	}
}

void Draw::DrawSignals(Level* level)
{
}

#endif