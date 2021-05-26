#include <SFML/Graphics.hpp>
#include <time.h>

using namespace sf;

int ts = 54; //tile size
Vector2i offset(48, 24); // Offset of the board

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


int main() {
    srand(time(0));

    RenderWindow app(VideoMode(740, 480), "Match-3 Game!");
    app.setFramerateLimit(60);

    Texture t1, t2;
    t1.loadFromFile("images/background.png");
    t2.loadFromFile("images/gems.png");

    Sprite background(t1), gems(t2);

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

    while (app.isOpen()) {
        Event e;
        while (app.pollEvent(e)) {
            if (e.type == Event::Closed) {
                app.close();
            }

            if (e.type == Event::MouseButtonPressed) {
                if (e.key.code == Mouse::Left) {
                    if (!isSwap && !isMoving) click++;
                    pos = Mouse::getPosition(app) - offset;
                }
            }
        }

        // mouse click
        if (click == 1) {
            x0 = pos.x / ts + 1;
            y0 = pos.y / ts + 1;
        }
        if (click == 2) {
            x = pos.x / ts + 1;
            y = pos.y / ts + 1;
            if (abs(x - x0) + abs(y - y0) == 1) {
                swap(grid[y0][x0], grid[y][x]);
                isSwap = 1;
                click = 0;
            } else { click = 1; }
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
                        }
                    }
                }
            }
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

        app.display();
    }
    return 0;
}
