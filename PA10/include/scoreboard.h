#ifndef SCOREBOARD_H
#define SCOREBOARD_H

#include <string>
#include <vector>

class Scoreboard {

public:
	Scoreboard(const std::string & fileName = "/home/dp/Desktop/CS480_Workspace/cs480Patel/PA10/launch/Scoreboard.txt");
	~Scoreboard(void);

	void GetScore(unsigned int position, std::string & name, unsigned int & score) const;
	unsigned int AddScore(const std::string & name, unsigned int score);

private:
	std::string m_fileName;
	std::vector<std::pair<std::string, unsigned int>> m_data;

	void UpdateFile(void);
};

#endif /* SCOREBOARD_H */
