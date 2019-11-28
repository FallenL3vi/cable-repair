#include <iostream>
#include <time.h>
#include <cmath>

struct Vect2
{
	unsigned x;
	unsigned y;
};
Vect2 c;
Vect2 m_Player;

enum Type { FREE, WALL, FLOOR, CABLE, ENGINE, CORE, CENTER, BROKEN};

unsigned const MAP_HEIGHT = 20;
unsigned const MAP_WIDTH = 20;
unsigned const MAP_LEVELS = 2;
unsigned const ENGINE_NUMBER = 3;
unsigned CABLE_SUM = 0;;
unsigned const CABLE_BROKEN = 5;
unsigned CABLE_INFO = CABLE_BROKEN;
bool Game = true;

Vect2 engi[ENGINE_NUMBER];

Type map[MAP_LEVELS][MAP_HEIGHT][MAP_WIDTH];

void generation(unsigned &CABLE_SUM)
{
	for(unsigned z = 0; z < MAP_LEVELS; z++)
	{
		for(unsigned y = 0; y < MAP_HEIGHT; y++)
		{
			for(unsigned x = 0; x < MAP_WIDTH; x++)
			{
					if(x == 0 || x == MAP_WIDTH - 1 || y == 0 || y == MAP_HEIGHT - 1)
					{
						map[z][y][x] = WALL;
					}
					else
					{
						
						map[z][y][x] = FLOOR;
					}
			}
		}
		if(z == 0)
		{
			unsigned n_y = rand()%(MAP_HEIGHT-2)+1;
			unsigned n_x = rand()%(MAP_WIDTH-2)+1;

			for(unsigned i = 0; i < ENGINE_NUMBER; i++)
			{
				map[z][n_y][n_x] = ENGINE;
				engi[i].x = n_x;
				engi[i].y = n_y;
				n_y = rand()%(MAP_HEIGHT-2)+1;
				n_x = rand()%(MAP_WIDTH-2)+1;
			}
				while(map[z][n_y][n_x] != FLOOR)
				{
					n_y = rand()%(MAP_HEIGHT-2)+1;
					n_x = rand()%(MAP_WIDTH-2)+1;
				}
				map[z][n_y][n_x] = CORE;
				c.x = n_x;
				c.y = n_y;
			auto position = [] (unsigned const SIZE) { return rand()%(SIZE-2)+1; };

			m_Player.x = position(20);
			m_Player.y = position(20);
		}
		//ZUKO HERE
		if(z == 1)
		{
			for(unsigned i = 0; i < ENGINE_NUMBER; i++)
			{
				unsigned n_x = engi[i].x;
				unsigned n_y = engi[i].y;
				map[z][n_y][n_x] = CABLE;
				CABLE_SUM++;
				auto sign = [] (int num) { return num < 0 ? -1 : num > 0 ? 1 : 0; };
				do{
					if(map[z][n_y][n_x] != CABLE)
					{
						CABLE_SUM++;
					}
					map[z][n_y][n_x] = CABLE;
					n_x += sign(c.x - n_x);
					n_y += sign(c.y - n_y);
				}while(n_x != c.x || n_y != c.y);
			}
			for(unsigned i = 0; i < CABLE_BROKEN; i++)
			{
				bool check = false;
				do{
					unsigned n_y = rand()%(MAP_HEIGHT-2)+1;
					unsigned n_x = rand()%(MAP_WIDTH-2)+1;

					if(map[z][n_y][n_x] == CABLE)
					{
						map[z][n_y][n_x] = BROKEN;
						check = true;
					}
				}while(check != true);
			}
		}
	}
}

void render()
{
	for(unsigned z = 0; z < MAP_LEVELS; z++)
	{
		for(unsigned y = 0; y < MAP_HEIGHT; y++)
		{
			for(unsigned x =0; x < MAP_WIDTH; x++)
			{
				if(z == 1 &&  x == m_Player.x && y == m_Player.y)
				{
					std::cout << "@";
				}
				else if(map[z][y][x] == WALL)
				{
				std::cout << "#";
				}
				else if(map[z][y][x] == FLOOR)
				{
					std::cout << " ";
				}
				else if(map[z][y][x] == ENGINE)
				{
					std::cout << "E";
				}
				else if(map[z][y][x] == CORE)
				{
					std::cout << "C";
				}
				else if(map[z][y][x] == CABLE)
				{
					std::cout << "+";
				}
				else if(map[z][y][x] == BROKEN)
				{
					std::cout << "=";
				}

			}
			std::cout << std::endl;
		}
	}
}

bool check_sum()
{
	unsigned sum = 0;
	for(unsigned y = 0; y < MAP_HEIGHT; y++)
	{
		for(unsigned x = 0; x < MAP_WIDTH; x++)
		{
			if(map[1][y][x] == CABLE)
			{
				sum ++;
			}
		}
	}
	
	return sum < CABLE_SUM ? 0 : sum == CABLE_SUM ? 1 : 0;
}

void info()
{
	std::cout << "BROKEN CABLES: " << CABLE_INFO << std::endl;
	std::cout << "CABLES : " << CABLE_SUM << std::endl;
	std::cout << "Type 'a''w''s'd' and press 'Enter' to move" << std::endl;
	std::cout << "Type 'k' and press 'Enter' to change broken cable" << std::endl;
	std::cout << "Type 'k' while standing on 'C' to check if evertything is OK " << std::endl;
	std::cout << "'+' - CABLE, '=' - BROKEN CABLE, 'C' - CORE, 'E' - ENGINE" << std::endl;
}

void input()
{
	char button;

	std::cin >> button;
	
	switch(button)
	{
		case 'w':
			{
				m_Player.y --;
				break;
			}
		case 's':
			{
				m_Player.y ++;
				break;
			}
		case 'a':
			{
				m_Player.x --;
				break;
			}
		case 'd':
			{
				m_Player.x ++;
				break;
			}
		case 'k':
			{
				if(map[1][m_Player.y][m_Player.x] == BROKEN)
				{
					std::cout <<"B" << std::endl;
					map[1][m_Player.y][m_Player.x] = CABLE;
				}
				else if(map[0][m_Player.y][m_Player.x] == CORE)
				{
					if(check_sum() == true)
					{
						Game = false;
						std::cout << "CABLES ARE FINE" << std::endl;
					}
					else if(check_sum() == false)
					{
						std::cout << "CABLES ARE BROKEN" << std::endl;
					}
				}
				break;
			}
		default:
			{
				break;
			}
	}
}

int main()
{
	srand(time(NULL));
//count cables sum
	generation(CABLE_SUM);
	while(Game)
	{
		info();
		render();
		input();
	}
	return 0;
}
