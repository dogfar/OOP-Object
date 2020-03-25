#include<bits/stdc++.h>
#include "IntToString.h"
using namespace std;

string IntToString(int n) {
	ostringstream stream;
	stream << n;
	return stream.str();
}
