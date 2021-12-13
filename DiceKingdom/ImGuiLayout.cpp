#include "pch.h"
#include "ImGuiLayout.h"

void ImGuiLayout::drawMaterialsBar(std::map<std::string, int>)
{
	ImGui::SetNextWindowPos(ImVec2(0.0f, 0.0f));
	//ImGui::SetNextWindowSize(ImVec2(WIDTH, 15.0f));
	ImGui::Begin("Materials' bar", NULL, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove);
	if (ImGui::BeginTable("Materials' bar table", 4, ImGuiTableFlags_BordersInnerH))
	{
		ImGui::TableSetupColumn("dice type", ImGuiTableColumnFlags_WidthFixed, 50.0f);
		ImGui::TableSetupColumn("decreasing values", ImGuiTableColumnFlags_WidthFixed, 50.0f);

		ImGui::TableNextRow();

		//for (int row = 0; row < 2; ++row)
		//{
		//	ImGui::TableSetColumnIndex(0);
		//	c_pos = ImGui::GetCursorPos();
		//	ImGui::SetCursorPos(ImVec2(c_pos.x + (col_widths[0] - ImGui::CalcTextSize(texts[row].data()).x) / 2.0f, c_pos.y + padding_y));
		//	ImGui::Text(texts[row].data());

		//	ImGui::TableSetColumnIndex(1);
		//	c_pos = ImGui::GetCursorPos();
		//	ImGui::SetCursorPos(ImVec2(c_pos.x + col_widths[1] - 15.0f, c_pos.y));
		//	ImGui::PushID(btn_id++);
		//	if (ImGui::Button("-"))
		//		std::cout << "-" + texts[row] << std::endl;
		//	ImGui::PopID();
		//}

		ImGui::EndTable();
	}
	ImGui::End();
}