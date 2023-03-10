
#include <cmath>

// Used to hold details of a point
struct Point
{
	int x;
	int y;
	Point(int _x, int _y)
	{
		x = _x;
		y = _y;
	}
	Point()
	{
		x = 0;
		y = 0;
	}
};

// The objects that we want stored in the quadtree

// The main quadtree class
class Quad
{
public:
	// Hold details of the boundary of this Quad
	Point topLeft;
	Point botRight;
	int k_level;
	int noOfLiveNode;
	// Children of this node
	Quad* a;
	Quad* b;
	Quad* c;
	Quad* d;


public:
	//Constructors of QuadTree
	Quad()
	{
		topLeft = Point(0, 0);
		botRight = Point(0, 0);
		a = NULL;
		b = NULL;
		c = NULL;
		d = NULL;
		k_level = 0;
		noOfLiveNode = 0;
		
	}
	Quad(Point topL, Point botR, int k, int n)
	{
		a = NULL;
		b = NULL;
		c = NULL;
		d = NULL;
		topLeft = topL;
		botRight = botR;
		k_level = k;
		noOfLiveNode = n;
		
	}


	Quad(int p, Quad* q, Quad* r, Quad* s, Quad* t, int n, Point x,Point y)
	{
		topLeft = x;
		botRight = y;
		a = q;
		b = r;
		c = s;
		d = t;
		k_level = p;
		noOfLiveNode = n;
	}
	
};

//to check if 2 trees are equal
bool areEqual(Quad* a, Quad* b)
{
	if (a == NULL && b == NULL)
		return 1;
	if (a != NULL && b != NULL)
	{
		return
			(a->k_level == b->k_level && a->noOfLiveNode == b->noOfLiveNode && areEqual(a->a, b->a) && areEqual(a->b, b->b) && areEqual(a->c, b->c) && areEqual(a->d, b->d));
	}
	return 0;
}




