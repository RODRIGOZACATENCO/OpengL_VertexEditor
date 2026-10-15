#include "glad/glad.h"
#include "../headers/GUI.h"
#include <imgui.h>
#include "imgui_impl_opengl3.h"

void GUI::showMainWindowGUI() {
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	const ImGuiViewport* viewport = ImGui::GetMainViewport();

	ImGui::SetNextWindowPos(ImVec2(viewport->WorkPos.x, viewport->WorkPos.y), ImGuiCond_Always);
	ImGui::SetNextWindowSize(ImVec2(250.0f, viewport->WorkSize.y), ImGuiCond_Always);
	ImGui::SetNextWindowBgAlpha(1.0f);
	ImGui::Begin("Editing Tools", nullptr,
	             ImGuiWindowFlags_NoMove |
	             ImGuiWindowFlags_NoResize |
	             ImGuiWindowFlags_NoCollapse);

	bool isGimball = (currentCameraMode == GIMBALL);
	bool isFree = (currentCameraMode == FREE);

	ImGui::Text("Camera:");
	ImGui::SameLine();

	ImGui::PushStyleColor(ImGuiCol_Button,
	                      isGimball ? ImVec4(0.2f, 0.7f, 0.2f, 1.0f) : ImGui::GetStyleColorVec4(ImGuiCol_Button));
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered,
	                      isGimball
		                      ? ImVec4(0.3f, 0.9f, 0.3f, 1.0f)
		                      : ImGui::GetStyleColorVec4(ImGuiCol_ButtonHovered));
	ImGui::PushStyleColor(ImGuiCol_ButtonActive,
	                      isGimball ? ImVec4(0.1f, 0.5f, 0.1f, 1.0f) : ImGui::GetStyleColorVec4(ImGuiCol_ButtonActive));
	if (ImGui::Button("Gimball##cam") && !isGimball)
		setCameraMode(GIMBALL);
	ImGui::PopStyleColor(3);

	ImGui::SameLine();

	ImGui::PushStyleColor(ImGuiCol_Button,
	                      isFree ? ImVec4(0.2f, 0.7f, 0.2f, 1.0f) : ImGui::GetStyleColorVec4(ImGuiCol_Button));
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered,
	                      isFree ? ImVec4(0.3f, 0.9f, 0.3f, 1.0f) : ImGui::GetStyleColorVec4(ImGuiCol_ButtonHovered));
	ImGui::PushStyleColor(ImGuiCol_ButtonActive,
	                      isFree ? ImVec4(0.1f, 0.5f, 0.1f, 1.0f) : ImGui::GetStyleColorVec4(ImGuiCol_ButtonActive));
	if (ImGui::Button("Free##cam") && !isFree)
		setCameraMode(FREE);
	ImGui::PopStyleColor(3);

	ImGui::Separator();

	bool faceActive = main_state.isFaceSelectionActive;
	bool vertexActive = main_state.isVertexSelectionActive;
	bool edgeActive = main_state.isEdgeSelectionActive;

	ImGui::PushStyleColor(ImGuiCol_Button,
	                      faceActive ? ImVec4(0.2f, 0.7f, 0.2f, 1.0f) : ImGui::GetStyleColorVec4(ImGuiCol_Button));
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered,
	                      faceActive
		                      ? ImVec4(0.3f, 0.9f, 0.3f, 1.0f)
		                      : ImGui::GetStyleColorVec4(ImGuiCol_ButtonHovered));
	ImGui::PushStyleColor(ImGuiCol_ButtonActive,
	                      faceActive
		                      ? ImVec4(0.1f, 0.5f, 0.1f, 1.0f)
		                      : ImGui::GetStyleColorVec4(ImGuiCol_ButtonActive));

	ImGui::AlignTextToFramePadding();
	ImGui::Text("Face Selection:");
	ImGui::SameLine();

	if (ImGui::Button(faceActive ? "ON##face" : "OFF##face")) { setState(FACE_EDITING); }
	ImGui::PopStyleColor(3);

	ImGui::SameLine();
	if (ImGui::Button("Clear##face")) {
		reset_selection_buffer_flag = FACE_EDITING;
	}

	ImGui::PushStyleColor(ImGuiCol_Button,
	                      vertexActive ? ImVec4(0.2f, 0.7f, 0.2f, 1.0f) : ImGui::GetStyleColorVec4(ImGuiCol_Button));
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered,
	                      vertexActive
		                      ? ImVec4(0.3f, 0.9f, 0.3f, 1.0f)
		                      : ImGui::GetStyleColorVec4(ImGuiCol_ButtonHovered));
	ImGui::PushStyleColor(ImGuiCol_ButtonActive,
	                      vertexActive
		                      ? ImVec4(0.1f, 0.5f, 0.1f, 1.0f)
		                      : ImGui::GetStyleColorVec4(ImGuiCol_ButtonActive));

	ImGui::AlignTextToFramePadding();
	ImGui::Text("Vertex Selection:");
	ImGui::SameLine();

	if (ImGui::Button(vertexActive ? "ON##vertex" : "OFF##vertex")) { setState(VERTEX_EDITING); }
	ImGui::PopStyleColor(3);

	ImGui::SameLine();
	if (ImGui::Button("Clear##vertex")) {
		reset_selection_buffer_flag = VERTEX_EDITING;
	}

	ImGui::PushStyleColor(ImGuiCol_Button,
	                      edgeActive ? ImVec4(0.2f, 0.7f, 0.2f, 1.0f) : ImGui::GetStyleColorVec4(ImGuiCol_Button));
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered,
	                      edgeActive
		                      ? ImVec4(0.3f, 0.9f, 0.3f, 1.0f)
		                      : ImGui::GetStyleColorVec4(ImGuiCol_ButtonHovered));
	ImGui::PushStyleColor(ImGuiCol_ButtonActive,
	                      edgeActive
		                      ? ImVec4(0.1f, 0.5f, 0.1f, 1.0f)
		                      : ImGui::GetStyleColorVec4(ImGuiCol_ButtonActive));

	ImGui::AlignTextToFramePadding();
	ImGui::Text("Edge Selection:");
	ImGui::SameLine();

	if (ImGui::Button(edgeActive ? "ON##edge" : "OFF##edge")) { setState(EDGE_EDITING); }
	ImGui::PopStyleColor(3);

	ImGui::SameLine();
	if (ImGui::Button("Clear##edge")) {
		reset_selection_buffer_flag = EDGE_EDITING;
	}
	ImGui::End();
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}
