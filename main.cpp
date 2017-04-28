//
// Test.cpp
//
// This is a direct port of the C version of the RTree test program.
//

#include <stdio.h>
#include "RTree.h"

struct Rect
{
    Rect()  {}
    
    Rect(float a_minX, float a_minY, float a_maxX, float a_maxY)
    {
        min[0] = a_minX;
        min[1] = a_minY;
        
        max[0] = a_maxX;
        max[1] = a_maxY;
    }
    
    
    float min[2];
    float max[2];
};

struct Rect rects[] =
{
    Rect(2, 2, 2, 2), // xmin, ymin, xmax, ymax (for 2 dimensional RTree)
    Rect(5, 7, 5, 7),
    Rect(8, 5, 8, 5),
    Rect(9, 2, 9, 2),
    Rect(8, 12, 8, 12),
    Rect(8, 12.4, 8, 12.4),
    Rect(1, 2, 1, 2),
    Rect(1, 1, 1, 1),
};

int nrects = sizeof(rects) / sizeof(rects[0]);

Rect search_rect(8.2, 10, 8.35, 12); // search will find above rects that this one overlaps


bool MySearchCallback(int id, void* arg)
{
    printf("Hit data rect %d\n", id);
    return true; // keep going
}

bool MyKNNSearchCallback(int id, void* arg)
{
    printf("KNN data rect %d\n", id);
    return true; // keep going
}


int main()
{
    RTree<int, float, 2, float> tree;
    
    int i, nhits;
    printf("nrects = %d\n", nrects);
    
    for(i=0; i<nrects; i++)
    {
        tree.Insert(rects[i].min, rects[i].max, i); // Note, all values including zero are fine in this version
    }
    
    nhits = tree.Search(search_rect.min, search_rect.max, MySearchCallback, NULL);
    
    printf("Search resulted in %d hits\n", nhits);
    
    // Iterator test
    int itIndex = 0;
    RTree<int, float, 2, float>::Iterator it;
    for( tree.GetFirst(it);
        !tree.IsNull(it);
        tree.GetNext(it) )
    {
        int value = tree.GetAt(it);
        
        float boundsMin[2] = {0,0};
        float boundsMax[2] = {0,0};
        it.GetBounds(boundsMin, boundsMax);
        printf("it[%d] %d = (%f,%f,%f,%f,%f)\n", itIndex++, value, boundsMin[0], boundsMin[1], boundsMax[0], boundsMax[1],
               (boundsMin[0] - 9) * (boundsMin[0] - 9) + (boundsMin[1] - 9) * (boundsMin[1] - 9));
    }
    
    // Iterator test, alternate syntax
    itIndex = 0;
    tree.GetFirst(it);
    while( !it.IsNull() )
    {
        int value = *it;
        ++it;
        printf("it[%d] %d\n", itIndex++, value);
    }
    
    Rect KNNsearch_rect(9, 9, 0, 0);
    tree.KNNSearch(KNNsearch_rect.min, MyKNNSearchCallback);
    
    getchar(); // Wait for keypress on exit so we can read console output
    
    // Output:
    //
    // nrects = 4
    // Hit data rect 1
    // Hit data rect 2
    // Search resulted in 2 hits
    // it[0] 0 = (0,0,2,2)
    // it[1] 1 = (5,5,7,7)
    // it[2] 2 = (8,5,9,6)
    // it[3] 3 = (7,1,9,2)  
    // it[0] 0
    // it[1] 1
    // it[2] 2
    // it[3] 3
}
