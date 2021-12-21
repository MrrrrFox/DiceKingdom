#include "pch.h"
#include "ImGuiLayout.h"
#include "colors.h"

void ImGuiLayout::initImGui(sf::RenderWindow* _window, unsigned int width, unsigned int height, DiceKingdom* _DK)
{
	ImGui::SFML::Init(*_window);
	WIDTH = width;
	HEIGHT = height;
	DK = _DK;

	ImGuiStyle& style = ImGui::GetStyle();
	style.WindowTitleAlign = ImVec2(0.5f, 0.5f);
	style.Colors[ImGuiCol_TitleBg] = gray;
	style.Colors[ImGuiCol_TitleBgActive] = gray;


	DK->add_dice("Idle", Dice(4), 12);
	DK->add_dice("Idle", Dice(4, 2), 3);
	DK->add_dice("Idle", Dice(6), 5);
	DK->add_dice("Idle", Dice(20));

	DK->add_dice("Lumber Camp", Dice(4), 12);

	DK->add_dice("Paint Rig", Dice(8), 2);
}

void ImGuiLayout::terminateImGui() const
{
	ImGui::SFML::Shutdown();
}

void ImGuiLayout::drawMenuInfo()
{
	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 20.0f);
	ImGui::PushStyleColor(ImGuiCol_WindowBg, black_70);

	ImGui::SetNextWindowPos(ImVec2(0.3f * static_cast<float> (WIDTH), 0.9f * static_cast<float> (HEIGHT)));
	ImGui::SetNextWindowSize(ImVec2(0.4f * static_cast<float> (WIDTH), 20.0f));
	ImGui::Begin("Title Bar", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove);
	ImGui::SetCursorPosX((ImGui::GetWindowSize().x - ImGui::CalcTextSize(menuText.data()).x) * 0.5f);
	ImGui::Text(menuText.data());
	ImGui::End();

	ImGui::PopStyleColor();
	ImGui::PopStyleVar();
}

void ImGuiLayout::drawMaterialsBar(const std::vector<Material*>& materials) const
{
	auto size = static_cast<int> (materials.size());

	ImGui::SetNextWindowPos(ImVec2(0.0f, 0.0f));
	ImGui::SetNextWindowSize(ImVec2(static_cast<float> (WIDTH), 20.0f));
	ImGui::Begin("Materials' bar", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove);

	ImGui::Columns(size, "Materials' bar", false);
	for(int i = 0; i < size; ++i)
		ImGui::SetColumnWidth(i, 100.0f);

	for(auto material : materials)
	{
		ImGui::Text((material->name + ": " + std::to_string(material->quantity)).data());
		ImGui::NextColumn();
	}

	ImGui::End();
}

void ImGuiLayout::drawPlacePanel(GameView place_enum)
{
	std::string placeName = convert_enum_to_place_name(place_enum);
	int btn_id = 0;

	std::set<DiceWithoutHP, DiceCompareWithoutHP> dices_combined = DK->return_dice_array_combined_with_idle(placeName);
	std::map<DiceWithoutHP, int, DiceCompareWithoutHP> dices_in_place = DK->return_dice_array(placeName);
	std::map<DiceWithoutHP, int, DiceCompareWithoutHP> dices_in_idle = DK->return_dice_array("Idle");

	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 10.0f);
	ImGui::PushStyleColor(ImGuiCol_Button, gray);
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, lightgray);

	ImGui::SetNextWindowPos(ImVec2(static_cast<float> (WIDTH) / 4.0f, static_cast<float> (HEIGHT) / 4.0f));
	ImGui::SetNextWindowSize(ImVec2(static_cast<float> (WIDTH) / 2.0f, static_cast<float> (HEIGHT) / 2.0f));
	ImGui::Begin(placeName.data(), nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove);

	if(ImGui::BeginTable(placeName.data(), 4, ImGuiTableFlags_BordersInnerH))
	{
		ImGui::TableSetupColumn("dice type", ImGuiTableColumnFlags_WidthFixed, panels_widths[0]);
		ImGui::TableSetupColumn("decreasing values", ImGuiTableColumnFlags_WidthFixed, panels_widths[1]);
		ImGui::TableSetupColumn("quantity", ImGuiTableColumnFlags_WidthFixed, panels_widths[2]);
		ImGui::TableSetupColumn("increasing values", ImGuiTableColumnFlags_WidthFixed, panels_widths[3]);

		bool idle = (place_enum == GameView::KINGDOM_IDLE);
		for(auto dice_type = dices_combined.begin(); dice_type != dices_combined.end(); ++dice_type)
		{
			drawDiceRow(*dice_type, dices_in_place, dices_in_idle, idle, placeName, btn_id);
		}
		ImGui::EndTable();
	}
	ImGui::End();

	ImGui::PopStyleColor(2);
	ImGui::PopStyleVar();
}

void ImGuiLayout::drawDiceRow(DiceWithoutHP dice_type, std::map<DiceWithoutHP, int, DiceCompareWithoutHP>& dices_in_place, std::map<DiceWithoutHP, int, DiceCompareWithoutHP>& dices_in_idle, bool idle, const std::string& placeName, int& btn_id)
{
	std::string dice_type_name;
	int dice_type_quantity;
	ImVec2 cursor_pos;
	float offset_y;

	dice_type_name = "D" + std::to_string(dice_type.faces);
	dice_type_quantity = dices_in_place[dice_type];

	ImGui::TableNextRow();

	ImGui::TableSetColumnIndex(0);
	cursor_pos = ImGui::GetCursorPos();
	offset_y = (idle ? 1.0f : 3.5f);
	ImGui::SetCursorPos(ImVec2(cursor_pos.x + (panels_widths[0] - ImGui::CalcTextSize(dice_type_name.data()).x) / 2.0f, cursor_pos.y + offset_y));
	ImGui::Text(dice_type_name.data());


	ImGui::TableSetColumnIndex(1);
	cursor_pos = ImGui::GetCursorPos();
	ImGui::SetCursorPos(ImVec2(cursor_pos.x + panels_widths[1] - 15.0f, cursor_pos.y));
	if(!idle && dices_in_place[dice_type] > 0)
	{
		ImGui::PushID(btn_id);
		btn_id++;
		if(ImGui::Button("-"))
		{
			Dice d_tmp = DK->find_most_damaged_dice(dice_type, placeName);
			DK->remove_dice(placeName, d_tmp);
			DK->add_dice("Idle", d_tmp);
		}
		ImGui::PopID();
	}


	ImGui::TableSetColumnIndex(2);
	cursor_pos = ImGui::GetCursorPos();
	if(dice_type_quantity)
		offset_y = (idle ? 0.0f : -1.0f);
	else
		offset_y = 2.0f;
	ImGui::SetCursorPos(ImVec2(cursor_pos.x + (panels_widths[2] - ImGui::CalcTextSize(std::to_string(dice_type_quantity).data()).x) / 2.0f, cursor_pos.y + offset_y));
	ImGui::Text(std::to_string(dice_type_quantity).data());

	if(!idle && dices_in_idle[dice_type] > 0)
	{
		ImGui::TableSetColumnIndex(3);
		ImGui::PushID(btn_id);
		btn_id++;
		if(ImGui::Button("+"))
		{
			Dice d_tmp = DK->find_least_damaged_dice(dice_type, "Idle");
			DK->add_dice(placeName, d_tmp);
			DK->remove_dice("Idle", d_tmp);
		}
		ImGui::PopID();
	}
}
