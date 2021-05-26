/*
 * Bejeweled App
 * Graham Scott
 * 04.1.2020
 * Version 1.2.0
 * An iteration on the Bejeweled code provided.
 * Accepts no arguments and returns 0 unless there's a resource error.
 *
 * All credits and licensing information can be found in the file credits.txt
 */

//     FEATURE ADDITION NOTES
/*
 * - Revamped Score tracking. 1 gem is now worth 100 points
 * - Added SCALE, a simple way to scale the resolution of the entire application up
 * - Added SFX for clicking and failed matches
 * - Combo Handling:
 * 	 - Different match SFX for multiple matches without player involvement
 * 	 - Score multipliers for combo matches
 * - Score adds to timer.
 * - "Swap Limit" Mechanics: Score high enough, and you will unlock the ability to swap gems that aren't next to each other.
 * - Added a title screen which gives a very basic explanation of game mechanics
*/

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <time.h>
#include <string>

using namespace sf;

const int SCALE = 2; // handy utility variable to change the visual scale of the game

int ts = 54 * SCALE; //tile size
Vector2i offset(48 * SCALE, 24 * SCALE); // Offset of the board

class Piece {
private:
	int x, y;
	int col, row;
	int kind; // type of gem
	int match; //Is the piece part of a 3+ match? Nonzero means yes.
	int alpha;

public:
	Piece() {
		match = 0;
		alpha = 255;
	}

	int getRow() {
		return row;
	}
	void setRow(int _row) {
		row = _row;
	}
	int getCol() {
		return col;
	}
	int setCol(int _col) {
		col = _col;
	}
	int getX() {
		return x;
	}
	void setX(int _x) {
		x = _x;
	}
	int getY() {
		return y;
	}
	void setY(int _y) {
		y = _y;
	}
	int getKind() {
		return kind;
	}
	void setKind(int _kind) {
		kind = _kind;
	}
	int getMatch() {
		return match;
	}
	void setMatch(int _match) {
		match = _match;
	}
	void incMatch() { // match++ is one of the only ways match is ever changed
		match++;
	}
	int getAlpha() {
		return alpha;
	}
	void setAlpha(int _alpha) {
		alpha = _alpha;
	}
};
Piece grid[10][10];

void swap(Piece p1, Piece p2) {
	int temp = p1.getCol();
	p1.setCol(p2.getCol());
	p2.setCol(temp);

	temp = p1.getRow();
	p1.setRow(p2.getRow());
	p2.setRow(temp);

	grid[p1.getRow()][p1.getCol()] = p1;
	grid[p2.getRow()][p2.getCol()] = p2;
}

/*
 * To keep a consistent font throughout the game, this function is used to initialize all text and make sure styling
 * is the same
 */
void textSetup(sf::Text* text, sf::Font* font, int size) {
	text->setFont(*font);
	text->setString("0");
	text->setCharacterSize(size*SCALE);
	text->setFillColor(sf::Color::White);
	text->setOutlineColor(sf::Color::Black);
	text->setOutlineThickness(5);
}

