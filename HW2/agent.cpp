#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <queue>
#include <algorithm>
#include <stack>
#include <list>

#define INF 0xfffff
#define SIZE 8
#define BLACK -1
#define WHITE 1
#define PASS 999

using namespace std;

int task;
int player;
char nonplayer;
int cuttingOfDepth;
int board[SIZE][SIZE];

int numberOfBlack;
int numberOfWhite;

int best_i = 1000;
int best_j = 1000;

const int direction[8][2] =
{
	0,1,
	0,-1,
	1,0,
	-1,0,
	1,1,
	1,-1,
	-1,1,
	-1,-1
};

const int weight[SIZE][SIZE] = 
{
99, -8 , 8 , 6 , 6 , 8 , -8 , 99,
-8, -24, -4, -3, -3, -4, -24, -8,
8 , -4 , 7 , 4 , 4 , 7 , -4 , 8 ,
6 , -3 , 4 , 0 , 0 , 4 , -3 , 6 ,
6 , -3 , 4 , 0 , 0 , 4 , -3 , 6 ,
8 , -4 , 7 , 4 , 4 , 7 , -4 , 8 ,
-8, -24, -4, -3, -3, -4, -24, -8,
99, -8 , 8 , 6 , 6 , 8 , -8 , 99,
};

ifstream ifile;
fstream ofile;

void input();
void output(bool);

int evaluate(int [SIZE][SIZE]);
int End(int [SIZE][SIZE]);
bool legalMoveList(int [SIZE][SIZE], bool [SIZE][SIZE], int);
void makeMove(int [SIZE][SIZE], int, int, int);
void saveOrginalBoard(int [SIZE][SIZE], int [SIZE][SIZE]);
void restoreOrginalBoard(int [SIZE][SIZE], int [SIZE][SIZE]);

void greedy();

int minmax();
int Min(int, int, int[SIZE][SIZE], int, int, bool, bool);
int Max(int, int, int [SIZE][SIZE], int, int, bool, bool);
void showMinMax(int, int, int, int);

int alphabeta();
int Alpha(int, int, int[SIZE][SIZE], int, int, bool, bool, int, int);
int Beta(int, int, int [SIZE][SIZE], int, int, bool, bool, int, int);
void showAlphaBeta(int, int, int, int);

int main()
{
	input();
	ofile.open("output.txt", ofstream::out | ofstream::trunc);
	
	for (int i = 0; i < SIZE; ++i)
	{
		for (int j = 0; j < SIZE; ++j)
		{
			ofile << '*';
		}
		ofile << endl;
	}

	if (task == 1)
	{
		greedy();
	}
	else if (task == 2)
	{
		ofile << "Node,Depth,Value" << endl;
		minmax();
	}
	else if (task == 3)
	{
		ofile << "Node,Depth,Value,Alpha,Beta" << endl;
		alphabeta();		
	}

	if (best_i >= 0 && best_j >= 0)
	{
		makeMove(board, best_i, best_j, player);
	}
	
	ofile.seekg(0, ios::beg);
	for (int i = 0; i < SIZE; ++i)
	{
		for (int j = 0; j < SIZE; ++j)
		{
			switch (board[i][j])
			{
				case 0: ofile << '*';break;
				case BLACK: ofile << 'X';break;
				case WHITE: ofile << 'O';break;
			}
		}
		ofile << endl;
	}
	ofile.close();

	return 0;
}

int minmax()
{
	return Max(player, 0, board, -1, -1, false, false);
}

int alphabeta()
{
	return Alpha(player, 0, board, -1, -1, false, false, -INF, INF);
}

void showMinMax(int i, int j, int depth, int value)
{
	if (i < 0 && j < 0)
	{
		ofile << "root";
	}
	else if (i == 999 && j == 999)
	{
		ofile << "pass";
	}
	else
	{
		ofile << char(97+j) << i+1;
	}

	ofile << "," << depth << ",";

	if (value == INF) ofile << "Infinity";
	else if (value == -INF) ofile << "-Infinity";
	else ofile << value;

	ofile << endl;
}

