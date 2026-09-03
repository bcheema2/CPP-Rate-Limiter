#include <iostream>
#include "TokenBucket.h"

using namespace std;


int main() {
    TokenBucket tb(10);
    char r;

    cout << "Enter a request: ";
    while ((cin >> r) && r != 'q') {

        if (tb.request()) {
            cout << "Request allowed. " << "Tokens left: " << tb.get_tokens() << "\n";
        }
        else {
            cout << "Request denied. 0 tokens left. " << "\n";
        }
    }

    cout << "\n-----Final Statistics ----\n";
    cout << "Total Allowed: " <<tb.get_allowed_count() <<"\n";
    cout << "Total Denied: " <<tb.get_denied_count() <<"\n";

    return 0;
}
