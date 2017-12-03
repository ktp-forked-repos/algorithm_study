#include <queue>
#include <algorithm>
#include <vector>

using namespace std;

#include "maxFlowPushRelabel.h"

/////////// For Testing ///////////////////////////////////////////////////////

#include <cassert>
#include <string>
#include <iostream>
#include "../common/iostreamhelper.h"

void testMaxFlowPushRelabel() {
    //return; //TODO: if you want to test a split function, make this line a comment.

    cout << "--- Max Flow - Push Relabel ---------" << endl;

    {
        MaxFlowPushRelabel<int> maxFlow(6);

        /*
        int in[][6] = {
        { 0, 16, 13, 0, 0, 0 },
        { 0, 0, 10, 12, 0, 0 },
        { 0, 4, 0, 0, 14, 0 },
        { 0, 0, 9, 0, 0, 20 },
        { 0, 0, 0, 7, 0, 4 },
        { 0, 0, 0, 0, 0, 0 }
        };
        */

        maxFlow.addEdge(0, 1, 16, 0);
        maxFlow.addEdge(0, 2, 13, 0);
        maxFlow.addEdge(1, 2, 10, 4);
        maxFlow.addEdge(1, 3, 12, 0);
        maxFlow.addEdge(2, 3, 0, 9);
        maxFlow.addEdge(2, 4, 14, 0);
        maxFlow.addEdge(3, 4, 0, 7);
        maxFlow.addEdge(3, 5, 20, 0);
        maxFlow.addEdge(4, 5, 4, 0);

        auto flow = maxFlow.calcMaxFlow(0, 5);
        cout << "Push Relabel : " << flow << endl;
        assert(flow == 23);
    }
    {
        MaxFlowPushRelabel<int> maxFlow(3);

        /*
        int in[][3] = {
        { 0, 3, 2 },
        { 0, 0, 2 },
        { 0, 0, 0 }
        };
        */

        maxFlow.addEdge(0, 1, 3, 0);
        maxFlow.addEdge(0, 2, 2, 0);
        maxFlow.addEdge(1, 2, 2, 0);

        auto flow = maxFlow.calcMaxFlow(0, 2);
        cout << "Push Relabel : " << flow << endl;
        assert(flow == 4);
    }

    cout << "OK!" << endl;
}
