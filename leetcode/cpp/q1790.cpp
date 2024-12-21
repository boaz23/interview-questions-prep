#include <string>
#include <iostream>

class Solution {
public:
	bool areAlmostEqual(std::string s1, std::string s2) {
		if (s1.length() != s2.length()) {
			return false;
		}

		char sc1, sc2;
		size_t mismatchCount = 0;
		bool foundSwap = false;

		for (size_t i = 0; i < s1.length(); i++) {
			if (s1[i] == s2[i]) {
				continue;
			}

			mismatchCount++;
			switch (mismatchCount) {
			case 1:
				sc1 = s1[i];
				sc2 = s2[i];
				break;

			case 2:
				if (s1[i] == sc2 && s2[i] == sc1) {
					foundSwap = true;
					break;
				}
				/* fall through */

			case 3:
				goto end;
			}
		}

end:
		return (mismatchCount == 0) || ((mismatchCount == 2) && foundSwap);
	}
};

static size_t failureCount = 0;

void runTestCase(
	const std::string &s1,
	const std::string &s2,
	bool expectedResult
) {
	bool result = Solution{}.areAlmostEqual(s1, s2);
	if (expectedResult != result) {
		std::cerr << "Failed test case:" << std::endl;
		std::cerr << "  input           = " << "\"" << s1 << "\"" << ", " << "\"" << s2 << "\"" << std::endl;
		std::cerr << "  actual result   = " << result         << std::endl;
		std::cerr << "  expected result = " << expectedResult << std::endl;
		failureCount++;
	}
}

int main(int argc, char *argv[]) {
	(void)argc;
	(void)argv;

	runTestCase("bank", "kanb", true);
	runTestCase("attack", "defend", false);
	runTestCase("kelb", "kelb", true);
	runTestCase("badnk", "kasnb", false);
	runTestCase("bankd", "kasnb", false);
	runTestCase("bankd", "kanbs", false);
	runTestCase("bankk", "kanbb", false);

	if (failureCount == 0) {
		std::cout << "All tests passed" << std::endl;
	}

	return 0;
}