int main() {
	//////////////////////////////////////////////////////
	//                       SETUP                      //
	//////////////////////////////////////////////////////

	srand(time(0));

	RenderWindow app(VideoMode(740 * SCALE, 480 * SCALE), "Match-3 Game!");
	app.setFramerateLimit(60);

	// Image Setup
	Texture t1, t2, t3;
	if (!t1.loadFromFile("images/background.png"))
		return -1;
	if(!t2.loadFromFile("images/gems.png"))
		return -1;
	if(!t3.loadFromFile("images/title.png"))
		return -1;
	Sprite background(t1), gems(t2), title(t3);

	background.scale(SCALE,SCALE); // If you scale the window larger, these resize the images
	gems.scale(SCALE,SCALE);
	title.scale(SCALE,SCALE);
	float titletimer = 6; // How long it hangs on the opening screen

	// Text Setup
	sf::Font font;
	if (!font.loadFromFile("fonts/hemi head bd it.ttf"))
		return -1;

	sf::Text scoreText;
	textSetup(&scoreText, &font, 30);
	scoreText.setString("SCORE:");
	scoreText.setPosition(490*SCALE,400*SCALE);

	int pointsScored = 0; // Points scored in one run through the main loop
	int playerScore = 0; // Total points scored
	sf::Text scoreNumberText;
	textSetup(&scoreNumberText, &font, 30);
	scoreNumberText.setString("0");
	scoreNumberText.setPosition(600*SCALE, 400*SCALE);

	int multiplier = 0; // Score multiplier from combos. Only displays when greater than 1.
	sf::Text multiplierText;
	textSetup(&multiplierText, &font, 80);
	multiplierText.setString("X");
	multiplierText.setPosition(510*SCALE,310*SCALE);

	float timer = 0; // Time left in the game
	sf::Text timerText;
	textSetup(&timerText, &font, 30);
	timerText.setString("0");
	timerText.setPosition(485*SCALE,435*SCALE);

	int swapLimit = 1; // Notifies the player when Swap Limit increases
	float swapNotificationTimer = 0;
	sf::Text swapLimitText;
	textSetup(&swapLimitText, &font, 25);
	swapLimitText.setString("Swap limit up!");
	swapLimitText.setPosition(20*SCALE,10*SCALE);

	// Sound Setup
	bool matchFlag = false; //Do we need to play the match sound effect on the next loop?
	int comboFlag = 0; //Tracks the number of matches that occur without the player clicking
	sf::SoundBuffer click1Buffer, click2Buffer, click3Buffer, clickfailBuffer, comboBuffer, loseBuffer;
	if (!click1Buffer.loadFromFile("sounds/click.ogg")) return -1;
	if (!click2Buffer.loadFromFile("sounds/click2.ogg")) return -1;
	if (!click3Buffer.loadFromFile("sounds/click3.ogg")) return -1;
	if (!clickfailBuffer.loadFromFile("sounds/clickfail.ogg")) return -1;
	if (!comboBuffer.loadFromFile("sounds/combo.ogg")) return -1;
	if (!loseBuffer.loadFromFile("sounds/lose.ogg")) return -1;
	sf:Sound click1, click2, click3, clickfail, combo, lose;
	click1.setBuffer(click1Buffer);
	click1.setLoop(false);
	click2.setBuffer(click2Buffer);
	click3.setBuffer(click3Buffer);
	clickfail.setBuffer(clickfailBuffer);
	combo.setBuffer(comboBuffer);
	lose.setBuffer(loseBuffer);

	// Grid Setup
	for (int i = 1; i <= 8; i++) {
		for (int j = 1; j <= 8; j++) {
			grid[i][j].setKind(rand() % 3);
			grid[i][j].setCol(j);
			grid[i][j].setRow(i);
			grid[i][j].setX(j * ts);
			grid[i][j].setY(i * ts);
		}
	}

	int x0, y0, x, y;
	int click = 0;
	Vector2i pos;
	bool isSwap = false, isMoving = false;

	timer = 25; // Timer starts at ~25 seconds (slightly less)

	//////////////////////////////////////////////////////
	//                    MAIN LOOP                     //
	//////////////////////////////////////////////////////

	while (app.isOpen()) {
		pointsScored = 0;
		Event e;
		while (app.pollEvent(e)) {
			if (e.type == Event::Closed) {
				app.close();
			}

			if (e.type == Event::KeyPressed) {
				if (e.key.code == Keyboard::R) { // Resets the game, setting all necessary variables to default
					multiplier = 0;
					playerScore = 0;
					for (int i = 1; i <= 8; i++) {
						for (int j = 1; j <= 8; j++) {
							grid[i][j].setKind(rand() % 3);
							grid[i][j].setCol(j);
							grid[i][j].setRow(i);
							grid[i][j].setX(j * ts);
							grid[i][j].setY(i * ts);
						}
					}
					isSwap = false;
					isMoving = false;
					timer = 25;
					swapLimit = 1;
				}
			}

			if (e.type == Event::MouseButtonPressed) { // Runs once when the user clicks on something
				if (e.key.code == Mouse::Left && timer > 0) {
					if (!isSwap && !isMoving) {
						click++;
						comboFlag = 0; // Combo tracker resets when player makes a swap
						multiplier = 0;
						if (click == 1) {
							click1.stop();
							click1.play();
						} else if (click == 2) {
							click2.stop();
							click2.play();
						}
					}
					pos = Mouse::getPosition(app) - offset;
				}
			}
		}
		if (titletimer < 1) {

			// mouse click
			if (click == 1) {
				x0 = pos.x / ts + 1;
				y0 = pos.y / ts + 1;
			}
			if (click == 2) {
				x = pos.x / ts + 1;
				y = pos.y / ts + 1;
				if (abs(x - x0) + abs(y - y0) <= swapLimit) { // If condition changed to allow more than just adjacent swaps
					swap(grid[y0][x0], grid[y][x]);
					isSwap = 1;
					click = 0;
				} else click = 1;
			}

			//Match finding
			for (int i = 1; i <= 8; i++) {
				for (int j = 1; j <= 8; j++) {
					if (grid[i][j].getKind() == grid[i + 1][j].getKind()) {
						if (grid[i][j].getKind() == grid[i - 1][j].getKind()) {
							for (int n = -1; n <= 1; n++) grid[i + n][j].incMatch();
						}
					}

					if (grid[i][j].getKind() == grid[i][j + 1].getKind()) {
						if (grid[i][j].getKind() == grid[i][j - 1].getKind()) {
							for (int n = -1; n <= 1; n++) grid[i][j + n].incMatch();
						}
					}
				}
			}

			//Moving animation
			isMoving = false;
			for (int i = 1; i <= 8; i++) {
				for (int j = 1; j <= 8; j++) {
					Piece &p = grid[i][j];
					int dx, dy;
					for (int n = 0; n < 4; n++)   // 4 - speed
					{
						dx = p.getX() - p.getCol() * ts;
						dy = p.getY() - p.getRow() * ts;
						if (dx) p.setX(p.getX() - dx / abs(dx));
						if (dy) p.setY(p.getY() - dy / abs(dy));
					}
					if (dx || dy) isMoving = 1;
				}
			}

			//Deleting amimation
			if (!isMoving) {
				for (int i = 1; i <= 8; i++) {
					for (int j = 1; j <= 8; j++) {
						if (grid[i][j].getMatch()) {
							if (grid[i][j].getAlpha() > 10) {
								grid[i][j].setAlpha(grid[i][j].getAlpha() - 10);
								isMoving = true;
							} else {
								matchFlag = true;
								pointsScored += 1; // Gain 1 point for each gem matched
							}
						}
					}
				}
			}

			if (matchFlag) {
				multiplier += 1;
				if (comboFlag) {
					combo.stop(); // plays a different match noise if a match was made without the player making a move
					combo.play();
				} else {
					click3.stop();
					click3.play();
				}
				comboFlag += 1;
				matchFlag = false;
			}

			//Get score
			int score = 0;
			for (int i = 1; i <= 8; i++) {
				for (int j = 1; j <= 8; j++) {
					score += grid[i][j].getMatch();
				}
			}

			//Second swap if no match
			if (isSwap && !isMoving) {
				if (!score) swap(grid[y0][x0], grid[y][x]);
				isSwap = 0;
				clickfail.stop();
				clickfail.play();
			}

			//Update grid
			if (!isMoving) {
				for (int i = 8; i > 0; i--) {
					for (int j = 1; j <= 8; j++) {
						if (grid[i][j].getMatch()) {
							for (int n = i; n > 0; n--) {
								if (!grid[n][j].getMatch()) {
									swap(grid[n][j], grid[i][j]);
									break;
								}
							}
						}
					}
				};

				for (int j = 1; j <= 8; j++) {
					for (int i = 8, n = 0; i > 0; i--) {
						if (grid[i][j].getMatch()) {
							grid[i][j].setKind(rand() % 7);
							grid[i][j].setY(-ts * n++);
							grid[i][j].setMatch(0);
							grid[i][j].setAlpha(255);
						}
					}
				}
			}
		}

		//////draw///////
		app.draw(background);

		for (int i = 1; i <= 8; i++) {
			for (int j = 1; j <= 8; j++) {
				Piece p = grid[i][j];
				gems.setTextureRect(IntRect(p.getKind() * 49, 0, 49, 49));
				gems.setColor(Color(255, 255, 255, p.getAlpha()));
				gems.setPosition(p.getX(), p.getY());
				gems.move(offset.x - ts, offset.y - ts);
				app.draw(gems);
			}
		}

		// Update Timer Text
		timer -= 0.02;
		timer += (pointsScored * multiplier) / 4.;
		if (timer > 0)
			timerText.setString(std::to_string(timer));
		else
			timerText.setString("0    (Press 'r')");

		// Update Score Text
		if (pointsScored > 0) {
			playerScore += pointsScored * multiplier;
			if (playerScore > swapLimit*100) {
				swapNotificationTimer = 2.02;
				swapLimit += 1;
			}
			scoreNumberText.setString(std::to_string(playerScore * 100));
			multiplierText.setString("X" + std::to_string(multiplier));
		}

		app.draw(timerText);
		if(multiplier>1)
			app.draw(multiplierText);
		if(swapNotificationTimer > 0) {
			app.draw(swapLimitText);
			swapNotificationTimer -= 0.02;
		}
		app.draw(scoreText);
		app.draw(scoreNumberText);

		//draw title screen
		if (titletimer > 0) {
			title.setColor(Color(255, 255, 255, std::min(255, static_cast<int>(255 * titletimer))));
			app.draw(title);
			titletimer -= 0.02;
		}

		app.display();
	}
	return 0;
}
