// Conway's Game of Life

#include <stdio.h>
#include <unistd.h>

#define H 100
#define W 100

// Define arrays as globals
int mat[H][W];
int buf[H][W];

// Init generation counter
int gen = 0;

// Init foreground and background chars
char* fg = "█";
char* fg_t = "▀";
char* fg_b = "▄";
char* bg = " ";

// Define count
int c;

/* Rendering */

void render(int t)
{
    for(int i = 0; i < H; i+=2)
    {
		for(int j = 0; j < W; j++)
		{
		    // Print array
		    if(mat[i][j] && !mat[i+1][j])
				printf("%s", fg_t);
	
		    else if(!mat[i][j] && mat[i+1][j])
				printf("%s", fg_b);
	
		    else if(mat[i][j] && mat[i+1][j])
				printf("%s", fg);
	
		    else
				printf("%s", bg);
	
		    // Swap buffers
		    buf[i][j] = mat[i][j];
		    buf[i+1][j] = mat[i+1][j];
		}
	printf("\n");
	}

	// Generation counter
	printf("Gen: %i\n", gen);
    
    // Delay of 't' ms
    usleep(t * 1000);

    // For cleaner look
    // 'ESC[#A' Moves cursor up 'H' lines
    // 'ESC[0J' Erases from cursor to end of screen
    printf("\x1b[%iA\x1b[0J", (H/2)+1);
}

/* Calculation */

int count(int x, int y)
{
    c = 0;

    // Sum all living cells directly touching (x, y)
    for(int i = y-1; i <= y+1; i++)
	for(int j = x-1; j <= x+1; j++)
	    if(buf[i][j] && i >= 0 && i < H && j >= 0 && j < W) // Bounds
			c += 1;

    return c;
}

void rules()
{
    for(int i = 0; i < H; i++)
    {
		for(int j = 0; j < W; j++)
		{
			c = count(j, i);

			// Optimization to skip empty space
			if(c)
			{
			    // If a living cell has more than 3 neighbours, die (Overpopulation)
			    // If a living cell has less than 2 neigbours, die (Underpopulation)
			    if(buf[i][j] && c-1 > 3 || c-1 < 2)
					mat[i][j] = 0;
		
			    // If a dead cell has exactly 3 living neighbours, revive (Birth)
			    if(!buf[i][j] && c == 3)
					mat[i][j] = 1;
	
				// Else, stay the same
			}
		}
    }
    gen++;
}

/* Plotting and Patterns */

void plot(int x, int y)
{
    mat[y][x] = 1;
}

void parse(int ph, int pw, int pat[ph][pw], int x, int y, int d)
{
    switch (d)
    {
        case 0:
            for(int i = 0; i < ph; i++)
            for(int j = 0; j < pw; j++)
                mat[y + i][x + j] = pat[i][j];
            break;

        case 1:
            for(int i = 0; i < ph; i++)
            for(int j = 0; j < pw; j++)
                mat[y + i][x + j] = pat[i][(pw-1)-j];
            break;

        case 2:
            for(int i = 0; i < ph; i++)
            for(int j = 0; j < pw; j++)
                mat[y + i][x + j] = pat[(ph-1)-i][(pw-1)-j];
            break;

        case 3:
            for(int i = 0; i < ph; i++)
            for(int j = 0; j < pw; j++)
                mat[y + i][x + j] = pat[(ph-1)-i][j];
            break;
    }
}

// Still lifes:

int block[2][2] = 
{
    {1, 1},
    {1, 1}
};

int eater[4][4] =
{
    {1, 1, 0, 0},
    {1, 0, 1, 0},
    {0, 0, 1, 0},
    {0, 0, 1, 1}
};

int beehive[3][4] =
{
    {0, 1, 1, 0},
    {1, 0, 0, 1},
    {0, 1, 1, 0}
};

int loaf[4][4] =
{
    {0, 1, 1, 0},
    {1, 0, 0, 1},
    {0, 1, 0, 1},
    {0, 0, 1, 0}
};

int boat[3][3] =
{
    {1, 1, 0},
    {1, 0, 1},
    {0, 1, 0}
};

int tub[3][3] =
{
    {0, 1, 0},
    {1, 0, 1},
    {0, 1, 0}
};

// Chaotic

int r_pent[3][3] =
{
    {0, 1, 1},
    {1, 1, 0},
    {0, 1, 0}
};

// Oscillators

int blinker[3][3] =
{ 
    {0, 1, 0},
    {0, 1, 0},
    {0, 1, 0}
};

int toad[4][4] =
{
    {0, 0, 1, 0},
    {1, 0, 0, 1},
    {1, 0, 0, 1},
    {0, 1, 0, 0}
};

int beacon[4][4] =
{
    {1, 1, 0, 0},
    {1, 1, 0, 0},
    {0, 0, 1, 1},
    {0, 0, 1, 1}
};

// Spaceships

int glider[3][3] =
{
    {0, 1, 0},
    {0, 0, 1},
    {1, 1, 1}
};

int lwss[4][5] =
{
    {1, 0, 0, 1, 0},
    {0, 0, 0, 0, 1},
    {1, 0, 0, 0, 1},
    {0, 1, 1, 1, 1}
};

/* Main */

int main(int argc, char* argv[])
{
    //parse(3, 3, r_pent, (W/2)-3, (H/2)-3, 0);
    parse(3, 3, blinker, 0, 0, 0);
    parse(4, 5, lwss, (W/2)-3, (H/2)-3, 0);
    //parse(2, 2, block, 0, 0, 0);

    while(1)
    {
	    render(10);
	    rules();
    }

    return 0;
}
