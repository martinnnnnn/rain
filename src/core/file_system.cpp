#include "file_system.h"




namespace rain
{
	void FileSystem::Init(const std::string& _root)
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
					std::vector<std::string> path;
					while (p != _root)
					{
						path.push_back(p.filename().generic_string());
						p = p.parent_path();
					}
					path.push_back(p.generic_string());

					PathNode* node = m_rootNode;
					for (int i = path.size() - 1; i >= 0; --i)
					{
						bool nodeFound = false;
						for (size_t j = 0; j < node->children.size(); ++j)
						{
							if (node->children[j]->path == path[i] && node->children[j]->path != m_rootNode->path)
							{
								node = node->children[j];
								nodeFound = true;
								break;
							}
						}
						if (!nodeFound)
						{
							PathNode* newNode = new PathNode();
							newNode->path = path[i];
							newNode->father = node;
							node->children.push_back(newNode);
							node = newNode;
						}
					}

				}
			}
		}
		m_rootNode->Print("");
	}

	void PathNode::Print(std::string _tabs)
	{
		std::cout << _tabs << path << std::endl;
		for (auto child : children)
		{
			child->Print(_tabs+"\t");
		}
	}
}