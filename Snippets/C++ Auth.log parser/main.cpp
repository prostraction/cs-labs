#include <iostream>
#include <fstream>
#include <string>

void Parse(std::string& word, const std::string* F, const int& f_size, const std::string* L, const int& l_size, const int& f_current, const int& l_current) {
	if (f_current < f_size) {
		if (f_current + 1 == f_size && (F[f_current] == "*" || word.find(F[f_current]) != std::string::npos)) {
			std::string verify;
			for (auto i = 0; i < f_size; i++) {
				if (F[i] == "*" && i > 0 && i < f_size - 1) {
					verify += word.substr(word.find(F[i - 1]) + F[i - 1].length() + 1, word.find(F[i+1]) - (word.find(F[i - 1]) + F[i - 1].length() + 2));
				}
				else if (F[i] == "*" && i > 0 && i < f_size == 1) {
					verify += word.substr(word.find(F[i - 1]) + F[i - 1].length() + 1, word.find(L[0]) - (word.find(F[i - 1]) + F[i - 1].length() + 2));
				}
				else {
					verify += F[i];
				}
				if (i + 1 != f_size) {
					verify += " ";
				}
			}
			
			if (word.find(verify) != std::string::npos) {
				if (F[f_current] == "*") {
					word = word.substr(0, word.find(L[l_current]));
					word = word.substr(word.find(F[f_current - 1]) + F[f_current - 1].size() + 1, word.length() - word.find(L[l_current]));
					word = word.substr(word.find(" ") + 1, word.length());
					while (word[word.length() - 1] == ' ') {
						word = word.substr(0, word.length() - 1);
					}
				}
				else {
					word = word.substr(0, word.find(L[l_current]));
					word = word.substr(word.find(F[f_current]) + F[f_current].length(), word.size() - word.find(L[l_current]));
				}
				
			}
			else {
				word.clear();
			}
		}
		else if (F[f_current] == "*" || word.find(F[f_current]) != std::string::npos) {
			Parse(word, F, f_size, L, l_size, f_current + 1, l_current);
		}
		else {
			word.clear();
		}
	}

}

int main() {
	std::ofstream output;
	output.open("kek.log");
	std::ifstream input;
	input.open("auth.log");

	if (!input.is_open()) {
		fprintf(stderr, "Error opening in\n"); return 1;
	}

	if (!output.is_open()) {
		fprintf(stderr, "Error opening out\n"); return 1;
	}

	input.seekg(0, input.end);
	auto size = input.tellg();
	input.clear();
	input.seekg(0);
	std::cout << size << " bytes\n";

	char* buffer = new char[size];
	input.read(buffer, size);
	if (!input)
		std::cout << "Error: only " << input.gcount() << " was read\n";
	input.close();

	std::string word;
	std::string First[3] = { "Failed", "*" , "for"};
	std::string Last[1] = { "from" };

	bool removeLine = true;
	std::string RemoveLine = "invalid user";

	for (auto i = 0; i < size; i++) {
		if (buffer[i] >= (char)' ') {
			word.insert(word.end(), buffer[i]);
		}
		else if (buffer[i] == '\n') { 
			Parse(word, First, 3, Last, 1, 0, 0);

			if (removeLine) {
				if (word.length() > RemoveLine.length()) {
					word = word.substr(word.find(RemoveLine) + RemoveLine.length(), word.length());
				}
			}
			
			while (word.length() > 0 && word[0] == ' ') {
				word = word.substr(1, word.length());
			}
			if (word.length() > 0) {
				output << word << "\n";
			}
			word.clear();
		}

	}
	return 0;
}