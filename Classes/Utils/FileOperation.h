/*
* Based on example: http://www.cocos2d-x.org/wiki/How_to_read_and_write_file_on_different_platforms
* Adapted and modified by Jukka Vatjus-Anttila
*/
#pragma once
#include <string>

class FileOperation 
{
public:
	static void saveFile(std::string newHighScore);
	static bool readFile(std::string* score);
	static std::string getFilePath();
};
