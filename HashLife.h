#include<Quad.h>

struct hash//to store details of node for hashing
{
    Quad* node;
    Quad* cachedNode;
    int hNo;
    hash(Quad* n, Quad* c)
    {
        node = n;
        cachedNode = c;
        hNo = 3*n->k_level+5*n->noOfLiveNode;
    }
    hash()
    {
        node = NULL;
        cachedNode = NULL;
        hNo = -1;
    }
};
int size = 189200;
std::vector<struct hash> list(size);

Quad* join(Quad* a, Quad* b, Quad* c, Quad* d)//function to join for nodes 
{
    int N = a->noOfLiveNode + b->noOfLiveNode + c->noOfLiveNode + d->noOfLiveNode;
    Quad* t = new Quad(a->k_level + 1, a, b, c, d, N,a->topLeft,d->botRight);
    return t;
}



int count_live_neighbour_cell(int a[3][3], int r, int c)//to count the number of live cells in the neigbour of centre node
{
    int i, j, count = 0;
    for (i = r - 1; i <= r + 1; i++) {
        for (j = c - 1;j <= c + 1;j++) 
        {
            if ((i == r && j == c) || (i < 0 || j < 0) || (i >= 3 || j >= 3)) {
                continue;
            }
            if (a[i][j] == 1) {
                count++;
            }
        }
    }
    return count;
}
Quad* life(Quad* a, Quad* b, Quad* c, Quad* d, Quad* e, Quad* f, Quad* g, Quad* h, Quad* i)//Game of life for 16*16
{
    int neighbour_live_cell;
    int arr[3][3];

    arr[0][0] = a->noOfLiveNode;
    arr[0][1] = b->noOfLiveNode;
    arr[0][2] = c->noOfLiveNode;
    arr[1][0] = d->noOfLiveNode;
    arr[1][1] = e->noOfLiveNode;
    arr[1][2] = f->noOfLiveNode;
    arr[2][0] = g->noOfLiveNode;
    arr[2][1] = h->noOfLiveNode;
    arr[2][2] = i->noOfLiveNode;

    neighbour_live_cell = count_live_neighbour_cell(arr, 1, 1);
    if (arr[1][1] == 1 && (neighbour_live_cell == 2 || neighbour_live_cell == 3))
    {
        Quad* q = new Quad(a->topLeft, i->botRight, 0, 1);
        return q ;
    }

    else if (arr[1][1] == 0 && neighbour_live_cell == 3)
    {
        Quad* q = new Quad(a->topLeft, i->botRight, 0, 1);
        return q;
    }

    else
    {
        Quad* q = new Quad(a->topLeft, i->botRight, 0, 0);
        return q;
    }

}
Quad* life_4(Quad* m)
{
    Quad* ad = life(m->a->a, m->a->b, m->b->a, m->a->c, m->a->d, m->b->c, m->c->a, m->c->b, m->d->a);
    Quad* bc = life(m->a->b, m->b->a, m->b->b, m->a->d, m->b->c, m->b->d, m->c->b, m->d->a, m->d->b);
    Quad* cb = life(m->a->c, m->a->d, m->b->c, m->c->a, m->c->b, m->d->a, m->c->c, m->c->d, m->d->c);
    Quad* da = life(m->a->d, m->b->c, m->b->d, m->c->b, m->d->a, m->d->b, m->c->d, m->d->c, m->d->d);

    return join(ad, bc, cb, da);
}
Quad* successor(Quad* m)
{
    if (areEqual(list[(3 * m->k_level + 5 * m->noOfLiveNode)%size].node, m))
    {
        return list[3 * m->k_level + 5 * m->noOfLiveNode].cachedNode;
    }
    
    Quad* s;

    if (m->noOfLiveNode == 0)
        return m->a;
    else if (m->k_level == 2)
        s = life_4(m);
    else
    {
        Quad* c1 = successor(join(m->a->a, m->a->b, m->a->c, m->a->d));
        Quad* c2 = successor(join(m->a->b, m->b->a, m->a->d, m->b->c));
        Quad* c3 = successor(join(m->b->a, m->b->b, m->b->c, m->b->d));
        Quad* c4 = successor(join(m->a->c, m->a->d, m->c->a, m->c->b));
        Quad* c5 = successor(join(m->a->d, m->b->c, m->c->b, m->d->a));
        Quad* c6 = successor(join(m->b->c, m->b->d, m->d->a, m->d->b));
        Quad* c7 = successor(join(m->c->a, m->c->b, m->c->c, m->c->d));
        Quad* c8 = successor(join(m->c->b, m->d->a, m->c->d, m->d->c));
        Quad* c9 = successor(join(m->d->a, m->d->b, m->d->c, m->d->d));

        s = join(successor(join(c1, c2, c4, c5)), successor(join(c2, c3, c5, c6)), successor(join(c4, c5, c7, c8)), successor(join(c5, c6, c8, c9)));
    }
    list.insert(list.begin()+((3*m->k_level+5*m->noOfLiveNode)%size),hash(m,s));
    return s;
}


