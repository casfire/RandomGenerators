#include <iostream>
#include <iomanip>
#include <ctime>
#include <bitset>
#include <cstdlib>
#include "Random.hpp"

#define TESTS 10223617
#define MIN(a, b) ((a < b) ? a : b)

std::clock_t benchmark(Random::Generator &generator, std::uint32_t &lastValue) {
	std::clock_t start, end;
	std::uint32_t value;
	start = std::clock();
	for (std::size_t n = 0; n < TESTS; n++) value = generator.next();
	end = std::clock();
	lastValue = value;
	return end - start;
}

std::clock_t benchmark_rand(std::uint32_t &lastValue) {
	std::clock_t start, end;
	std::uint32_t value;
	start = std::clock();
	for (std::size_t n = 0; n < TESTS; n++) value = std::rand();
	end = std::clock();
	lastValue = value;
	return end - start;
}

void printBenchmark(std::ostream &out, const char *name, std::clock_t min, std::clock_t ticks, std::uint32_t lastValue) {
	float seconds = static_cast<float>(ticks) / static_cast<float>(CLOCKS_PER_SEC);
	float speed = static_cast<float>(ticks) / static_cast<float>(min);
	std::bitset<32> bits(lastValue);
	out << name << " | ";
	out << std::fixed << std::setw(12) << std::setprecision(3) << seconds;
	out << " s | ";
	out << std::fixed << std::setw(14) << std::setprecision(8) << speed;
	out << " | " << bits;
	out << "\n";
}

int main() {
	std::uint32_t seed = std::time(0);
	std::srand(seed);
	Random::STDLIB stdlib(seed);
	Random::LCG lcg(seed);
	Random::MWC mwc(seed);
	Random::WELL well(seed);
	Random::MT mt(seed);

	std::clock_t c_stdlib = 0, c_lcg = 0, c_mwc = 0, c_well = 0, c_mt = 0, c_rand = 0;
	std::uint32_t l_stdlib, l_lcg, l_mwc, l_well, l_mt, l_rand;

	while (true) {
		c_rand += benchmark_rand(l_rand);
		c_stdlib += benchmark(stdlib, l_stdlib);
		c_lcg += benchmark(lcg, l_lcg);
		c_mwc += benchmark(mwc, l_mwc);
		c_well += benchmark(well, l_well);
		c_mt += benchmark(mt, l_mt);

		std::clock_t min = MIN(MIN(c_rand, c_stdlib), MIN(MIN(c_lcg, c_mwc), MIN(c_well, c_mt)));
		
		std::cout << "   name | total time ran | relative speed | last generated value\n\n";
		printBenchmark(std::cout, " rand()", min, c_rand, l_rand);
		printBenchmark(std::cout, " STDLIB", min, c_stdlib, l_stdlib);
		printBenchmark(std::cout, "    LCG", min, c_lcg, l_lcg);
		printBenchmark(std::cout, "    MWC", min, c_mwc, l_mwc);
		printBenchmark(std::cout, "   WELL", min, c_well, l_well);
		printBenchmark(std::cout, "     MT", min, c_mt, l_mt);
		std::cout << std::endl;
	}

	return 0;
}
