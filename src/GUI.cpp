//
// Created by rodrigo on 29/04/2026.
//
#include "glad/glad.h"
#include "../include/GUI.h"


#include <imgui.h>
#include "imgui_impl_opengl3.h"
// Face button

void GUI::showMainWindowGUI()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	const ImGuiViewport* viewport = ImGui::GetMainViewport();

	if (!isPanelVisible) {
		// Show only a small button in the top-left corner to re-open the panel
		ImGui::SetNextWindowPos(ImVec2(viewport->WorkPos.x, viewport->WorkPos.y), ImGuiCond_Always);
		ImGui::SetNextWindowSize(ImVec2(0, 0)); // auto-fit to content
		ImGui::Begin("##menu_toggle", nullptr,
			ImGuiWindowFlags_NoMove |
			ImGuiWindowFlags_NoResize |
			ImGuiWindowFlags_NoCollapse |
			ImGuiWindowFlags_NoTitleBar |
			ImGuiWindowFlags_AlwaysAutoResize);
		if (ImGui::Button("Menu")) {
			isPanelVisible = true;
		}
		ImGui::End();
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		return;
	}

	// Pin the panel to the left side of the screen like a fixed menu
	ImGui::SetNextWindowPos(ImVec2(viewport->WorkPos.x, viewport->WorkPos.y), ImGuiCond_Always);
	ImGui::SetNextWindowSize(ImVec2(250, viewport->WorkSize.y), ImGuiCond_Always);
	ImGui::Begin("Editing Tools", nullptr,
		ImGuiWindowFlags_NoMove |
		ImGuiWindowFlags_NoResize |
		ImGuiWindowFlags_NoCollapse);

	// Hide button at the top of the panel
	if (ImGui::Button("Hide")) {
		isPanelVisible = false;
	}
	ImGui::Separator();

	// --- Camera Type ---
	bool isGimball = (currentCameraMode == GIMBALL);
	bool isFree    = (currentCameraMode == FREE);

	ImGui::Text("Camera:");
	ImGui::SameLine();

	// Gimball button
	ImGui::PushStyleColor(ImGuiCol_Button,        isGimball ? ImVec4(0.2f, 0.7f, 0.2f, 1.0f) : ImGui::GetStyleColorVec4(ImGuiCol_Button));
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, isGimball ? ImVec4(0.3f, 0.9f, 0.3f, 1.0f) : ImGui::GetStyleColorVec4(ImGuiCol_ButtonHovered));
	ImGui::PushStyleColor(ImGuiCol_ButtonActive,  isGimball ? ImVec4(0.1f, 0.5f, 0.1f, 1.0f) : ImGui::GetStyleColorVec4(ImGuiCol_ButtonActive));
	if (ImGui::Button("Gimball##cam") && !isGimball)
		setCameraMode(GIMBALL);
	ImGui::PopStyleColor(3);

	ImGui::SameLine();

	// Free button
	ImGui::PushStyleColor(ImGuiCol_Button,        isFree ? ImVec4(0.2f, 0.7f, 0.2f, 1.0f) : ImGui::GetStyleColorVec4(ImGuiCol_Button));
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, isFree ? ImVec4(0.3f, 0.9f, 0.3f, 1.0f) : ImGui::GetStyleColorVec4(ImGuiCol_ButtonHovered));
	ImGui::PushStyleColor(ImGuiCol_ButtonActive,  isFree ? ImVec4(0.1f, 0.5f, 0.1f, 1.0f) : ImGui::GetStyleColorVec4(ImGuiCol_ButtonActive));
	if (ImGui::Button("Free##cam") && !isFree)
		setCameraMode(FREE);
	ImGui::PopStyleColor(3);

	ImGui::Separator();

	bool faceActive = main_state.isFaceSelectionActive;
	bool vertexActive = main_state.isVertexSelectionActive;
	bool edgeActive = main_state.isEdgeSelectionActive;

	// --- Face Selection ---
	ImGui::PushStyleColor(ImGuiCol_Button, faceActive ? ImVec4(0.2f, 0.7f, 0.2f, 1.0f) : ImGui::GetStyleColorVec4(ImGuiCol_Button));
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, faceActive ? ImVec4(0.3f, 0.9f, 0.3f, 1.0f) : ImGui::GetStyleColorVec4(ImGuiCol_ButtonHovered));
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, faceActive ? ImVec4(0.1f, 0.5f, 0.1f, 1.0f) : ImGui::GetStyleColorVec4(ImGuiCol_ButtonActive));

	ImGui::AlignTextToFramePadding();
	ImGui::Text("Face Selection:");
	ImGui::SameLine();

	if (ImGui::Button(faceActive ? "ON##face" : "OFF##face"))
	{

		setState(FACE_EDITING);
	}
	ImGui::PopStyleColor(3);

	ImGui::SameLine(); // Move to the right of the ON/OFF button
	if (ImGui::Button("Clear##face"))
	{
		reset_selection_buffer_flag=FACE_EDITING;//set the flag to reset the selection buffer on the GPU in the main loop
	}


	// --- Vertex Selection ---
	ImGui::PushStyleColor(ImGuiCol_Button, vertexActive ? ImVec4(0.2f, 0.7f, 0.2f, 1.0f) : ImGui::GetStyleColorVec4(ImGuiCol_Button));
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, vertexActive ? ImVec4(0.3f, 0.9f, 0.3f, 1.0f) : ImGui::GetStyleColorVec4(ImGuiCol_ButtonHovered));
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, vertexActive ? ImVec4(0.1f, 0.5f, 0.1f, 1.0f) : ImGui::GetStyleColorVec4(ImGuiCol_ButtonActive));

	ImGui::AlignTextToFramePadding();
	ImGui::Text("Vertex Selection:");
	ImGui::SameLine();

	if (ImGui::Button(vertexActive ? "ON##vertex" : "OFF##vertex"))
	{
		setState(VERTEX_EDITING);
	}
	ImGui::PopStyleColor(3);

	ImGui::SameLine();
	if (ImGui::Button("Clear##vertex"))
	{	
		
		reset_selection_buffer_flag=VERTEX_EDITING;//set the flag to reset the selection buffer on the GPU in the main loop
	}


	// --- Edge Selection ---
	ImGui::PushStyleColor(ImGuiCol_Button, edgeActive ? ImVec4(0.2f, 0.7f, 0.2f, 1.0f) : ImGui::GetStyleColorVec4(ImGuiCol_Button));
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, edgeActive ? ImVec4(0.3f, 0.9f, 0.3f, 1.0f) : ImGui::GetStyleColorVec4(ImGuiCol_ButtonHovered));
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, edgeActive ? ImVec4(0.1f, 0.5f, 0.1f, 1.0f) : ImGui::GetStyleColorVec4(ImGuiCol_ButtonActive));

	ImGui::AlignTextToFramePadding();
	ImGui::Text("Edge Selection:");
	ImGui::SameLine();

	if (ImGui::Button(edgeActive ? "ON##edge" : "OFF##edge"))
	{
		setState(EDGE_EDITING);
	}
	ImGui::PopStyleColor(3);

	ImGui::SameLine();
	if (ImGui::Button("Clear##edge"))
	{
		reset_selection_buffer_flag=EDGE_EDITING;//set the flag to reset the selection buffer on the GPU in the main loop
	}
	ImGui::End();
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}