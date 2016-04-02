/* Author: Shi Jinping
 * Completed Date: March 20th, 2016
 * This code implements Exam1 for WorkApplication's examination.
 * A class GluttonousSnake is defined and recursion data structure is used to implemente the algorithm
 * A stack is defined to record the path for each recursion
 */

#include <iostream>
#include <stack>
using namespace std;

const int _MIN = 1;
const int _MAX = 500;
const int BLOCKED = -1;

typedef struct Point
{
	int x = 0;
	int y = 0;
} Point;

class GluttonousSnake
{
public:
	GluttonousSnake();
	~GluttonousSnake();

	bool SetRow(int row);
	bool SetCol(int col);
	int GetRow();
	int GetCol();
	int GetGridVal(int x, int y);

	int GetMaxScore(); // get the max scores along the path

	void SetStartPnt(Point pnt);
	bool SetVisited(bool status);
	bool InitGrid(int row, int col, int **grid);
	bool InitVisited(int row, int col, bool status);

	bool ReachBoarder(const Point pnt);

	void SearchPath();
	stack<Point> GetPath();

	void GetGrid();

private:
	int m_row;
	int m_col;
	int **m_ppGrid;
	bool **m_ppVisited;
	int m_pathMaxScore;
	stack <Point> m_pathStack;

};


GluttonousSnake::GluttonousSnake()
{
	m_row = 0;
	m_col = 0;
	m_pathMaxScore = -1; // set as -1
	m_ppGrid = NULL;
	m_ppVisited = NULL;
}

GluttonousSnake::~GluttonousSnake()
{
	if (NULL != m_ppGrid)
	{
		for (int i = 0; i < m_row; ++i)
		{
			delete [] m_ppGrid[i];
		}
		delete [] m_ppGrid;
	}

	if (NULL != m_ppVisited)
	{
		for (int i = 0; i < m_row; ++i)
		{
			delete [] m_ppVisited[i];
		}
		delete [] m_ppVisited;
	}
}

bool GluttonousSnake::SetRow(int row)
{
	if (row > _MAX || row < _MIN)
	{
		return false;
	}
	m_row = row;
	return true;
}

bool GluttonousSnake::SetCol(int col)
{
	if (col > _MAX || col < _MIN)
	{
		return false;
	}
	m_col = col;
	return true;
}

void GluttonousSnake::SetStartPnt(Point pnt)
{
	m_pathStack.push(pnt);
	m_ppVisited[pnt.x][pnt.y] = true;
}

bool GluttonousSnake::InitGrid(int row, int col, int **grid)
{
	if (m_row > _MAX || m_row < _MIN || m_col > _MAX || m_col < _MIN)
	{
		return false;
	}

	if (NULL != (m_ppGrid = new int *[row]))
	{
		for (int i = 0; i < row; ++i)
		{
			m_ppGrid[i] = new int [col];
		}

		for (int i = 0; i < row; ++i)
		{
			for (int j = 0; j < col; ++j)
			{
				m_ppGrid[i][j] = grid[i][j];
			}
		}
		return true;
	}

	return false;
}

bool GluttonousSnake::InitVisited(int row, int col, bool status = false)
{
	if (NULL != (m_ppVisited = new bool *[row]))
	{
		for (int i = 0; i < row; ++i)
		{
			m_ppVisited[i] = new bool [col];
		}

		for (int i = 0; i < row; ++i)
		{
			for ( int j = 0; j < col; ++j)
			{
				m_ppVisited[i][j] = status;
			}
		}
		return true;
	}
	return false;
}

bool GluttonousSnake::SetVisited(bool status = false)
{
	if (m_row > _MAX || m_row < _MIN || m_col > _MAX || m_col < _MIN)
	{
		return false;
	}

	for (int i = 0; i < m_row; ++i)
	{
		for (int j = 0; j < m_col; ++j)
		{
			m_ppVisited[i][j] = status;
		}
	}
	return true;
}

bool GluttonousSnake::ReachBoarder(const Point pnt)
{
	if (pnt.y == m_col - 1 && // reach the right most
		m_ppVisited[(pnt.x - 1 + m_row) % m_row][pnt.y] == true && // the up has been visited
		m_ppVisited[(pnt.x + 1 + m_row) % m_row][pnt.y] == true) // the down has been visited
	{
		return true;
	}

	return false;
}

