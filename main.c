#include <stdio.h>
#include <stdlib.h>

int getGrid(int* grid, int cols, int* mask, int row, int col);
void setGrid(int* grid, int cols, int* mask, int row, int col, int val);
void initGrid(int* grid, int rows, int cols, int* mask);
int newCellState(int currentState, int surroundingCount);
void iterateGrid(int* grid, int rows, int cols, int* mask);
void drawGrid(int* grid, int rows, int cols, int* mask);

int main(void) {
	int* grid;
	int* mask = (int*) malloc(1);
	initGrid(grid, 10, 10, mask);
	drawGrid(grid, 10, 10, mask);
	free(grid);
}

int getGrid(int* grid, int cols, int* mask, int row, int col) {
	return (grid[row * cols + col] & *mask) != 0;
}

void setGrid(int* grid, int cols, int* mask, int row, int col, int val) {
	if (val) grid[row * cols + col] |= *mask;
	else grid[row * cols + col] &= ~*mask;
}

void initGrid(int* grid, int rows, int cols, int* mask) {
	grid = (int*) malloc(sizeof(int) * 10 * 10);
	*mask = 1;

	setGrid(grid, cols, mask, 4, 4, 1);
	setGrid(grid, cols, mask, 5, 5, 1);
	setGrid(grid, cols, mask, 6, 5, 1);
	setGrid(grid, cols, mask, 6, 4, 1);
	setGrid(grid, cols, mask, 6, 3, 1);
}

int newCellState(int currentState, int surroundingCount) {
	if (currentState) return (surroundingCount == 2 || surroundingCount == 3);
	return surroundingCount == 3;
}

int wrapPos(int pos, int max) {
	if (pos < 0) return max;
	if (pos >= max) return 0;
	return pos;
}

void iterateGrid(int* grid, int rows, int cols, int* mask) {
	int row;
	int col;
	int* newMask = (int*) malloc(1);
	*newMask = ~*mask;
	for (row = 0; row < rows; row++) {
		for (col = 0; col < cols; col++) {
			int offRow;
			int offCol;
			int neighborCount = 0;
			for (offRow = -1; offRow <= 1; offRow++) {
				for (offCol = -1; offCol <= 1; offCol ++) {
					neighborCount += getGrid(grid, cols, mask, wrapPos(row + offRow, rows), wrapPos(col + offCol, cols));
				}
			}
			int curCellVal = getGrid(grid, cols, mask, row, col);
			neighborCount -= curCellVal;
			setGrid(grid, cols, mask, row, col, newCellState(curCellVal, neighborCount));
		}
	}
	*mask = *newMask;
}

void drawGrid(int* grid, int rows, int cols, int* mask) {
	// Draw top bar
	int i;
	printf("+");
	for (i = 0; i < cols; i++) {
		printf("-");
	};
	printf("+\n");

	// Draw rows of grid
	for (i = 0; i < rows; i++) {
		printf("|");
		int j;
		for (j = 0; j < cols; j++) {
			//if (grid[i * cols + j]) printf("x");
			if (getGrid(grid, cols, mask, i, j)) printf("x");
			else printf(" ");
		}
		printf("|\n");
	}

	// Draw bottom bar
	printf("+");
	for (i = 0; i < cols; i++) {
		printf("-");
	};
	printf("+\n");
}
