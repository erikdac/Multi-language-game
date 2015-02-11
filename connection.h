#ifndef CONNECTION_H
#define CONNECTION_H

class Connection {
private:
	int s0;
public:
	Connection(int s0, int res);
	void readInput();
};

#endif
