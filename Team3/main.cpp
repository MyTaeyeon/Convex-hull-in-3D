#include <fstream>
#include "triangleTool.h"
#include <vector>
#include <iostream>
#include <queue>

using namespace std;

int main()
{
	ifstream input("Data.txt");
	int N; input >> N;
	vector <Point3D> X(N);
	for (int i = 0; i < N; i++) input >> X[i];
	input.close();

	// ========== HANDLE INPUT - find convex hull ===========================

	vector <Point2D> Y(N);
	vector <vector <double>> d(3);
	d[0] = { 1, 0, 0 }; d[1] = { 0, 1, 0 }; d[2] = { 0, 0, 1 };

	vector <vector <int> > Face(0); // result

	vector < vector < int> > taken(N, vector <int>(N, 2)); // an edges only appear twice
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

	while (Face.empty())
	{
		for (int i = 0; i < N; i++) Y[i] = to2D(X[i], d[0], d[1]);
		
		int firstIndex = 0;
		for (int i = 1; i < N; i++)
			if (Y[i].y < Y[firstIndex].y) firstIndex = i;
			else if (Y[i].y == Y[firstIndex].y && Y[i].x > Y[firstIndex].x) firstIndex = i;

		int secondIndex;
		if (firstIndex == 0) secondIndex = 1;
		else secondIndex = 0;
		for (int i = 0; i < N; i++)
			if (i != firstIndex && i != secondIndex)
			{
				if (det({ {1, 1, 1} , {Y[firstIndex].x, Y[secondIndex].x, Y[i].x}, {Y[firstIndex].y, Y[secondIndex].y, Y[i].y} }) < 0) secondIndex = i;
			}

		vector <int> I = { firstIndex, secondIndex };
		for (int i = 0; i < N; i++)
			if (i != firstIndex && i != secondIndex)
			{
				if (det({ {1, 1, 1} , {Y[firstIndex].x, Y[secondIndex].x, Y[i].x}, {Y[firstIndex].y, Y[secondIndex].y, Y[i].y} }) == 0) I.push_back(i);
			}

		bool feasible = false;
		if (I.size() > 2 && dim(I, X) == 3) feasible = true;

		if (feasible)
		{
			Face.push_back(I);
		}
		else
		{
			chooseAnOrthogonalSystem(d, X[firstIndex], X[secondIndex]);
		}
	}
	// order it in a clockwise direction  ===================================

	vector <double> temp;
	temp = d[0]; d[0] = d[1]; d[1] = d[2]; d[2] = temp;
	Face[0] = getFace(Face[0], X, d);

	Face[0].push_back(Face[0][0]);
	for (int i = 0; i < Face[0].size() - 1; i++)
	{
		taken[Face[0][i]][Face[0][i + 1]]--;
		taken[Face[0][i + 1]][Face[0][i]]--;
	}
	Face[0].pop_back();

	// Find all face ========================================================

	int curr = 0;
	while (curr < Face.size())
	{
		vector <bool> Exception(N, false); // for another face ==============
		queue <pair <int, int> > edges;
		Face[curr].push_back(Face[curr][0]);
		for (int i = 0; i < Face[curr].size() - 1; i++) Exception[Face[curr][i]] = true;
		for (int i = 0; i < Face[curr].size() - 1; i++)
			if (taken[Face[curr][i]][Face[curr][i + 1]] > 0)
			{
				edges.push({ Face[curr][i], Face[curr][i + 1] });
			}
		Face[curr].pop_back();

		while (!edges.empty())
		{
			chooseAnOrthogonalSystem(d, X[edges.front().first], X[edges.front().second]);
			for (int i = 0; i < N; i++) Y[i] = to2D(X[i], d[0], d[1]);

			int k = -1;
			for (int i = 0; i < N; i++)
				if (!(Y[i] == Y[edges.front().first]))
				{
					k = i;
					break;
				}
			vector <int> Fnew(1, k);
			int t = k;

			for (int i = k + 1; i < N; i++)
				if (i != edges.front().first && i != edges.front().second && i != k)
				{
					double carry = det({ {1, 1, 1} , {Y[edges.front().first].x, Y[k].x, Y[i].x} , {Y[edges.front().first].y, Y[k].y, Y[i].y} });
					if (carry == 0) Fnew.push_back(i);
					else if (carry < 0)
					{
						Fnew.clear();
						Fnew = { i };
						k = i;
					}
				}
			bool isOld = false;
			for (int i = 0; i < Fnew.size(); i++)
				if (Exception[Fnew[i]]) { isOld = true; break; }
			if (isOld)
			{
				k = t;
				Fnew = { k };
				for (int i = k + 1; i < N; i++)
					if (i != edges.front().first && i != edges.front().second && i != k)
					{
						double carry = det({ {1, 1, 1} , {Y[edges.front().first].x, Y[k].x, Y[i].x} , {Y[edges.front().first].y, Y[k].y, Y[i].y} });
						if (carry == 0) Fnew.push_back(i);
						else if (carry > 0)
						{
							Fnew.clear();
							Fnew = { i };
							k = i;
						}
					}
			}
			Fnew.push_back(edges.front().first);
			Fnew.push_back(edges.front().second);
			if (Fnew.size() > 3)
			{
				vector <double> temp;
				temp = d[0]; d[0] = d[1]; d[1] = d[2]; d[2] = temp;
				Face.push_back(getFace(Fnew, X, d));
			}
			else Face.push_back(Fnew);
			Fnew.clear();
			Fnew.shrink_to_fit();
			
			int p = Face.size() - 1;
			Face[p].push_back(Face[p][0]);
			for (int i = 0; i < Face[p].size() - 1; i++)
			{
				taken[Face[p][i]][Face[p][i + 1]]--;
				taken[Face[p][i + 1]][Face[p][i]]--;
			}
			Face[p].pop_back();
			edges.pop();
		}
		curr++;
	}

	// ======================================================================

	ofstream output("result.txt");
	for (int i = 0; i < Face.size(); i++)
	{
		output << i << ". \n";
		for (int j = 0; j < Face[i].size(); j++) output << X[Face[i][j]];
	}
	output.close();

	// ======================================================================

	ofstream plot("plot.txt");
	for (int i = 0; i < Face.size(); i++)
	{
		int a = Face[i][0], b = Face[i][1];
		for (int j = 2; j < Face[i].size(); j++)
		{
			plot << X[a] << X[b] << X[Face[i][j]];
			b = Face[i][j];
		}
	}
	plot.close();

	return 0;
}