int Max(int player, int depth, int board[SIZE][SIZE], int upi, int upj, bool passMax, bool passMin)
{
	int value;
	int bestValue = -INF;
	bool isExistLegalMove;

	if (End(board))
	{
		int x = evaluate(board);
		showMinMax(upi, upj, depth, x);
		return x;
	}

	if (depth >= cuttingOfDepth)
	{
		int x = evaluate(board);
		showMinMax(upi, upj, depth, x);
		return x;
	}

	if (passMax && passMin)
	{
		int x = evaluate(board);
		showMinMax(upi, upj, depth, x);
		return x;
	}

	int orginalBoard[SIZE][SIZE];
	saveOrginalBoard(orginalBoard, board);	
	bool legal[SIZE][SIZE];
	isExistLegalMove = legalMoveList(board, legal, player);
	//cout << isExistLegalMove << endl;

	if (!isExistLegalMove)
	{
		showMinMax(upi, upj, depth, bestValue);
		value = Min(-player, depth+1, board, PASS, PASS, true, passMin);
		if (value > bestValue)
		{
			bestValue = value;
		}
		showMinMax(upi, upj, depth, value);

	}
	else
	{
		showMinMax(upi, upj, depth, bestValue);
		// For any legal move
		for (int i = 0; i < SIZE; ++i)
		{
			for (int j = 0; j < SIZE; ++j)
			{
				if (legal[i][j])
				{
					//cout << char(97+j) << ","<< i+1 << endl;
					makeMove(board, i, j, player);

					value = Min(-player, depth+1, board, i, j, passMax, passMin);				

					restoreOrginalBoard(orginalBoard, board);

					if (value > bestValue)
					{
						bestValue = value;
						if (depth == 0)
						{
							best_j = j;
							best_i = i;
						}
						
					}
					else if ((value == bestValue) && (depth == 0))
					{
						if (i < best_i)
						{
							best_i = i;
							best_j = j;
						}
						else if (i == best_i && j < best_j)
						{
							best_i = i;
							best_j = j;
						}
					}
					
					showMinMax(upi, upj, depth, bestValue);
				}
			}
		}
	}

	return bestValue;
}

int Min(int player, int depth, int board[SIZE][SIZE], int upi, int upj, bool passMax, bool passMin)
{
	int value;
	int bestValue = INF;
	bool isExistLegalMove;

	if (End(board))
	{
		int x = evaluate(board);
		showMinMax(upi, upj, depth, x);
		return x;
	}

	if (depth >= cuttingOfDepth)
	{
		int x = evaluate(board);
		showMinMax(upi, upj, depth, x);
		return x;
	}

	if (passMax && passMin)
	{
		int x = evaluate(board);
		showMinMax(upi, upj, depth, x);
		return x;
	}
	
	int orginalBoard[SIZE][SIZE];
	saveOrginalBoard(orginalBoard, board);
	bool legal[SIZE][SIZE];
	isExistLegalMove = legalMoveList(board, legal, player);

	if (!isExistLegalMove)
	{
		showMinMax(upi, upj, depth, bestValue);
		value = Max(player, depth+1, board, PASS, PASS, passMax, true);
		if (value < bestValue)
		{
			bestValue = value;
		}
		showMinMax(upi, upj, depth, value);
	}
	else
	{
		showMinMax(upi, upj, depth, bestValue);
		// For any legal move
		for (int i = 0; i < SIZE; ++i)
		{
			for (int j = 0; j < SIZE; ++j)
			{
				if (legal[i][j])
				{
					makeMove(board, i, j, player);

					value = Max(-player, depth+1, board, i, j, passMax, passMin);
					
					restoreOrginalBoard(orginalBoard, board);

					if (value < bestValue)
					{
						bestValue = value;
					}
					
					showMinMax(upi, upj, depth, bestValue);

				}
			}
		}
	}

	return bestValue;
}

