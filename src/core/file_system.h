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
		std::string path;
		PathNode* father;
		std::vector<PathNode*> children;
	};



	class FileSystem
	{
	public:
		FileSystem() : m_rootNode(nullptr) {}
		void Init(const std::string& _root)
		{
			m_rootNode = new PathNode();
			m_rootNode->path = _root;
			m_rootNode->father = nullptr;

			for (auto& entry : fs::recursive_directory_iterator(_root))
			{
				if (!is_directory(entry))
				{
					fs::path p = fs::path(entry);

					if (p.has_parent_path())
					{
						while (p != _root)
						{
							std::cout << p << " 's directory : " << p.parent_path() /*<< " which name is " << p.parent_path().filename()*/;
							std::cout << std::endl;
							p = p.parent_path();
						}
						std::cout << std::endl;
					}
				}
			}
		}

	private:
		PathNode* m_rootNode;
	};
}