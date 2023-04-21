#include "triangleTool.h"

using namespace std;

bool operator < (Point2D& a, Point2D& b)
{
	return a.x < b.x;
}

bool operator ==(Point2D& a, Point2D& b)
{
	return (a.x == b.x && a.y == b.y);
}

ifstream& operator >>(ifstream& in, Point3D& a)
{
	in >> a.x >> a.y >> a.z;
	return in;
}

ofstream& operator <<(ofstream& out, Point3D& a)
{
	out << a.x << " " << a.y << " " << a.z << endl;
	return out;
}

ostream& operator <<(ostream& out, Point3D& a)
{
	out << a.x << " " << a.y << " " << a.z << endl;
	return out;
}

bool operator ==(Point3D& a, Point3D& b)
{
	if (a.x == b.x && a.y == b.y && a.z == b.z) return true;
	else return false;
}

Point2D to2D(Point3D& base, vector <double>& k, vector <double>& l)
{
	Point2D ans;
	ans.x = base.x * k[0] + base.y * k[1] + base.z * k[2];
	ans.y = base.x * l[0] + base.y * l[1] + base.z * l[2];
	return ans;
}

double det(vector<vector<double> > base)
{
	if (base.size() == 2) return (base[0][0] * base[1][1] - base[1][0] * base[0][1]);
	else
	{
		double answer = 0;
		for (int i = 0; i < base.size(); i++)
		{
			vector <vector <double> > key(base.size() - 1, vector <double>(base.size() - 1));
			int line = 0, column = 0;
			for (int j = 1; j < base.size(); j++)
			{
				column = 0;
				for (int k = 0; k < base.size(); k++)
					if (k != i) key[line][column++] = base[j][k];
				line++;
			}
			answer = answer + pow(-1, i) * base[0][i] * det(key);
		}
		return answer;
	}
}

int dim(vector <int>& index, vector<Point3D>& x)
{
	double coff = x[index[0]].x / x[index[0]].y;
	int e = 0;
	for (int i = 1; i < index.size(); i++) 
		if (x[index[i]].x / x[index[i]].y != coff) { e++; break; }

	coff = x[index[0]].x / x[index[0]].z;
	for (int i = 1; i < index.size(); i++)
		if (x[index[i]].x / x[index[i]].z != coff) { e++; break; }

	coff = x[index[0]].y / x[index[0]].z;
	for (int i = 1; i < index.size(); i++)
		if (x[index[i]].y / x[index[i]].z != coff) { e++; break; }
	return e;
}

bool compare(pair<Point2D, int>& a, pair<Point2D, int>& b)
{
	return a.first < b.first;
}

vector <int> getFace(vector <int>& source, vector <Point3D>& points, vector <vector <double> >& d)
{
	vector <pair<Point2D, int> > y(source.size());
	for (int i = 0; i < source.size(); i++) y[i] = { to2D(points[source[i]], d[0], d[1]), source[i] };

	sort(y.begin(), y.end(), compare);

	vector <int> answer(1, 0);
	int j = 1;
	while (j < y.size())
	{
		if (answer.size() < 2) { answer.push_back(j); j++; }
		else
		{
			int a = answer[answer.size() - 2], b = answer[answer.size() - 1];
			if (det({ {1, 1, 1}, {y[a].first.x, y[b].first.x, y[j].first.x}, {y[a].first.y, y[b].first.y, y[j].first.y} }) < 0) answer.pop_back();
			else { answer.push_back(j); j++; }
		}
	}

	vector <int> temp(1, int(y.size()) - 1);
	j = y.size() - 2;
	while (j >= 0)
	{
		if (temp.size() < 2) { temp.push_back(j); j--; }
		else
		{
			int a = temp[temp.size() - 2], b = temp[temp.size() - 1];
			if (det({ {1, 1, 1}, {y[a].first.x, y[b].first.x, y[j].first.x}, {y[a].first.y, y[b].first.y, y[j].first.y} }) < 0) temp.pop_back();
			else { temp.push_back(j); j--; }
		}
	}

	if (temp[temp.size() - 2] != answer[1])
	{
		for (int i = 1; i < temp.size() - 1; i++) answer.push_back(temp[i]);
		for (int i = 0; i < answer.size(); i++) answer[i] = y[answer[i]].second;
		return answer;
	}
	else
	{
		vector <double> temp;
		temp = d[0]; d[0] = d[1]; d[1] = d[2]; d[2] = temp;
		answer.clear();
		answer.shrink_to_fit();
		temp.clear();
		temp.shrink_to_fit();
		y.clear();
		y.shrink_to_fit();
		return getFace(source, points, d);
	}
}

void chooseAnOrthogonalSystem(vector<vector <double> >& d, Point3D& x, Point3D& y)
{
	d[2] = { x.x - y.x, x.y - y.y, x.z - y.z };

	if (d[2][1] == 0 && d[2][2] == 0)
	{
		d[0] = { 0, 1, 0 };
		d[1] = { 0, 0, 1 };
	}
	else if (d[2][1] == 0 && d[2][2] != 0)
	{
		d[0] = { 0, 1, 0 };
		d[1] = { 1, 0, -d[2][0] / d[2][2] };
	}
	else if (d[2][1] != 0 && d[2][2] == 0)
	{
		d[0] = { 0, 0, 1 };
		d[1] = { 1, -d[2][0] / d[2][1], 0 };
	}
	else
	{
		d[0] = { 0, -d[2][2] / d[2][1], 1 };
		d[1] = { 1, -d[2][0] * d[2][1] / (d[2][1] * d[2][1] + d[2][2] * d[2][2]), -d[2][0] * d[2][2] / (d[2][1] * d[2][1] + d[2][2] * d[2][2]) };
	}
}