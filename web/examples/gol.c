
#include <p5.h>

#define WIDTH 600
#define HEIGHT WIDTH

#define CELL_SIZE 20
#define COLS (WIDTH / CELL_SIZE)
#define ROWS (HEIGHT / CELL_SIZE)

bool boards[2][COLS][ROWS] = {0};

int curr_board = 0;
#define other_board (1 - curr_board)

void renderBoard() {
    for (int i = 0; i < COLS; i++) {
        for (int j = 0; j < ROWS; j++) {
            int x = i * CELL_SIZE;
            int y = j * CELL_SIZE;

            if (boards[curr_board][i][j]) {
                fill(P5_WHITE);
            } else {
                fill(P5_BLACK);
            }
            rect(x, y, CELL_SIZE, CELL_SIZE);
        }
    }
}

bool nextState(bool cell, int nbors) {
    // Any live cell with fewer than two live neighbors dies, as if by underpopulation.
    if (cell && nbors < 2) return false;
    // Any live cell with two or three live neighbors lives on to the next generation.
    else if (cell && (nbors == 2 || nbors == 3)) return true;
    // Any live cell with more than three live neighbors dies, as if by overpopulation.
    else if (cell && nbors > 3) return false;
    // Any dead cell with exactly three live neighbors becomes a live cell, as if by reproduction.
    else if (!cell && nbors == 3) return true;
    // Any other dead cell stays dead, as if by common sense.
    else return false;
}

int emod(int a, int b) {
    return (a + b) % b;
}

void updateBoard() {
    for (int i = 0; i < COLS; i++) {
        for (int j = 0; j < ROWS; j++) {

            int nbors = 0;
            for (int di = -1; di <= 1; di++) {
                for (int dj = -1; dj <= 1; dj++) {
                    int ni = emod(i + di, COLS);
                    int nj = emod(j + dj, ROWS);
                    if (ni == i && nj == j) continue;
                    nbors += boards[curr_board][ni][nj];
                }
            }

            boards[other_board][i][j] = nextState(boards[curr_board][i][j], nbors);
        }
    }

    curr_board = 1 - curr_board;
}

bool isPaused = false;

void pause(void *btn_void) {
    p5_Element *btn = btn_void;
    isPaused = !isPaused;

    const char *new_msg = isPaused ? "Resume" : "Pause";
    setHTML(btn, new_msg);
}

void mousePressed(void *data) {
    (void) data;
    int cellX = mouseX() / CELL_SIZE;
    int cellY = mouseY() / CELL_SIZE;

    bool *ptr = &boards[curr_board][cellX][cellY];
    *ptr = !*ptr;
}

void setup() {
    p5_Element canvas = createCanvas(WIDTH, HEIGHT);

    // ·#·
    // ··#
    // ###
    boards[curr_board][1][0] = true;
    boards[curr_board][2][1] = true;
    boards[curr_board][0][2] = true;
    boards[curr_board][1][2] = true;
    boards[curr_board][2][2] = true;

    static p5_Element pause_btn;
    pause_btn = createButton("Pause");
    print(pause_btn._id);
    
    onMousePressed(&pause_btn, pause, &pause_btn);
    onMousePressed(&canvas, mousePressed, NULL);

    setFrameRate(10);
}

void draw() {
    background(P5_BLACK);

    if (!isPaused) updateBoard();

    renderBoard();
}