int GluttonousSnake::GetGridVal(int x, int y)
{
	return m_ppGrid[x][y];
}

int GluttonousSnake::GetMaxScore()
{
	int score = 0;
	stack<Point> tmp = m_pathStack;
	while (!tmp.empty())
	{
		Point pnt = tmp.top();
		score += m_ppGrid[pnt.x][pnt.y];
		tmp.pop();
	}

	// score starts from 0. But if there is no path to the boader, should return -1
	// so compare m_pathMaxScore and score-1
	m_pathMaxScore = (score-1) > m_pathMaxScore ? score : m_pathMaxScore;

	return m_pathMaxScore;

}

void GluttonousSnake::SearchPath()
{
	Point pnt = m_pathStack.top();
	Point nextPnt;

	if (ReachBoarder(pnt))
	{
		int Max = GetMaxScore();

		m_ppVisited[pnt.x][pnt.y] = false;
		m_pathStack.pop();
		return;
	}

	// move up
	nextPnt.x = (pnt.x - 1 + m_row) % m_row;
	nextPnt.y = pnt.y;
	if (nextPnt.x < m_row && nextPnt.x >= 0 && 
	    nextPnt.y < m_col && nextPnt.y >= 0 && 
	    m_ppGrid[nextPnt.x][nextPnt.y] != BLOCKED && 
	    m_ppVisited[nextPnt.x][nextPnt.y] == false)
	{
		m_ppVisited[nextPnt.x][nextPnt.y] = true;
		m_pathStack.push(nextPnt);
		GluttonousSnake::SearchPath();
	}

	// move right
	nextPnt.x = pnt.x;
	nextPnt.y = pnt.y + 1;
	if (nextPnt.x < m_row && nextPnt.x >= 0 && 
	    nextPnt.y < m_col && nextPnt.y >= 0 && 
	    m_ppGrid[nextPnt.x][nextPnt.y] != BLOCKED && 
	    m_ppVisited[nextPnt.x][nextPnt.y] == false)
	{
		m_ppVisited[nextPnt.x][nextPnt.y] = true;
		m_pathStack.push(nextPnt);
		SearchPath();
	}

	// move down
	nextPnt.x = (pnt.x + 1 + m_row) % m_row;
	nextPnt.y = pnt.y;
	if (nextPnt.x < m_row && nextPnt.x >= 0 && 
	    nextPnt.y < m_col && nextPnt.y >= 0 && 
	    m_ppGrid[nextPnt.x][nextPnt.y] != BLOCKED && 
	    m_ppVisited[nextPnt.x][nextPnt.y] == false)
	{
		m_ppVisited[nextPnt.x][nextPnt.y] = true;
		m_pathStack.push(nextPnt);
		SearchPath();
	}

	// Can't move to any direction
	// Set to true 
	// Actually, it can get the REAL longest path if set it to false
	// but it violates the rule that "during the trip can visit each cell only once"
	m_ppVisited[pnt.x][pnt.y] = true;
	m_pathStack.pop();
	return;
}


int main()
{
	int 			nRow = 0;
	int 			nCol = 0;
	int 			nMaxScore = -1;
	Point 			StartPnt;
	int 			**grid = NULL;
	GluttonousSnake snake;

	cin >> nRow >> nCol;

	grid = new int *[nRow];
	for (int i = 0; i < nRow; ++i)
	{
		grid[i] = new int [nCol];
	}

	for (int i = 0; i < nRow; ++i)
	{
		for (int j = 0; j < nCol; ++j)
		{
			cin >> grid[i][j];
		}
	}

	snake.SetCol(nCol);
	snake.SetRow(nRow);
	snake.InitGrid(nCol, nRow, grid);
	snake.InitVisited(nCol, nRow);
	
	for (int i = 0; i < nRow; ++i)
	{
		if (snake.GetGridVal(i, 0) == BLOCKED)
		{
			continue;
		}
		int max = -1;
		StartPnt.x = i;
		StartPnt.y = 0;
		snake.SetVisited();
		snake.SetStartPnt(StartPnt);
		snake.SearchPath();
		max = snake.GetMaxScore();
		nMaxScore = nMaxScore < max ? max : nMaxScore;
	}

	cout << nMaxScore << endl;

	if (NULL != grid)
	{
		for (int i = 0; i < nRow; ++i)
		{
			delete [] grid[i];
		}
		delete [] grid;
	}
}