void showAlphaBeta(int i, int j, int depth, int value, int alpha, int beta)
{
	if (i < 0 && j < 0)
	{
		ofile << "root";
	}
	else if (i == 999 && j == 999)
	{
		ofile << "pass";
	}
	else
	{
		ofile << char(97+j) << i+1;
	}

	ofile << "," << depth << ",";

	if (value == INF) ofile << "Infinity";
	else if (value == -INF) ofile << "-Infinity";
	else ofile << value;

	ofile << ",";

	if (alpha == INF) ofile << "Infinity";
	else if (alpha == -INF) ofile << "-Infinity";
	else ofile << alpha;

	ofile << ",";

	if (beta == INF) ofile << "Infinity";
	else if (beta == -INF) ofile << "-Infinity";
	else ofile << beta;

	ofile << endl;
}

int Alpha(int player, int depth, int board[SIZE][SIZE], int upi, int upj, bool passMax, bool passMin, int alpha, int beta)
{
	int value;
	int bestValue = -INF;
	bool isExistLegalMove;

	if (End(board))
	{
		int x = evaluate(board);
		showAlphaBeta(upi, upj, depth, x, alpha, beta);
		return x;
	}

	if (depth >= cuttingOfDepth)
	{
		int x = evaluate(board);
		showAlphaBeta(upi, upj, depth, x, alpha, beta);
		return x;
	}

	if (passMax && passMin)
	{
		int x = evaluate(board);
		showAlphaBeta(upi, upj, depth, x, alpha, beta);
		return x;
	}

	int orginalBoard[SIZE][SIZE];
	saveOrginalBoard(orginalBoard, board);	
	bool legal[SIZE][SIZE];
	isExistLegalMove = legalMoveList(board, legal, player);

	if (!isExistLegalMove)
	{

		showAlphaBeta(upi, upj, depth, bestValue, alpha, beta);
		value = Beta(-player, depth+1, board, PASS, PASS, true, passMin, alpha, beta);
		if (value > bestValue)
		{
			bestValue = value;
		}
		
		// ****************************
		// can be (value <= beta)
		if (value > alpha && value < beta)
		{
			alpha = value;
		}

		showAlphaBeta(upi, upj, depth, value, alpha, beta);

		if (value >= beta)
		{
			return value;
		}

	}
	else
	{
		showAlphaBeta(upi, upj, depth, bestValue, alpha, beta);
		// For any legal move
		for (int i = 0; i < SIZE; ++i)
		{
			for (int j = 0; j < SIZE; ++j)
			{
				if (legal[i][j])
				{
					makeMove(board, i, j, player);

					value = Beta(-player, depth+1, board, i, j, false, passMin, alpha, beta);				

					restoreOrginalBoard(orginalBoard, board);


					if (value > bestValue)
					{
						bestValue = value;
						if (depth == 0)
						{
							best_j = j;
							best_i = i;
						}
						
					}
					else if ((value == bestValue) && (depth == 0))
					{
						if (i < best_i)
						{
							best_i = i;
							best_j = j;
						}
						else if (i == best_i && j < best_j)
						{
							best_i = i;
							best_j = j;
						}
					}
					
					// ****************************
					// can be (value <= beta)
					if (value > alpha && value < beta)
					{
						alpha = value;
					}

					showAlphaBeta(upi, upj, depth, bestValue, alpha, beta);


					if (value >= beta)
					{
						return value;
					}

				}
			}
		}
	}

	return bestValue;
}

