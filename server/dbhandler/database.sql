SET TRANSACTION ISOLATION LEVEL SERIALIZABLE;

CREATE TABLE accounts (
	active	 bool DEFAULT 0,
	username VARCHAR(30) PRIMARY KEY,
	password VARCHAR(40) NOT NULL
);

CREATE TABLE players (
	name	VARCHAR(30),
	x 		INTEGER CHECK (x >= 0),
	y		INTEGER CHECK (x >= 0),
	lvl		INTEGER CHECK (lvl > 0) DEFAULT 1,
	health	INTEGER CHECK (health > 0),
	mana	INTEGER CHECK (mana >= 0),
	FOREIGN KEY (name) REFERENCES accounts (username)
);

INSERT INTO accounts (username, password) VALUE ('erik', 'no');
INSERT INTO accounts (username, password) VALUE ('daniel', 'yay');
INSERT INTO accounts (username, password) VALUE ('emil', 'password');
INSERT INTO accounts (username, password) VALUE ('mona', '123');

INSERT INTO players (name, x, y, health, mana) VALUE ("erik", 100, 100, 100, 100);
INSERT INTO players (name, x, y, health, mana) VALUE ("daniel", 100, 100, 100, 100);
INSERT INTO players (name, x, y, health, mana) VALUE ("emil", 100, 100, 100, 100);
INSERT INTO players (name, x, y, health, mana) VALUE ("mona", 100, 100, 100, 100);

