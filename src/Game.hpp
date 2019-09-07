// Game.hpp

#pragma once

#include "Main.hpp"
#include "Vector.hpp"
#include "LinkedList.hpp"
#include "Camera.hpp"
#include "Random.hpp"
#include "Pair.hpp"

class Genome;
class Game;
class AI;

static const int NUM_TETROMINOS = 19;
static const char tetrominos[NUM_TETROMINOS][4][4] = {
	{
		{ 0, 0, 0, 0 },
		{ 0, 0, 0, 0 },
		{ 0, 0, 1, 1 },
		{ 0, 1, 1, 0 }
	},
	{
		{ 0, 0, 0, 0 },
		{ 0, 0, 1, 0 },
		{ 0, 0, 1, 1 },
		{ 0, 0, 0, 1 }
	},
	{
		{ 0, 0, 0, 0 },
		{ 0, 0, 0, 0 },
		{ 0, 1, 1, 0 },
		{ 0, 0, 1, 1 }
	},
	{
		{ 0, 0, 0, 0 },
		{ 0, 0, 0, 1 },
		{ 0, 0, 1, 1 },
		{ 0, 0, 1, 0 }
	},
	{
		{ 0, 0, 1, 0 },
		{ 0, 0, 1, 0 },
		{ 0, 0, 1, 0 },
		{ 0, 0, 1, 0 }
	},
	{
		{ 0, 0, 0, 0 },
		{ 0, 0, 0, 0 },
		{ 1, 1, 1, 1 },
		{ 0, 0, 0, 0 }
	},
	{
		{ 0, 0, 0, 0 },
		{ 0, 0, 0, 0 },
		{ 0, 1, 1, 1 },
		{ 0, 0, 1, 0 }
	},
	{
		{ 0, 0, 0, 0 },
		{ 0, 0, 1, 0 },
		{ 0, 1, 1, 0 },
		{ 0, 0, 1, 0 }
	},
	{
		{ 0, 0, 0, 0 },
		{ 0, 0, 1, 0 },
		{ 0, 1, 1, 1 },
		{ 0, 0, 0, 0 }
	},
	{
		{ 0, 0, 0, 0 },
		{ 0, 0, 1, 0 },
		{ 0, 0, 1, 1 },
		{ 0, 0, 1, 0 }
	},
	{
		{ 0, 0, 0, 0 },
		{ 0, 0, 1, 1 },
		{ 0, 0, 1, 0 },
		{ 0, 0, 1, 0 }
	},
	{
		{ 0, 0, 0, 0 },
		{ 0, 0, 0, 0 },
		{ 0, 1, 1, 1 },
		{ 0, 0, 0, 1 }
	},
	{
		{ 0, 0, 0, 0 },
		{ 0, 0, 1, 0 },
		{ 0, 0, 1, 0 },
		{ 0, 1, 1, 0 }
	},
	{
		{ 0, 0, 0, 0 },
		{ 0, 1, 0, 0 },
		{ 0, 1, 1, 1 },
		{ 0, 0, 0, 0 }
	},
	{
		{ 0, 0, 0, 0 },
		{ 0, 1, 1, 0 },
		{ 0, 0, 1, 0 },
		{ 0, 0, 1, 0 }
	},
	{
		{ 0, 0, 0, 0 },
		{ 0, 0, 0, 1 },
		{ 0, 1, 1, 1 },
		{ 0, 0, 0, 0 }
	},
	{
		{ 0, 0, 0, 0 },
		{ 0, 0, 1, 0 },
		{ 0, 0, 1, 0 },
		{ 0, 0, 1, 1 }
	},
	{
		{ 0, 0, 0, 0 },
		{ 0, 0, 0, 0 },
		{ 0, 1, 1, 1 },
		{ 0, 1, 0, 0 }
	},
	{
		{ 0, 0, 0, 0 },
		{ 0, 1, 1, 0 },
		{ 0, 1, 1, 0 },
		{ 0, 0, 0, 0 }
	}
};

static const int NUM_UNIQUE_TETROMINOS = 7;
static const int uniqueTetrominos[NUM_UNIQUE_TETROMINOS] = {
	0,
	2,
	4,
	6,
	10,
	14,
	18
};

static const int rotateCW[NUM_TETROMINOS] = {
	1, 0,
	3, 2,
	5, 4,
	7, 8, 9, 6,
	11, 12, 13, 10,
	15, 16, 17, 14,
	18
};

static const int rotateCCW[NUM_TETROMINOS] = {
	1, 0,
	3, 2,
	5, 4,
	9, 6, 7, 8,
	13, 10, 11, 12,
	17, 14, 15, 16,
	18
};

// colors
static const int WHITE = 0;
static const int RED = 1;
static const int GREEN = 2;
static const int BLUE = 3;
static const int CYAN = 4;
static const int MAGENTA = 5;
static const int YELLOW = 6;
static const int ORANGE = 7;
static const glm::vec4 colors[8] = {
	{ 1.f, 1.f, 1.f, 1.f },
	{ 1.f, 0.f, 0.f, 1.f },
	{ 0.f, 1.f, 0.f, 1.f },
	{ 0.f, 0.f, 1.f, 1.f },
	{ 0.f, 1.f, 1.f, 1.f },
	{ 1.f, 0.f, 1.f, 1.f },
	{ 1.f, 1.f, 0.f, 1.f },
	{ 1.f, .5f, 0.f, 1.f }
};
static const int tetrominoColors[NUM_TETROMINOS] = {
	GREEN, GREEN,
	RED, RED,
	CYAN, CYAN,
	MAGENTA, MAGENTA, MAGENTA, MAGENTA,
	BLUE, BLUE, BLUE, BLUE,
	ORANGE, ORANGE, ORANGE, ORANGE,
	YELLOW
};

// size of a square on the board
static const int SQUARE_SIZE = 30;

class Game {
public:
	Game(AI* _ai);
	~Game();

	// init
	void init();

	// term
	void term();

	// play a sound
	int playSound(const char* filename, bool loop);

	// stop a sound
	int stopSound(int channel);

	// lets you control the game with a keyboard
	void doKeyboardInput();
	
	// runs the AI and allows it to make inputs
	void doAI();

	// process a frame
	void process();

	// draw a frame
	void draw(Camera& camera);

	// inputs
	enum Input {
		IN_DOWN,
		IN_RIGHT,
		IN_LEFT,
		IN_CW,
		IN_CCW,
		IN_MAX
	};
	bool inputs[IN_MAX];
	bool oldInputs[IN_MAX];
	Uint32 inputTimes[IN_MAX];
	bool pressed(Input in);
	bool repeat(Input in);

	// game board
	static const int boardW = 10;
	static const int boardH = 20;
	ArrayList<int> board;

	Uint32 score = 0;
	Uint32 ticks = 0;
	Random rand;
	int ticksPerSecond = 0;
	bool gameInSession = false;

	AI* ai = nullptr;
	Genome* genome = nullptr;

	// player info
	int tetromino = 0;
	int playerX = 0;
	int playerY = 0;
	bool moved = true;
	
	// state machine
	enum State {
		INVALID,
		PLAY,
		CLEAR,
		DROP,
		NUM
	};
	State state = State::INVALID;
	Uint32 stateTime = 0;

	// music
	int music = 0;
	int musicChannel = -1;

	// bake tetro into current position
	void bakeTetro();

	// lift tetro from current position
	void liftTetro();

	// return true if tetro is blocked in current position
	bool blocked();

	// add a new piece to the board
	void newPiece();

	// check each row for a line clear
	int clearLines();

	// move rows down
	void dropLines();
};