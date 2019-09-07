// Game.cpp

#include "Main.hpp"
#include "Game.hpp"
#include "Engine.hpp"
#include "Renderer.hpp"
#include "AI.hpp"

Game::Game(AI* _ai) {
	ai = _ai;
	ticksPerSecond = 60;
}

Game::~Game() {
	term();
}

int Game::playSound(const char* filename, bool loop) {
	if (!ai) {
		return mainEngine->playSound(filename, loop);
	} else {
		return 0;
	}
}

int Game::stopSound(int channel) {
	if (!ai) {
		return mainEngine->stopSound(channel);
	} else {
		return 0;
	}
}

void Game::init() {
	board.clear();
	board.resize(boardH * boardW);
	int c = 0;
	for (int y = 0; y < boardH; ++y) {
		for (int x = 0; x < boardW; ++x, ++c) {
			board[c] = 0;
		}
	}

	score = 0;
	ticks = 0;
	rand.seedTime();

	state = PLAY;
	stateTime = 0;
	newPiece();
	gameInSession = true;
}

void Game::term() {
	gameInSession = false;
}

void Game::newPiece() {
	playerX = boardW / 2 - 2;
	playerY = -3;
	tetromino = uniqueTetrominos[rand.getUint8() % NUM_UNIQUE_TETROMINOS];
	if (moved) {
		moved = false;
	} else {
		term();
		playSound("sounds/die.wav", false);
	}
}

void Game::draw(Camera& camera) {
	Renderer* renderer = mainEngine->getRenderer();
	assert(renderer);

	// get square image
	Image* image = mainEngine->getImageResource().dataForString("images/square.png");
	assert(image);

	// board size
	static const int offX = mainEngine->getXres() / 2 - boardW * SQUARE_SIZE / 2;
	static const int offY = mainEngine->getYres() / 2 - boardH * SQUARE_SIZE / 2;

	// draw game window
	Rect<int> boardRect(offX, offY, SQUARE_SIZE * boardW, SQUARE_SIZE * boardH);
	renderer->drawHighFrame(
		Rect<int>(offX - 15, offY - 15, SQUARE_SIZE * boardW + 30, SQUARE_SIZE * boardH + 30),
		5, glm::vec4(0.f, 0.f, .5f, 1.f), false);
	renderer->drawLowFrame(
		Rect<int>(offX - 5, offY - 5, SQUARE_SIZE * boardW + 10, SQUARE_SIZE * boardH + 10),
		5, glm::vec4(0.f, 0.f, .5f, 1.f), true);
	renderer->drawRect(&boardRect, glm::vec4(0.f, 0.f, 0.f, 1.f));

	// draw board
	int c = 0;
	for (int y = 0; y < boardH; ++y) {
		for (int x = 0; x < boardW; ++x, ++c) {
			if (board[c]) {
				Rect<int> rect(offX + x * SQUARE_SIZE, offY + y * SQUARE_SIZE, SQUARE_SIZE, SQUARE_SIZE);
				auto& color = colors[board[c]];
				image->drawColor(nullptr, rect, color);
			}
		}
	}

	// score
	{
		Rect<int> rect;
		rect.x = 10; rect.y = 10;
		rect.w = 0; rect.h = 0;
		StringBuf<32> buf("Score: %d", score);
		renderer->printText(rect, buf.get());
	}

	// ai stats
	if (ai) {
		// generation
		{
			Rect<int> rect;
			rect.x = 10; rect.y = 50;
			rect.w = 0; rect.h = 0;
			StringBuf<32> buf("Generation: %d", ai->getGeneration());
			renderer->printText(rect, buf.get());
		}

		// measured
		{
			Rect<int> rect;
			rect.x = 10; rect.y = 70;
			rect.w = 0; rect.h = 0;
			StringBuf<32> buf("Measured: %d%%", ai->getMeasured());
			renderer->printText(rect, buf.get());
		}

		// max fitness
		{
			Rect<int> rect;
			rect.x = 10; rect.y = 90;
			rect.w = 0; rect.h = 0;
			StringBuf<32> buf("Max fitness: %lld", ai->getMaxFitness());
			renderer->printText(rect, buf.get());
		}
	}
}

void Game::doKeyboardInput() {
	inputs[IN_DOWN] = mainEngine->getKeyStatus(SDL_SCANCODE_DOWN);
	inputs[IN_RIGHT] = mainEngine->getKeyStatus(SDL_SCANCODE_RIGHT);
	inputs[IN_LEFT] = mainEngine->getKeyStatus(SDL_SCANCODE_LEFT);
	inputs[IN_CW] = mainEngine->getKeyStatus(SDL_SCANCODE_X);
	inputs[IN_CCW] = mainEngine->getKeyStatus(SDL_SCANCODE_Z);
}

void Game::doAI() {
	float (&outputs)[5] = genome->outputs;
	inputs[IN_DOWN] = outputs[0] > 0.f;
	inputs[IN_RIGHT] = outputs[1] > 0.f;
	inputs[IN_LEFT] = outputs[2] > 0.f;
	inputs[IN_CW] = outputs[3] > 0.f;
	inputs[IN_CCW] = outputs[4] > 0.f;
}

bool Game::pressed(Input in) {
	if (inputs[in]) {
		if (oldInputs[in]) {
			return false;
		} else {
			oldInputs[in] = true;
			return true;
		}
	} else {
		oldInputs[in] = false;
		return false;
	}
}

bool Game::repeat(Input in) {
	if (inputs[in] && ticks - inputTimes[in] >= (Uint32)ticksPerSecond / 6) {
		inputTimes[in] = ticks;
		return true;
	} else {
		return false;
	}
}

