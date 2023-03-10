#include <SFML/Graphics.hpp>
#include <iostream>
#include <time.h>
#include <HashLife.h>
#include <vector>

#define WHITE sf::Color::White
#define BLACK sf::Color::Black
#define GRAY sf::Color(153, 153, 153)

 struct Node;
 const int CELL_SIZE = 5;
 const int n = 7;
 int GRID_WIDTH = 128;
 int GRID_HEIGHT = 128;
 std::vector<Node> unpack;
 std::vector<Node> package;

 struct Node //node used for printing
{
    Point p;
    int Live;
    Node(Point pt, int a)
    {
        p = pt;
        Live = a;
    }
};

 int countLiveCells(std::vector<Node> nodes, Point p1, Point p2)//to count number of live cells in the given quad
 {
     int c = 0;
     for (int i = p1.x;i < p2.x;i++)
         for (int j = p2.y;j < p1.y;j++)
         {
             c += nodes[i + j * GRID_HEIGHT].Live;
         }
     return c;
 }

 Quad* create( Quad* node,Point p1, Point p2, int k, std::vector<Node> nodeList)//Given a list of nodes, conversion to Quadtree
 {
     if (k<0)
     {
         return node;
     }
     if (node==NULL)
     {
         Quad* q = new Quad(p1, p2, k, countLiveCells(nodeList, p1, p2));
         node = q;
     }
     
     node->a = create(node->a, Point(p1.x, p1.y), Point((p1.x + p2.x) / 2, (p1.y + p2.y) / 2), k - 1, nodeList);
     node->b = create(node->b, Point((p1.x + p2.x) / 2, p1.y), Point(p2.x, (p1.y + p2.y) / 2), k - 1, nodeList);
     node->c = create(node->c, Point(p1.x, (p1.y + p2.y) / 2), Point((p1.x + p2.x) / 2, p2.y), k - 1, nodeList);
     node->d = create(node->d, Point((p1.x + p2.x) / 2, (p1.y + p2.y) / 2), Point(p2.x, p2.y), k - 1, nodeList);
     return node;
 }

void quadTovector(Quad* node)//Given a quad, conversion to list of nodes for printing
{
    if (node == NULL)
    {
        return;
    }
    else if (node->k_level == 0 )
    {
        unpack.push_back(Node(Point(node->topLeft.x, node->botRight.y), node->noOfLiveNode));
        return;
    }
    quadTovector(node->a);
    quadTovector(node->b);
    quadTovector(node->c);
    quadTovector(node->d);
}

int count=0;
int f = 2;
int main()
{
    const sf::Vector2<float> CELL_VECTOR(CELL_SIZE, CELL_SIZE);//vector of left bottom corner point of each cell to be printed
    int N_CELLS = GRID_WIDTH * GRID_HEIGHT;//Total number of cells tobe printed
   
    //random grid cells formation
    std::vector<int> grid(N_CELLS, 0);//for printing
    srand(time(NULL));
    for (int i = 0; i < N_CELLS; i++)
       grid[i] = (rand()%10) ? 0 : 1;

    Quad* root = NULL;//root of the quad tree

    int delay = 100;

    sf::RenderWindow window;//declaration of graphic window
    do
    {
        std::cout << count << " Generation\n";
        count += pow(2, (n - f));
        f += 2;

        window.create(sf::VideoMode(CELL_SIZE * GRID_WIDTH, CELL_SIZE * GRID_HEIGHT), "Game of Life");

        sf::Event event;
        while (window.pollEvent(event))
        {
            switch (event.type)
            {
            case sf::Event::Closed:
                window.close();
                break;
            }
        }
        //printing grids
        window.clear(WHITE);
        for (int x = 0; x < GRID_WIDTH; x++)
        {
            for (int y = 0; y < GRID_HEIGHT; y++)
            {
                sf::RectangleShape cell;
                cell.setPosition(x * CELL_SIZE, y * CELL_SIZE);
                cell.setSize(CELL_VECTOR);
                cell.setOutlineThickness(1);
                cell.setOutlineColor(WHITE);
                package.push_back(Node(Point(x, y), grid[x + y * GRID_WIDTH]));
                if (grid[x + y * GRID_WIDTH] == 1)
                    cell.setFillColor(BLACK);
                else
                    cell.setFillColor(GRAY);
                window.draw(cell);

            }
        }
        window.display();

        root = create(root, Point(0, GRID_HEIGHT), Point(GRID_WIDTH, 0), n, package);
        Quad* nextQuad = successor(root);
        quadTovector(nextQuad);
        
        //grids are updated
        grid.assign(grid.size(),0);
            for (std::vector<Node>::iterator i = unpack.begin(); i != unpack.end(); ++i)
            {
                grid[i->p.x + i->p.y * GRID_HEIGHT] = i->Live;
            }
        unpack.clear();
        sf::sleep(sf::milliseconds(delay));
    } while (window.isOpen());
}