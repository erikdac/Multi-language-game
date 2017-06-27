SET TRANSACTION ISOLATION LEVEL SERIALIZABLE;

CREATE TABLE accounts (
	username	VARCHAR(30) 	NOT NULL PRIMARY KEY,
	password	VARCHAR(40) 	NOT NULL,
	active 		BOOLEAN		DEFAULT 0,
	hash 		VARCHAR(64) 	DEFAULT '',
	expiration	DATETIME	DEFAULT '1000-01-01 00:00:00'
);

CREATE TABLE players (
	name	VARCHAR(30),
	x	INTEGER CHECK (x >= 0),
	y	INTEGER CHECK (x >= 0),
	lvl	INTEGER DEFAULT 1 CHECK (lvl > 0),
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

