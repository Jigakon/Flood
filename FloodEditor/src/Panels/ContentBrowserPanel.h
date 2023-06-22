#ifndef CONTENTBROWSERPANEL__H
#define CONTENTBROWSERPANEL__H

#include <filesystem>
#include "Flood/Renderer/Texture.h"

namespace Flood
{
	class ContentBrowserPanel
	{
	public:
		ContentBrowserPanel();

		void OnImGuiRender();

	private:
		std::filesystem::path m_CurrentDirectory;

		std::unordered_map<std::string, Ref<Texture2D>> m_Icons;

		Ref<Texture2D> m_Arrow;

		static const std::unordered_map<std::string, std::string> s_Formats;
	};
}

#endif //!CONTENTBROWSERPANEL__H