int Beta(int player, int depth, int board[SIZE][SIZE], int upi, int upj, bool passMax, bool passMin, int alpha, int beta)
{
	int value;
	int bestValue = INF;
	bool isExistLegalMove;

	if (End(board))
	{
		int x = evaluate(board);
		showAlphaBeta(upi, upj, depth, x, alpha, beta);
		return x;
	}

	if (depth >= cuttingOfDepth)
	{
		int x = evaluate(board);
		showAlphaBeta(upi, upj, depth, x, alpha, beta);
		return x;
	}

	if (passMax && passMin)
	{
		int x = evaluate(board);
		showAlphaBeta(upi, upj, depth, x, alpha, beta);
		return x;
	}
	
	int orginalBoard[SIZE][SIZE];
	saveOrginalBoard(orginalBoard, board);
	bool legal[SIZE][SIZE];
	isExistLegalMove = legalMoveList(board, legal, player);

	if (!isExistLegalMove)
	{
		showAlphaBeta(upi, upj, depth, bestValue, alpha, beta);
		value = Alpha(player, depth+1, board, PASS, PASS, passMax, true, alpha, beta);
		if (value < bestValue)
		{
			bestValue = value;
		}

		// ****************************
		// can be (value >= alpha)
		if (value < beta && value > alpha)
		{
			beta = value;
		}

		showAlphaBeta(upi, upj, depth, value, alpha, beta);


		if (value <= alpha)
		{
			return value;
		}

		
	}
	else
	{
		showAlphaBeta(upi, upj, depth, bestValue, alpha, beta);
		// For any legal move
		for (int i = 0; i < SIZE; ++i)
		{
			for (int j = 0; j < SIZE; ++j)
			{
				if (legal[i][j])
				{
					makeMove(board, i, j, player);

					value = Alpha(-player, depth+1, board, i, j, passMax, false, alpha, beta);
					
					restoreOrginalBoard(orginalBoard, board);

					if (value < bestValue)
					{
						bestValue = value;
					}

					// ****************************
					// can be (value >= alpha)
					if (value < beta && value > alpha)
					{
						beta = value;
					}
					
					showAlphaBeta(upi, upj, depth, bestValue, alpha, beta);

					if (value <= alpha)
					{
						return value;
					}

					
				}
			}
		}
	}

	return bestValue;
}

void greedy()
{
	int value;
	int bestValue = -INF;
	bool isExistLegalMove;

	if (End(board))
	{
		return;
	}

	int orginalBoard[SIZE][SIZE];
	saveOrginalBoard(orginalBoard, board);	
	bool legal[SIZE][SIZE];
	isExistLegalMove = legalMoveList(board, legal, player);

	if (isExistLegalMove)
	{
		// For any legal move
		for (int i = 0; i < SIZE; ++i)
		{
			for (int j = 0; j < SIZE; ++j)
			{
				if (legal[i][j])
				{
					//cout << char(97+j) << ","<< i+1 << endl;
					makeMove(board, i, j, player);

					value = evaluate(board);				

					restoreOrginalBoard(orginalBoard, board);

					if (value > bestValue)
					{
						bestValue = value;
						best_j = j;
						best_i = i;
					}
					else if (value == bestValue)
					{
						if (i < best_i)
						{
							best_i = i;
							best_j = j;
						}
						else if (i == best_i && j < best_j)
						{
							best_i = i;
							best_j = j;
						}
					}
				}
			}
		}
	}
}

int evaluate(int board[SIZE][SIZE])
{
	int playerWeight = 0;
	int opponentWeight = 0;
	for (int i = 0; i < SIZE; ++i)
	{
		for (int j = 0; j < SIZE; ++j)
		{
			if (board[i][j] == player)
			{
				playerWeight += weight[i][j];
			}
			else if (board[i][j] == -player)
			{
				opponentWeight += weight[i][j];
			}
		}
	}
	return playerWeight - opponentWeight;
}

