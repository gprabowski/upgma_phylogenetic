#include <vector>
#include <algorithm>
#include <string>
#include <iostream>

using namespace std;

int& getElem(vector<int> &v, int x, int y, int dim) {
	return v[x * dim + y];
}

int minOperations(string& s1, string& s2) {
	vector<int> memo((s1.length() + 1) * (s2.length() + 1));
	for(int a = 0; a < s1.length() + 1; ++a)
		memo[a * (s2.length() + 1) + 0] = a;	
	for(int a = 0; a < s2.length() + 1; ++a)
		memo[a] = a;
	for(int i = 1; i < s1.length() + 1; ++i) {
		for(int j = 1; j < s2.length() + 1; ++j){
			if(s1[i - 1] == s2[j - 1]) {
					memo[i * (s2.length() + 1) + j] = memo[(i - 1) * (s2.length() + 1) + j - 1];
					continue;
			}
			memo[i * (s2.length() + 1) + j] = min({
						      memo[i * (s2.length() + 1) + j -1] + 1,
							  memo[(i - 1) * (s2.length() + 1) +j] + 1,
							  memo[(i - 1) * s2.length() + (j - 1)] + 1
							  });

		}
	}
	return memo[(s1.length() + 1) * (s2.length() + 1) - 1];
}

int main(int argc, char** argv) {
		if(argc != 3) {
			cout << "Please provide two strings as cmd arguments" << endl;
			exit(1);
		}
		string s1 = argv[1];
		string s2 = argv[2];
		
		cout << "the result is " << minOperations(s1, s2) << endl;
		
		return 0;
}