void Game::process() {
	if (!gameInSession) {
		return;
	}
	if (ai) {
		doAI();
	} else {
		doKeyboardInput();
	}

	if (!ai && mainEngine->pressKey(SDL_SCANCODE_M)) {
		++music;
		if (music >= 4) {
			music = 0;
		}
		stopSound(musicChannel);
		switch (music) {
		case 1: musicChannel = playSound("sounds/tetris-music1.wav", true); break;
		case 2: musicChannel = playSound("sounds/tetris-music2.wav", true); break;
		case 3: musicChannel = playSound("sounds/tetris-music3.wav", true); break;
		default: break;
		}
	}

	if (state == State::PLAY) {
		liftTetro();

		if (repeat(IN_RIGHT)) {
			++playerX;
			if (blocked()) {
				--playerX;
			} else {
				playSound("sounds/move.wav", false);
			}
		}

		if (repeat(IN_LEFT)) {
			--playerX;
			if (blocked()) {
				++playerX;
			} else {
				playSound("sounds/move.wav", false);
			}
		}

		if (pressed(IN_CW)) {
			tetromino = rotateCW[tetromino];
			if (blocked()) {
				tetromino = rotateCCW[tetromino];
			} else {
				playSound("sounds/rotate.wav", false);
			}
		}

		if (pressed(IN_CCW)) {
			tetromino = rotateCCW[tetromino];
			if (blocked()) {
				tetromino = rotateCW[tetromino];
			} else {
				playSound("sounds/rotate.wav", false);
			}
		}

		if (ticks) {
			int beat = std::max(1, ticksPerSecond / (2 + (int)score / 5));
			if ((inputs[IN_DOWN] && ticks % (beat / 8) == 0) ||
				(!inputs[IN_DOWN] && ticks % beat == 0)) {
				++playerY;
				if (blocked()) {
					--playerY;
					bakeTetro();
					state = State::CLEAR;
					stateTime = ticks;
					playSound("sounds/drop.wav", false);
				}
				else {
					moved = true;
				}
			}
		}

		if (state == State::PLAY) {
			bakeTetro();
		}
	}
	if (state == State::CLEAR && ticks - stateTime >= (Uint32)ticksPerSecond / 3) {
		int result = clearLines();
		if (result) {
			state = State::DROP;
			stateTime = ticks;
			switch (result) {
			case 1: playSound("sounds/clear1.wav", false); break;
			case 2: playSound("sounds/clear2.wav", false); break;
			case 3: playSound("sounds/clear3.wav", false); break;
			case 4: playSound("sounds/clear4.wav", false); break;
			default: break;
			}
		} else {
			state = State::PLAY;
			stateTime = ticks;
			newPiece();
		}
	}
	if (state == State::DROP && ticks - stateTime >= (Uint32)ticksPerSecond / 3) {
		dropLines();
		state = State::PLAY;
		stateTime = ticks;
		newPiece();
	}
	++ticks;
}

void Game::bakeTetro() {
	int startX = playerX;
	int startY = playerY;
	int endX = playerX + 4;
	int endY = playerY + 4;
	for (int y = startY; y < endY; ++y) {
		for (int x = startX; x < endX; ++x) {
			if (x < 0 || y < 0 || x >= boardW || y >= boardH) {
				continue;
			}
			int u = x - startX;
			int v = y - startY;
			if (tetrominos[tetromino][v][u]) {
				board[y * boardW + x] = tetrominoColors[tetromino];
			}
		}
	}
}

void Game::liftTetro() {
	int startX = playerX;
	int startY = playerY;
	int endX = playerX + 4;
	int endY = playerY + 4;
	for (int y = startY; y < endY; ++y) {
		for (int x = startX; x < endX; ++x) {
			if (x < 0 || y < 0 || x >= boardW || y >= boardH) {
				continue;
			}
			int u = x - startX;
			int v = y - startY;
			if (tetrominos[tetromino][v][u]) {
				board[y * boardW + x] = 0;
			}
		}
	}
}

bool Game::blocked() {
	int startX = playerX;
	int startY = playerY;
	int endX = playerX + 4;
	int endY = playerY + 4;
	for (int y = startY; y < endY; ++y) {
		for (int x = startX; x < endX; ++x) {
			int u = x - startX;
			int v = y - startY;
			if (tetrominos[tetromino][v][u]) {
				if (x < 0 || x >= boardW || y >= boardH) {
					return true;
				}
				if (y >= 0) {
					if (board[y * boardW + x]) {
						return true;
					}
				}
			}
		}
	}
	return false;
}

int Game::clearLines() {
	int result = 0;
	for (int y = 0; y < boardH; ++y) {
		bool filled = true;
		for (int x = 0; x < boardW; ++x) {
			if (board[y * boardW + x] == 0) {
				filled = false;
				break;
			}
		}
		if (filled) {
			++result;
			++score;
			for (int u = 0; u < boardW; ++u) {
				board[y * boardW + u] = 0;
			}
		}
	}
	return result;
}

void Game::dropLines() {
	for (int y = 0; y < boardH; ++y) {
		bool filled = false;
		for (int x = 0; x < boardW; ++x) {
			if (board[y * boardW + x] != 0) {
				filled = true;
				break;
			}
		}
		if (!filled) {
			for (int v = y; v > 0; --v) {
				for (int u = 0; u < boardW; ++u) {
					board[v * boardW + u] = board[(v - 1) * boardW + u];
				}
			}
			for (int u = 0; u < boardW; ++u) {
				board[u] = 0;
			}
		}
	}
}