bool legalMoveList(int board[][SIZE], bool legal[][SIZE], int player)
{
	int isExistLegalMove = false;
	// Begin legal moves list
	int x, y;
	for (int i = 0; i < SIZE; ++i)
	{
		for (int j = 0; j < SIZE; ++j)
		{
			legal[i][j] = false;
		}
	}
	for (int i = 0; i < SIZE; ++i)
	{
		for (int j = 0; j < SIZE; ++j)
		{
			if (board[i][j] == player)
			{
				for (int loop = 0; loop < 8; ++loop)
				{
					x = i + direction[loop][0];
					y = j + direction[loop][1];

					if ((x < SIZE) && (x >= 0) && (y < SIZE) && (y >= 0))
					{
						while (board[x][y] == -player)
						{
							x += direction[loop][0];
							y += direction[loop][1];
						}
						if (
							((x < SIZE) && (x >= 0) && (y < SIZE) && (y >= 0))
							&&
							((x != i + direction[loop][0]) || (y != j + direction[loop][1]))
							)
							
						{
							//cout << x << y << endl;
							if (board[x][y] == 0)
							{
								legal[x][y] = true;
								isExistLegalMove = true;
								//cout << char(97+j) << ","<< i+1 << endl;
							}
							
							//cout << char(97+j) << ","<< i+1 << endl;
						}
					}
				}
			}
		}
	}
	//cout << isExistLegalMove << endl;
	// End legal moves list
	return isExistLegalMove;
}

void saveOrginalBoard(int orginalBoard[][SIZE], int board[][SIZE])
{
	for (int i = 0; i < SIZE; ++i)
	{
		for (int j = 0; j < SIZE; ++j)
		{
			orginalBoard[i][j] = board[i][j];
		}
	}
}

void restoreOrginalBoard(int orginalBoard[][SIZE], int board[][SIZE])
{
	for (int i = 0; i < SIZE; ++i)
	{
		for (int j = 0; j < SIZE; ++j)
		{
			board[i][j] = orginalBoard[i][j];
		}
	}
}

void makeMove(int board[][SIZE], int i, int j, int player)
{
	board[i][j] = player;
	for (int loop = 0; loop < 8; ++loop)
	{
		bool sign = false;
		int si = i + direction[loop][0];
		int sj = j + direction[loop][1];
		if ((si < SIZE) && (si >= 0) && (sj < SIZE) && (sj >= 0))
		{
			while(board[si][sj] == -player)
			{
				si += direction[loop][0];
 				sj += direction[loop][1];
 				sign = true;
			}
			if ( (si < SIZE) && (si >= 0) && (sj < SIZE) && (sj >= 0)&& (sign && (board[si][sj] == player)))
			{
				int p = i;
				int q = j;
				while(p!= si || q != sj)
				{
					board[p][q] = player;
					p += direction[loop][0];
					q += direction[loop][1];
				}
			}
		}
	}
}

int End(int board[SIZE][SIZE])
{
	int sumWhite = 0;
	int sumBlack = 0;
	for (int i = 0; i < SIZE; ++i)
	{
		for (int j = 0; j < SIZE; ++j)
		{
			if (board[i][j] == WHITE)
			{
				sumWhite++;
			}
			else if (board[i][j] == BLACK)
			{
				sumBlack++;
			}
		}
	}
	if (sumWhite == 0)
	{
		return BLACK;
	}
	else if (sumBlack == 0)
	{
		return WHITE;
	}
	else
	{
		return 0;
	}
}

void input()
{
	char temp;
	numberOfBlack = 0;
	numberOfWhite = 0;
	ifile.open("input.txt");
	ifile >> task;
	ifile >> temp;
	player = (temp == 'X')?BLACK:WHITE;
	ifile >> cuttingOfDepth;
	for (int i = 0; i < SIZE; ++i)
	{
		for (int j = 0; j < SIZE; ++j)
		{
			ifile >> temp;
			if (temp == '*')
			{
				board[i][j] = 0;
			}
			else if (temp == 'X')
			{
				board[i][j] = BLACK;
			}
			else if (temp == 'O')
			{
				board[i][j] = WHITE;
			}
		}
	}
}

void output(bool flag)
{
	if (flag)
	{
		ofile.seekg(0, ios::beg);
	}
	for (int i = 0; i < SIZE; ++i)
	{
		for (int j = 0; j < SIZE; ++j)
		{
			switch (board[i][j])
			{
				case 0: ofile << '*';break;
				case BLACK: ofile << 'X';break;
				case WHITE: ofile << 'O';break;
			}
		}
		ofile << endl;
	}
	ofile.close();
}