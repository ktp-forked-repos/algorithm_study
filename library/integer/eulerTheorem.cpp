#include <cmath>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

#include "eulerTheorem.h"
#include "primeFactor.h"
#include "primeNumberBasic.h"


/////////// For Testing ///////////////////////////////////////////////////////

#include <time.h>
#include <cassert>
#include <string>
#include <iostream>
#include "../common/iostreamhelper.h"
#include "../common/profile.h"

#define NN  1000000

void testEulerTheorem() {
    return; //TODO: if you want to test functions of this file, make this line a comment.

    cout << "--- Euler Theorem -----------------------" << endl;
    {
        //int N = 1000000009;
        int N = 1009;
        for (int i = 1; i <= N; i++) {
            auto ans1 = phi(i);
            auto ans2 = phiFast(i);
            if (ans1 != ans2)
                cerr << "Mismatched!" << endl;
            assert(ans1 == ans2);
        }
    }

    cout << "OK!" << endl;
}
