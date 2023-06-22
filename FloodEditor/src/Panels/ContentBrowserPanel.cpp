#include "flpch.h"
#include "ContentBrowserPanel.h"

#include <imgui/imgui.h>

namespace Flood
{
	const std::filesystem::path g_AssetPath = "assets";

	const std::unordered_map<std::string, std::string> ContentBrowserPanel::s_Formats = {
			{".png", "Image"}, {".jpg", "Image"},
			{".c", "Text"},{".cpp", "Text"},{".h", "Text"},{".hpp", "Text"},{".txt", "Text"},
			{".flood", "Level"},
	};

	ContentBrowserPanel::ContentBrowserPanel() : m_CurrentDirectory(g_AssetPath)
	{
		m_Icons["Folder"] = Texture2D::Create("Resources/Icons/folder.png");
		m_Icons["Unknown"] = Texture2D::Create("Resources/Icons/unknownFile.png");
		m_Icons["Text"] = Texture2D::Create("Resources/Icons/textFile.png");
		m_Icons["Image"] = Texture2D::Create("Resources/Icons/imageFile.png");
		m_Icons["Level"] = Texture2D::Create("Resources/Icons/levelFile.png");

		m_Arrow = Texture2D::Create("Resources/Arrow.png");
	}

	void ContentBrowserPanel::OnImGuiRender()
	{
		ImGui::Begin("Content Browser");

		ImGuiStyle& style = ImGui::GetStyle();
		auto lastFrameRounding = style.FrameRounding;
		style.FrameRounding = 5.f;

		ImGui::BeginDisabled(m_CurrentDirectory == std::filesystem::path(g_AssetPath));
		if (ImGui::ImageButton((ImTextureID)m_Arrow->GetRendererID(), {16, 16}))
			m_CurrentDirectory = m_CurrentDirectory.parent_path();
		ImGui::EndDisabled();

		static float padding = 16.f;
		static float thumbnailSize = 128.f;
		float cellSize = padding + thumbnailSize;

		float panelWidth = ImGui::GetContentRegionAvail().x;
		int columnCount = (int)panelWidth / cellSize;
		if (columnCount < 1)
			columnCount = 1;
		ImGui::Columns(columnCount, 0, false);
		
		for (auto& it : std::filesystem::directory_iterator(m_CurrentDirectory))
		{
			const auto& path = it.path();

			auto relativePath = std::filesystem::relative(path, g_AssetPath);
			std::string filenameString = relativePath.string();
			ImGui::PushID(filenameString.c_str());

			bool isDirectory = it.is_directory();
			uint32_t icon;
			if (isDirectory)
				icon = m_Icons["Folder"]->GetRendererID();
			else
			{
				auto extension = relativePath.extension().string();
				auto image = ContentBrowserPanel::s_Formats.find(extension);
				if (image != ContentBrowserPanel::s_Formats.end())
					icon = m_Icons[image->second]->GetRendererID();
				else
					icon = m_Icons["Unknown"]->GetRendererID();
			}
			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
			ImGui::ImageButton((ImTextureID)icon, { thumbnailSize, thumbnailSize }, {0, 1}, {1, 0});

			if (ImGui::BeginDragDropSource())
			{
				const wchar_t* itemPath = relativePath.c_str();
				ImGui::SetDragDropPayload("CONTENT_BROWSER_ITEM", itemPath, (wcslen(itemPath) + 1)* sizeof(wchar_t), ImGuiCond_Once);
				ImGui::SetTooltip("Move");
				ImGui::EndDragDropSource();
			}

			ImGui::PopStyleColor();
			if(ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
			{
				if (isDirectory)
				{
					m_CurrentDirectory /= path.filename();
				}
			}
			ImGui::TextWrapped(path.stem().string().c_str());

			ImGui::NextColumn();

			ImGui::PopID();
		}
		style.FrameRounding = lastFrameRounding;

		ImGui::Columns(1);

		ImGui::SliderFloat("Thumbnail Size", &thumbnailSize, 16.f, 256.f);
		ImGui::SliderFloat("Paddings", &padding, 0.f, 32.f);

		ImGui::End();
	}
}
