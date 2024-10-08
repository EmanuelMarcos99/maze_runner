#include <stdio.h>
#include <stdlib.h>
#include <stack>
#include <iostream>
#include <thread>
#include <chrono>
#include <atomic>
#include <future>

char **maze; 
int num_rows;
int num_cols;
bool finalizado;

struct pos_t
{
	int i;
	int j;
};

std::stack<pos_t> valid_positions;

pos_t load_maze(const char *file_name)
{
	pos_t initial_pos;
	FILE *file_maze = fopen(file_name, "r");

	fscanf(file_maze, "%d %d\n", &num_rows, &num_cols);

	maze = (char **)malloc(num_rows * sizeof(char *));
	for (int i = 0; i < num_rows; ++i)
		maze[i] = (char *)malloc(num_cols * sizeof(char));
	char barran;

	for (int i = 0; i < num_rows; ++i)
	{
		for (int j = 0; j < num_cols; ++j)
		{
			fscanf(file_maze, "%c", &maze[i][j]);
			if (maze[i][j] == 'e')
			{
				initial_pos.i = i;
				initial_pos.j = j;
			}
		}
		barran = fscanf(file_maze, "%c", &barran);
	}
	fclose(file_maze);
	return initial_pos;
}

void print_maze()
{
	for (int i = 0; i < num_rows; ++i)
	{
		for (int j = 0; j < num_cols; ++j)
		{
			printf("%c", maze[i][j]);
		}
		printf("\n");
	}
}

bool walk(pos_t current_pos)
{
	std::this_thread::sleep_for(std::chrono::milliseconds(100));
    
	int i = current_pos.i;
	int j = current_pos.j;

	if (i + 1 > num_rows && maze[current_pos.i + 1][current_pos.j] == 's')
	{
		finalizado = true;
	}

	if (i - 1 > -1 && maze[current_pos.i - 1][current_pos.j] == 's')
	{
		finalizado = true;
	}

	if (j + 1 < num_cols && maze[current_pos.i][current_pos.j + 1] == 's')
	{
		finalizado = true;
	}

	if (j - 1 > -1 && maze[current_pos.i][current_pos.j - 1] == 's')
	{
		finalizado = true;
	}

	if (j + 1 < num_cols && (maze[i][j + 1] == 'x'))
	{
		maze[current_pos.i][current_pos.j] = '.';
		current_pos.j = j + 1;
		if (maze[current_pos.i][current_pos.j] != '.')
		{
			maze[current_pos.i][current_pos.j] = 'o';
			std::this_thread::sleep_for(std::chrono::milliseconds(100));
			std::thread t(walk, current_pos);
			t.detach();
		}
	}

	if (j - 1 > -1 && (maze[i][j - 1] == 'x'))
	{
		maze[current_pos.i][current_pos.j] = '.';
		current_pos.j = j - 1;
		if (maze[current_pos.i][current_pos.j] != '.')
		{
			maze[current_pos.i][current_pos.j] = 'o';
			std::this_thread::sleep_for(std::chrono::milliseconds(100));
			std::thread t(walk, current_pos);
			t.detach();
		}
	}

	if (i + 1 < num_rows && (maze[i + 1][j] == 'x'))
	{
		maze[current_pos.i][current_pos.j] = '.';
		current_pos.i = i + 1;
		if (maze[current_pos.i][current_pos.j] != '.')
		{
			maze[current_pos.i][current_pos.j] = 'o';
			std::this_thread::sleep_for(std::chrono::milliseconds(100));
			std::thread t(walk, current_pos);
			t.detach();
		}
	}

	if (i - 1 > -1 && (maze[i - 1][j] == 'x'))
	{
		maze[current_pos.i][current_pos.j] = '.';
		current_pos.i = i - 1;
		if (maze[current_pos.i][current_pos.j] != '.')
		{
			maze[current_pos.i][current_pos.j] = 'o';
			std::this_thread::sleep_for(std::chrono::milliseconds(100));
			std::thread t(walk, current_pos);
			t.detach();
		}
	}

	if (finalizado == true)
	{
		return true;
	}
	return false;


int main(int argc, char *argv[])
{
	pos_t initial_pos = load_maze("../data/maze.txt");
	std::thread walk_thread(walk, initial_pos);

	while (!finalizado)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
		system("clear || maze");
		print_maze();
	}
	if (finalizado) {
		std::cout << "Saída encontrada" << std::endl;
	}

	return 0;
}
