#pragma once

#include <vector>
#include <filesystem>
#include <string>
#include <iostream>

namespace fs = std::experimental::filesystem::v1;
namespace rain
{
	struct pathNode
	{
		pathNode() :
			path(""),
			father(nullptr)
		{
			
		}

		void  PrintToConsole(std::string _tabs);
		void PrintToUI();

		std::string path;
		pathNode* father;
		std::vector<pathNode*> children;
	};

    class FileSystem
    {
    public:
        FileSystem() : m_rootNode(nullptr) {}
        void Init(const std::string& _root);
        void PrintToUI();
    private:
        pathNode * m_rootNode;
    };





}