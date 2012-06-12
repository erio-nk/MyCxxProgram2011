#include <stdio.h>
#include <algorithm>
#include <vector>

int main()
{
	std::vector<int> A;
	std::vector<int> B;

	A.push_back(0);
	A.push_back(1);
	A.push_back(2);
	A.push_back(3);
	//A.push_back(4);
	A.push_back(5);
	A.push_back(6);
	//A.push_back(7);
	A.push_back(8);
	//A.push_back(9);

	//B.push_back(0);
	B.push_back(1);
	B.push_back(2);
	//B.push_back(3);
	B.push_back(4);
	B.push_back(5);
	B.push_back(6);
	B.push_back(7);
	B.push_back(8);
	B.push_back(9);

	std::sort(A.begin(), A.end());	// ソート済みでないとDiffれない。
	std::sort(B.begin(), B.end());

	int a = 0, b = 0;
	std::vector<int> M;	// -リスト
	std::vector<int> P;	// +リスト
	while (true) {
		if (a == A.size() && b == B.size()) {
			break;	// 終了
		}
		if (a == A.size()) {
			P.push_back(B[b]);
			++b;
			continue;
		}
		if (b == B.size()) {
			M.push_back(A[a]);
			++a;
			continue;
		}
		if (A[a] == B[b]) {
			++a;
			++b;
			continue;
		}
		if (A[a] < B[b]) {
			M.push_back(A[a]);
			++a;
			continue;
		}
		if (A[a] > B[b]) {
			P.push_back(B[b]);
			++b;
			continue;
		}
	}
	
	printf("A : ");
	for (int i = 0; i < A.size(); ++i) {
		printf("%d, ", A[i]);
	}
	printf("\n");

	printf("B : ");
	for (int i = 0; i < B.size(); ++i) {
		printf("%d, ", B[i]);
	}
	printf("\n");
	
	printf("- : ");
	for (int i = 0; i < M.size(); ++i) {
		printf("%d, ", M[i]);
	}
	printf("\n");

	printf("+ : ");
	for (int i = 0; i < P.size(); ++i) {
		printf("%d, ", P[i]);
	}
	printf("\n");

	return 0;
}
