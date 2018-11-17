#include "scoreboard.h"

#include <fstream>
#include <sstream>
#include <assert.h>

#include <iostream>

Scoreboard::Scoreboard(const std::string & fileName): m_fileName(fileName){
	std::fstream file;

	file.open(m_fileName);
	if(!file.is_open()){
		m_fileName = "launch/Scoreboard.txt";
		file.open(m_fileName); //open default
		assert(file.is_open());
	}

	std::stringstream fileBuffer;
	fileBuffer << file.rdbuf(); //load file data into buffer
	file.close();

	std::string name;
	unsigned int score;
	while(!fileBuffer.eof()){
		fileBuffer >> name >> score;
		m_data.push_back(std::pair<std::string, unsigned int>(name, score));
	}
}

Scoreboard::~Scoreboard(void){
	UpdateFile();
}

void Scoreboard::GetScore(unsigned int position, std::string & name, unsigned int & score) const{
	name = m_data[position].first;
	score = m_data[position].second;
}

unsigned int Scoreboard::AddScore(const std::string & name, unsigned int score){
	unsigned int newPlace = 0;
	std::vector<std::pair<std::string, unsigned int>>::const_iterator iterator = m_data.begin();	
	
	while(iterator != m_data.end()){
		if(iterator->second <= score)
			break;
		++newPlace;
		++iterator;
	}
	
	m_data.insert(iterator, std::pair<std::string, unsigned int>(name,score));	

	return newPlace;
}


void Scoreboard::UpdateFile(void){
	std::ofstream file;

	file.open(m_fileName);
	assert(file.is_open());

	for(unsigned int i = 0; i<10; ++i)
		file << m_data[i].first << " " << m_data[i].second << std::endl;

	file.close();	
}
