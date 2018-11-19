/*
* Based on example: http://www.cocos2d-x.org/wiki/How_to_read_and_write_file_on_different_platforms
* Adapted and modified by Jukka Vatjus-Anttila
*/

#include "cocos2d.h"
#include "FileOperation.h"

USING_NS_CC;

void FileOperation::saveFile(std::string newHighScore)
{
	auto fileUtils = FileUtils::getInstance();
	if (!fileUtils->writeStringToFile(newHighScore, getFilePath())) {
		log("Unable to save highscore to file: %s", getFilePath().c_str());
	}
	else {
		log("Wrote high-score %s to file: %s", newHighScore.c_str(), getFilePath().c_str());
	}
}

bool FileOperation::readFile(std::string* score)
{
	auto fileUtils = FileUtils::getInstance();
	if (fileUtils->isFileExist(getFilePath())) {
		log("File exists in %s", getFilePath().c_str());

		score->append(fileUtils->getStringFromFile(getFilePath()));
		return true;
	}
	else {
		log("File does not exists in %s", getFilePath().c_str());
		return false;
	}
}

std::string FileOperation::getFilePath()
{
	std::string path("");

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	// Query writeable path. On Android ususally it means /data/data/com.something.this/ but not always.
	path.append(cocos2d::FileUtils::getInstance()->getWritablePath());
	path.append("highscores");
#endif

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
	// Query writeable path which is c://users/documents/Appdata/...
	path.append(cocos2d::FileUtils::getInstance()->getWritablePath() + "highscores");
#endif

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WOPHONE)
	path = cocos2d::CCApplication::sharedApplication().getAppDataPath();

#endif

	return path;
}