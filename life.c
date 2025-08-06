// Conway's Game of Life (v2)

#include <stdio.h>
#include <unistd.h>

#define H 100
#define W 100

// Define arrays as globals
int mat[H][W];
int buf[H][W];

// Define generation counter
int gen = 0;

// Define foreground and background chars
char* fg = "█";
char* fg_t = "▀";
char* fg_b = "▄";
char* bg = " ";

/* Rendering */

void render(int t, int g)
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

    int l = 0;
    if(g)
    {
	printf("Gen: %i\n", gen);
	l = 1;
    }
    
    // Delay of 't' ms
    usleep(t * 1000);

    // (Optional) for cleaner look
    // 'ESC[#A' Moves cursor up 'H' lines
    // 'ESC[0J' Erases from cursor to end of screen
    printf("\x1b[%iA\x1b[0J", (H/2)+l);
}

/* Calculation */

int count(int x, int y)
{
    int num = 0;

    // Sum all living cells directly touching (x, y)
    for(int i = y-1; i <= y+1; i++)
	for(int j = x-1; j <= x+1; j++)
	    if(buf[i][j] && i >= 0 && i < H && j >= 0 && j < W)
		num += 1;

    return buf[y][x] ? num - 1 : num;
}

void rules()
{
    for(int i = 0; i < H; i++)
    {
	for(int j = 0; j < W; j++)
	{
	    // If a living cell has more than 3 neighbours, die (Overpopulation)
	    // If a living cell has less than 2 neigbours, die (Underpopulation)
	    // (Else, live)
	    if(buf[i][j] && count(j, i) > 3 || count(j, i) < 2)
		mat[i][j] = 0;

	    // If a dead cell has exactly 3 living neighbours, revive (Birth)
	    if(!buf[i][j] && count(j, i) == 3)
		mat[i][j] = 1;
	}
    }

    gen++;
}

/* Plotting and Structure methods */

void plot(int x, int y)
{
    mat[y][x] = 1;
}

void parse(int pw, int ph, int pat[ph][pw], int x, int y)
{
    for(int i = 0; i < ph; i++)
    {
	for(int j = 0; j < pw; j++)
	{
	    mat[y + i][x + j] = pat[i][j];
	}
    }
}

void block(int x, int y)
{
    int a[2][2] = 
    {
	{1, 1},
	{1, 1}
    };

    parse(2, 2, a, x, y);
}

void blinker(int x, int y, int d)
{
    int a[3][3];

    switch(d)
    {
	case 0:
	    int a[3][3] =
	    {
		{0, 1, 0},
		{0, 1, 0},
		{0, 1, 0}
	    };
	    parse(3, 3, a, x, y);

	    break;

	case 1:
	    int b[3][3] =
	    {
		{0, 0, 0},
		{1, 1, 1},
		{0, 0, 0}
	    };
	    parse(3, 3, b, x, y);

	    break;
    }
}

void glider(int x, int y, int d)
{
    switch(d)
    {
	case 0:
	    int a[3][3] =
	    {
		{0, 1, 0},
		{0, 0, 1},
		{1, 1, 1}
	    };
	    parse(3, 3, a, x, y);

	    break;

	case 1:
	    int b[3][3] =
	    {
		{0, 1, 0},
		{1, 0, 0},
		{1, 1, 1}
	    };
	    parse(3, 3, b, x, y);

	    break;

	case 2:
    	    int c[3][3] =
	    {
		{1, 1, 1},
		{1, 0, 0},
    		{0, 1, 0}
	    };
	    parse(3, 3, c, x, y);

	    break;

	case 3:
	    int d[3][3] =
	    {
		{1, 1, 1},
		{0, 0, 1},
		{0, 1, 0}
	    };
	    parse(3, 3, d, x, y);

	    break;
    }
}

void climber(int x, int y, int d)
{
    switch(d)
    {
	case 0:
	    plot(x, y);

	    break;
    }
}

void eater(int x, int y, int d)
{
    switch(d)
    {
	case 0:
	    int a[4][4] =
	    {
		{1, 1, 0, 0},
		{1, 0, 1, 0},
		{0, 0, 1, 0},
		{0, 0, 1, 1}
	    };
	    parse(4, 4, a, x, y);

	    break;
	case 1:
	    int b[4][4] =
	    {
		{0, 0, 1, 1},
		{0, 1, 0, 1},
		{0, 1, 0, 0},
		{1, 1, 0, 0}
	    };
	    parse(4, 4, b, x, y);

	    break;

	case 2:
	    int c[4][4] =
	    {
		{1, 1, 0, 0},
		{0, 1, 0, 0},
		{0, 1, 0, 1},
		{0, 0, 1, 1}
	    };
	    parse(4, 4, c, x, y);

	    break;
	case 3:
	    int d[4][4] =
	    {
		{0, 0, 1, 1},
		{0, 0, 1, 0},
		{1, 0, 1, 0},
		{1, 1, 0, 0}
	    };
	    parse(4, 4, d, x, y);

	    break;
    }
}

void r_pent(int x, int y)
{
    int a[3][3] =
    {
	{0, 1, 1},
	{1, 1, 0},
	{0, 1, 0}
    };
    parse(3, 3, a, x, y);
}

/* Main */

int main(int argc, char* argv[])
{
    // R-Pentomino
    // W = 100, H = 100
    r_pent((W/2)-3, (H/2)-3);

    while(1)
    {
	render(10, 1);
	rules();
    }

    return 0;
}
