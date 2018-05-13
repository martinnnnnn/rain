#pragma once

#include <vector>
#include <filesystem>
#include <string>
#include <iostream>

namespace fs = std::experimental::filesystem::v1;
namespace rain
{
	struct PathNode
	{
		PathNode() :
			path(""),
			father(nullptr)
		{
			
		}

		void Print(std::string _tabs);
		void PrintToUI();

		std::string path;
		PathNode* father;
		std::vector<PathNode*> children;
	};



	class FileSystem
	{
	public:
		FileSystem() : m_rootNode(nullptr) {}
		void Init(const std::string& _root);
		void PrintToUI();
	private:
		PathNode* m_rootNode;
	};
}