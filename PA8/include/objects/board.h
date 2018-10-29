#ifndef BOARD_H
#define BOARD_H

#include "object.h"

class Board : public Object {
public:
	Board(const std::string & objFile);
	~Board(void);
        
	void Update(unsigned int dt);
	void Render(void);
};

#endif /* BOARD_H */