#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <time.h>
#include <stdbool.h>


void line_print(uint8_t line[])
{
	for (int i = 0; i < 4; i++)
		printf("%x ", line[i]);
	printf("\n");
}

void line_play(uint8_t line[])
{
	int i = 0;
	for (int j = 0; j < 4; j++)
	{
		// Skip zeroes until end of line
		while (i < 4 && line[i] == 0)
			i++;
		
		// If the line has more input values
		if (i < 4)
		{
			uint8_t val = line[i];
			i++;
			
			// Skip zeroes until end of line
			while (i < 4 && line[i] == 0)
				i++;
			
			if (i < 4 && val == line[i])
			{
				// Combine similar values
				line[j] = val + 1;
				i++;
			}
			else
			{
				// Just keep the original value
				line[j] = val;
			}
		}
		// We're already finished with the line
		else
		{
			line[j] = 0;
		}
	}
}

void grid_print(uint8_t grid[])
{
	for (int y = 0; y < 4; y++)
	{
		for (int x = 0; x < 4; x++)
		{
			if (grid[x + 4*y])
				printf("%x ", grid[x + 4*y]);
			else
				printf(". ");
		}
		printf("\n");
	}
}

bool grid_step(uint8_t grid[])
{
	// Count the number of zeroes
	int zeros = 0;
	for (int i = 0; i < 16; i++)
	{
		if (grid[i] == 0)
			zeros++;
	}
	
	// Check if all the spaces are full
	if (zeros == 0)
		return false;
	
	// Fill one of the zeros at random
	int j = rand() % zeros;
	for (int i = 0; i < 16; i++)
	{
		if (grid[i] == 0)
		{
			if (j == 0)
			{
				grid[i] = 1;
				break;
			}
			j--;
		}
	}
	return true;
}

void grid_play(uint8_t grid[])
{
	// Choose a random direction 0..3
	int direction = rand() & 0x3;
	
	for (int z = 0; z < 4; z++)
	{
		uint8_t line[4];
		
		// Copy grid row to line
		switch (direction)
		{
			case 0: // left
				for (int x = 0; x < 4; x++)
					line[x] = grid[x + 4*z];
			case 1: // right
				for (int x = 0; x < 4; x++)
					line[3 - x] = grid[x + 4*z];
			case 2: // up
				for (int y = 0; y < 4; y++)
					line[y] = grid[z + 4*y];
			case 3: // down
				for (int y = 0; y < 4; y++)
					line[3 - y] = grid[z + 4*y];
		}
		
		line_play(line);
		
		// Copy line back to grid
		switch (direction)
		{
			case 0: // left
				for (int x = 0; x < 4; x++)
					grid[x + 4*z] = line[x];
			case 1: // right
				for (int x = 0; x < 4; x++)
					grid[x + 4*z] = line[3 - x];
			case 2: // up
				for (int y = 0; y < 4; y++)
					grid[z + 4*y] = line[y];
			case 3: // down
				for (int y = 0; y < 4; y++)
					grid[z + 4*y] = line[3 - y];
		}
	}
}

int grid_score(uint8_t grid[])
{
	int score = 0;
	for (int i = 0; i < 16; i++)
	{
		if (grid[i])
			score += 1 << grid[i];
	}
	return score;
}

int grid_run()
{
	uint8_t grid[16];
	memset(grid, 0, sizeof(grid));
	
	// Begin with two nonzero cells
	grid_step(grid);
	
	while (grid_step(grid))
	{
		grid_play(grid);
	}
	
	return grid_score(grid);
}

int main(int argc, char *argv[])
{
	srand(time(NULL));
	
	int max_score = 0;
	while (1)
	{
		int score = grid_run();
		if (score > max_score)
		{
			max_score = score;
			printf("%d\n", max_score);
		}
	}
	
	return 0;
}
