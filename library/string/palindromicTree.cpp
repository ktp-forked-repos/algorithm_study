#include <vector>
#include <string>
#include <algorithm>

using namespace std;

#include "palindromicTree.h"

/////////// For Testing ///////////////////////////////////////////////////////

#include <cassert>
#include <iostream>
#include "../common/iostreamhelper.h"

#include "palindromicString.h"

//template <typename T>
//vector<int> getPalindromes(T& s, int n);

void testPalindromicTree() {
    //return; //TODO: if you want to test string functions, make this line a comment.

    vector<string> in{
        "a",
        "aa",
        "aaa",
        "aba",
        "abc",
        "abcca",
        "abca"
    };

    cout << "-- Palindromic Tree ---" << endl;
    for (auto s : in) {
        PalindromicTree tree(10);

        int gt = countPalindromicSubstring(s, (int)s.length());
        int ans = (int)tree.count(s);
        if (ans != gt) {
            cout << "gt = " << gt << ", ans = " << ans << endl;
        }
        assert(ans == gt);
    }

    for (int i = 0; i < 10; i++) {
        string s;
        for (int j = 0; j < 100; j++)
            s += 'a' + rand() % 3;

        vector<int> gt((int)s.length());
        int prevN = 0;
        for (int i = 0; i < (int)s.length(); i++) {
            int currN = countPalindromicSubstring(s, i + 1);
            gt[i] = currN - prevN;
            prevN = currN;
        }

        PalindromicTree tree((int)s.length());
        vector<int> ans = tree.countAll(s);
        if (ans != gt) {
            cout << s << endl;
            cout << "gt = " << gt << ", ans = " << ans << endl;
        }
        assert(ans == gt);
    }

    cout << "OK!" << endl;
}
