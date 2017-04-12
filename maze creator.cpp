#include<iostream>
#include<ctime>
#include<cstdlib>
using namespace std;
class UFSet
{
public:
	UFSet(){};
	~UFSet()
	{
		delete[]parent;
	}
	void reset(int sz)
	{
		if (parent != NULL) parent = NULL;
		Size = sz;
		parent = new int[sz];
		for (int i = 0; i < sz; i++)
			parent[i] = i;
	}
	UFSet(int sz)
	{
		reset(sz);
	}
	int find_root(int x)
	{
		while (parent[x] != x)
			x = parent[x];
		return x;
	}
	void Union(int root1, int root2)
	{
		parent[find_root(root2)] = find_root(root1);
	}
private:
	int *parent;
	int Size;
};
class Maze
{
private:
	bool **con = NULL;
	int row, col;
	UFSet u;
public:
	Maze(){};
	void reset(int n, int m)
	{
		if (con != NULL) con = NULL;
		row = n;
		col = m;
		int tot = (n + 1) * (m + 1);
		con = new bool*[tot];
		for (int i = 0; i < tot; i++)
		{
			con[i] = new bool[tot];
			for (int j = 0; j < tot; j++)
				con[i][j] = false;
		}
		u.reset((row + 1) * (col + 1) + 1);
	}
	Maze(int n, int m)
	{
		reset(n, m);
	}
	void set_col(int r1, int tail)
	{
		int r2 = r1 + col + 1;
		while (r1 < tail)
		{
			u.Union(r1, r2);
			con[r1][r2] = true;
			con[r2][r1] = true;
			r1 = r2;
			r2 += col + 1;
		}
	}
	void set_row(int r1, int tail)
	{
		int r2 = r1 + 1;
		while (r1 < tail)
		{
			u.Union(r1, r2);
			con[r1][r2] = true;
			con[r2][r1] = true;
			r1++;
			r2++;
		}
	}
	void expand(int now, int r, int step, int maxstep)
	{
		int dir[4] = { -col - 1, col + 1, -1, 1 };
		int next, tot = 0, b[4];
		bool flag = false;
		if (step == maxstep) return;
		srand(time(0));
		if (rand() % 32 == 1) return;
		for (int i = 0; i < 4; i++)
		{
			next = now + dir[i];
			if ((next >= 0) && (next <= (row + 1) * (col + 1)))
			{
				if ((!con[next][now]) && (!con[now][next]) && (u.find_root(next) != r) && (u.find_root(next) != u.find_root(now)))
				{
					b[tot] = next;
					tot++;
					flag = true;
				}
			}
		}
		if (flag)
		{
			srand(time(0));
			next = b[rand() % tot];
			step++;
			con[now][next] = true;
			con[next][now] = true;
			u.Union(now, next);
			u.Union(next, now);
			expand(next, r, step, maxstep);
		}
	}
	void create()
	{
		int tot = (row + 1) * (col + 1), select;
		bool *check;
		u.reset(tot);

		set_col(col + 1, row * (col + 1));
		set_row(row * (col + 1), (row + 1) * (col + 1) - 1);

		set_row(0, col);
		set_col(col, row * (col + 1) - 1);
		check = new bool[row];
		for (int i = 0; i < row; i++)
			check[i] = false;
		for (int i = 1; i <= row / 2; i++)
		{
			int j = 1;
			srand(time(0));
			select = rand() % (row - 2) + 1;
			while (check[select])
			{
				j++;
				srand(j * time(0));
				select = rand() % (row - 2) + 1;
			}
			expand(select * (col + 1), 0, 0, tot / 3);
			check[select] = true;
		}
		for (int i = 0; i < row; i++)
			check[i] = false;
		for (int i = 1; i <= row / 2; i++)
		{
			int j = 1;
			srand(time(0));
			select = rand() % (row - 2) + 1;
			while (check[select])
			{
				j++;
				srand(j * time(0));
				select = rand() % (row - 2) + 1;
			}
			expand((select + 1) * (col + 1) - 1, col + 1, 0, tot / 3);
			check[select] = true;
		}
		check = NULL;
		check = new bool[col];
		for (int i = 0; i < col; i++)
			check[i] = false;
		for (int i = 1; i <= col / 2; i++)
		{
			int j = 1;
			srand(time(0));
			select = rand() % (col - 2) + 1;
			while (check[select])
			{
				j++;
				srand(j * time(0));
				select = rand() % (col - 2) + 1;
			}
			expand(select, col + 1, 0, tot / 3);
			check[select] = true;
		}
		for (int i = 0; i < col; i++)
			check[i] = false;
		for (int i = 1; i <= col / 2; i++)
		{
			int j = 1;
			srand(time(0));
			select = rand() % (col - 2) + 1;
			while (check[select])
			{
				j++;
				srand(j * time(0));
				select = rand() % (col - 2) + 1;
			}
			expand(row * (col + 1) + select, 0, 0, tot / 3);
			check[select] = true;
		}
	}
	void print()
	{
		for (int i = 0; i < col; i++)
			cout << " _";
		cout << endl;
		for (int i = col + 1; i < (row + 1) * (col + 1); i++)
		{
			if (i == (row + 1) * (col + 1) - 1)
			{
				cout << " " << endl;
				continue;
			}
			if (con[i][i - (col + 1)])
				cout << "|";
			else
				cout << " ";
			if ((con[i][i + 1]) && (i % (col + 1) < col))
				cout << "_";
			else
				cout << " ";
			if (i % (col + 1) == col)
				cout << endl;
		}
	}
};
int main()
{
	Maze mz;
	int m, n;
    cout << "Enter the width of the maze: ";
    cin >> m;
    cout << "Enter the height of the maze: ";
    cin >> n;
    mz.reset(n, m);
	mz.create();
	mz.print();
	cout << endl;
	return 0;
}
