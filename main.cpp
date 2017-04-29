//
// Test.cpp
//
// This is a direct port of the C version of the RTree test program.
//

#include <stdio.h>
#include <fstream>
#include "RTree.h"

#define NUMDIMS 3
#define FEATUREDIMS 9

class Point
{
public:
    float data[FEATUREDIMS];
};

void ReadFeatures(std::ifstream &a_input, int totalNums, Point* a_pointArray)
{
    for(int i = 0; i < totalNums; i++)
        for(int j = 0; j < FEATUREDIMS; j++)
            a_input >> a_pointArray[i].data[j];
}


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
    RTree<int, float, 3, float> tree;
    
    int totalNums;
    
    std::ifstream m_input("/Users/AGLAX/Desktop/R-T/R-T/features.txt");
    if(!m_input.is_open())
        printf("no input!\n");
        
    m_input >> totalNums;
    printf("totalNums = %d\n", totalNums);
    
    Point* m_pointArray = new Point[totalNums];
    ReadFeatures(m_input, totalNums, m_pointArray);
    
    for(int i = 0; i <totalNums; i++)
    {
        tree.Insert(m_pointArray[i].data, m_pointArray[i].data, i); // Note, all values including zero are fine in this version
        printf("%d\n", i);
    }
    
    float searchRange[2][3] = {{1000, 1000, 1000}, {9000, 9000, 9000}};
    int nhits = tree.Search(searchRange[0], searchRange[1], MySearchCallback, NULL);
    printf("Search resulted in %d hits\n", nhits);

    float kNNSearchPoint[3] = {5000, 5000, 5000};
    tree.KNNSearch(kNNSearchPoint, MyKNNSearchCallback, 7);
    
    delete [] m_pointArray;
    
    /*
    // Iterator test
    int itIndex = 0;
    RTree<int, float, 3, float>::Iterator it;
    for( tree.GetFirst(it);
        !tree.IsNull(it);
        tree.GetNext(it) )
    {
        int value = tree.GetAt(it);
        
        float boundsMin[3] = {0,0,0};
        float boundsMax[3] = {0,0, 0};
        it.GetBounds(boundsMin, boundsMax);
        printf("it[%d] %d = (%f,%f,%f,%f,%f, %f, %f)\n", itIndex++, value, boundsMin[0], boundsMin[1],  boundsMin[2], boundsMax[0], boundsMax[1], boundsMax[2],
               (boundsMin[0] - 9) * (boundsMin[0] - 9) + (boundsMin[1] - 9) * (boundsMin[1] - 9) + (boundsMin[2] - 9) * (boundsMin[2] - 9));
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
    
    Rect KNNsearch_rect(9, 9, 9, 0, 0, 0);
    tree.KNNSearch(KNNsearch_rect.min, MyKNNSearchCallback, 3);*/
    
    getchar(); // Wait for keypress on exit so we can read console output
    
}
