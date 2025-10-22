#include <iostream>
#include <fstream>
#include <string>
#include <future>

// File for tests
void create_new_file() {
	std::ofstream o;
	o.open("temp.txt");
	for (int i = 0; i < 20000; i++) {
		o << i << std::endl;
	}
	o.close();
}

// Testing load (doubles input value)
int process(std::string& data) {
	int result = 0;
	for (int j = 0; j < 999999; j++) {
		int q = 0;
		int dec = 1;

		for (int i = data.size() - 1; i >= 0; i--) {
			int t = (data[i] - 48);
			q += (dec * t);
			dec *= 10;
		}
		if (j == 999998)
			result = q;
		q = 0;
		dec = 1;
	}

	data = std::to_string(2*result);
	return 1;
}

struct Threads {
	std::string data;

	void Process() {
		process(this->data);
	}	
} **MT;

void read_file() {
	int thread_count = std::thread::hardware_concurrency();
	std::thread* t = new std::thread[thread_count];
	int last_index = thread_count;

	MT = new struct Threads*[thread_count];
	for (int i = 0; i < thread_count; i++) {
		MT[i] = new struct Threads;
	}

	/* Multi Threads */
	clock_t t1 = clock();
	std::ifstream i1;
	i1.open("temp.txt");
	while (!i1.eof()) {
		for (int q = 0; q < thread_count; q++) {
			i1 >> MT[q]->data;
			if (i1.eof()) {
				last_index = q;
				break;
			}	
		}
		for (int q = 0; q < last_index; q++) {
			t[q] = std::thread(&Threads::Process, MT[q]);
		}
		
		for (int q = 0; q < last_index; q++) {
			t[q].join();
			std::cout << MT[q]->data << std::endl;
		}

	}
	i1.close();
	t1 = clock() - t1;
	for (int i = 0; i < thread_count; i++) {
		delete MT[i];
	}
	delete[] MT;

	/* Single thread */
	clock_t t2 = clock();
	std::string buffer;
	std::ifstream i2;
	i2.open("temp.txt");
	while (true) {
		i2 >> buffer;
		if (i2.eof())
			break;
		process(buffer);
		std::cout << buffer << std::endl;
	}
	i2.close();
	t2 = clock() - t2;

	std::cout << "Multi Threads: " << (((float)t1) / CLOCKS_PER_SEC) << " sec. \n"
					"Single thread: " << (((float)t2) / CLOCKS_PER_SEC) << " sec.\n";
}

int main() {
	create_new_file();
	read_file();
}