#include <fstream>
#include <vector>
#include <iostream>
#include <cmath>
#include <algorithm>

struct Point2D
{
	double x, y;
	Point2D()
	{
		x = 0;
		y = 0;
	}
};

struct Point3D
{
	double x, y, z;
};

bool operator < (Point2D& a, Point2D& b);
bool operator ==(Point2D& a, Point2D& b);

std::ifstream& operator >>(std::ifstream& in, Point3D& a);
std::ofstream& operator <<(std::ofstream& out, Point3D& a);
std::ostream& operator <<(std::ostream& out, Point3D& a);

bool operator ==(Point3D& a, Point3D& b);

Point2D to2D(Point3D& base, std::vector <double>& k, std::vector <double>& l);

double det(std::vector <std::vector <double> > base);
int dim(std::vector <int>& index, std::vector<Point3D>& x);

bool compare(std::pair<Point2D, int>& a, std::pair<Point2D, int>& b);

std::vector <int> getFace(std::vector <int>& source, std::vector <Point3D>& points, std::vector<std::vector <double> >& d);

void chooseAnOrthogonalSystem(std::vector< std::vector <double> >& d, Point3D& x, Point3D& y);