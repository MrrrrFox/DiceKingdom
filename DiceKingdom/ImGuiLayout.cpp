#include "pch.h"
#include "ImGuiLayout.h"
#include "colors.h"

void ImGuiLayout::initImGui(sf::RenderWindow * _window, int width, int height)
{
	ImGui::SFML::Init(*_window);
	WIDTH = width;
	HEIGHT = height;

	ImGuiStyle& style = ImGui::GetStyle();
	style.WindowTitleAlign = ImVec2(0.5f, 0.5f);
	style.Colors[ImGuiCol_TitleBg] = gray;
	style.Colors[ImGuiCol_TitleBgActive] = gray;
}

void ImGuiLayout::terminateImGui()
{
	ImGui::SFML::Shutdown();
}

void ImGuiLayout::drawMenuInfo()
{
	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 20.0f);
	ImGui::PushStyleColor(ImGuiCol_WindowBg, black_70);

	ImGui::SetNextWindowPos(ImVec2(0.3f * WIDTH, 0.9f * HEIGHT));
	ImGui::SetNextWindowSize(ImVec2(0.4f * WIDTH, 20.0f));
	ImGui::Begin("Title Bar", NULL, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove);
	ImGui::SetCursorPosX((ImGui::GetWindowSize().x - ImGui::CalcTextSize(menuText.data()).x) * 0.5f);
	ImGui::Text(menuText.data());
	ImGui::End();

	ImGui::PopStyleColor();
	ImGui::PopStyleVar();
}

void ImGuiLayout::drawMaterialsBar(std::unordered_map<std::string, int> materials)
{
	int size = (int)materials.size();

	ImGui::SetNextWindowPos(ImVec2(0.0f, 0.0f));
	ImGui::Begin("Materials' bar", NULL, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove);
	
	ImGui::Columns(size, "Materials' bar", false);
	for (int i = 0; i < size; ++i)
		ImGui::SetColumnWidth(i, 100.0f);

	for (auto material : materials)
	{
		ImGui::Text((material.first + ": " + std::to_string(material.second)).data());
		ImGui::NextColumn();
	}

	ImGui::End();
}

void ImGuiLayout::drawPlacePanel(Place & place)
{
	int btn_id = 0;
	std::map<DiceWithoutHP, int, DiceCompareWithoutHP> dices = place.return_dice_array();

	ImVec2 cursor_pos;
	
	std::string dice_type_name;
	int dice_type_quantity;

	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 10.0f);
	ImGui::PushStyleColor(ImGuiCol_Button, gray);
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, lightgray);

	ImGui::SetNextWindowPos(ImVec2(WIDTH / 4.0f, HEIGHT / 4.0f));
	ImGui::SetNextWindowSize(ImVec2(WIDTH / 2.0f, HEIGHT / 2.0f));
	ImGui::Begin(place.get_name().data(), NULL, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove);

	if (ImGui::BeginTable(place.get_name().data(), 4, ImGuiTableFlags_BordersInnerH))
	{
		ImGui::TableSetupColumn("dice type", ImGuiTableColumnFlags_WidthFixed, panels_widths[0]);
		ImGui::TableSetupColumn("decreasing values", ImGuiTableColumnFlags_WidthFixed, panels_widths[1]);
		ImGui::TableSetupColumn("quantity", ImGuiTableColumnFlags_WidthFixed, panels_widths[2]);
		ImGui::TableSetupColumn("increasing values", ImGuiTableColumnFlags_WidthFixed, panels_widths[3]);

		for (auto dice_type = dices.begin(); dice_type != dices.end(); ++dice_type)
		{
			dice_type_name = "D" + std::to_string(dice_type->first.faces);
			dice_type_quantity = dice_type->second;

			ImGui::TableNextRow();

			ImGui::TableSetColumnIndex(0);
			cursor_pos = ImGui::GetCursorPos();
			ImGui::SetCursorPos(ImVec2(cursor_pos.x + (panels_widths[0] - ImGui::CalcTextSize(dice_type_name.data()).x) / 2.0f, cursor_pos.y + panels_padding_y));
			ImGui::Text(dice_type_name.data());

			ImGui::TableSetColumnIndex(1);
			cursor_pos = ImGui::GetCursorPos();
			ImGui::SetCursorPos(ImVec2(cursor_pos.x + panels_widths[1] - 15.0f, cursor_pos.y));
			ImGui::PushID(btn_id++);
			if (ImGui::Button("-"))
			{
				place.remove(Dice(dice_type->first.faces));
			}
			ImGui::PopID();

			ImGui::TableSetColumnIndex(2);
			cursor_pos = ImGui::GetCursorPos();
			ImGui::SetCursorPos(ImVec2(cursor_pos.x + (panels_widths[2] - ImGui::CalcTextSize(std::to_string(dice_type_quantity).data()).x) / 2.0f, cursor_pos.y));
			ImGui::Text(std::to_string(dice_type_quantity).data());

			ImGui::TableSetColumnIndex(3);
			ImGui::PushID(btn_id++);
			if (ImGui::Button("+"))
			{
				place.add(Dice(dice_type->first.faces));
			}
			ImGui::PopID();
		}
		ImGui::EndTable();
	}
	ImGui::End();

	ImGui::PopStyleColor(2);
	ImGui::PopStyleVar();
}