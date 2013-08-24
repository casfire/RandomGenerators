#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "RandomGenerators.hpp"

#define TESTS 10223616 + 1
#define MIN(a, b) ((a < b)?a:b)

void printBinary(uint32_t value) {
	for(size_t i = 0; i < 32; i++){
		printf("%c", (value & (0x80000000 >> i))?'1':'0');
	}
}

clock_t benchmark(Random::Generator &generator, uint32_t &lastValue) {
	clock_t start, end;
	uint32_t value;
	start = clock();
	for(size_t n = 0; n < TESTS; n++)value = generator.next();
	end = clock();
	lastValue = value;
	return end - start;
}

void printBenchmark(const char *name, clock_t min, clock_t ticks, uint32_t lastValue) {
	printf("%s | %6.2f seconds | ", name, (float)ticks / CLOCKS_PER_SEC);
	if(min == ticks)printf("      fastest | ");
	else printf("%5.2f%% slower | ", 100.f - 100.f * (float)min / ticks);
	printBinary(lastValue);
	printf("\n");
}

int main() {

	Random::STDLIB stdlib;
	Random::LCG lcg;
	Random::MWC mwc;
	Random::WELL well;
	Random::MT mt;

	clock_t c_stdlib = 0, c_lcg = 0, c_mwc = 0, c_well = 0, c_mt = 0;
	uint32_t l_stdlib, l_lcg, l_mwc, l_well, l_mt;

	while(1){
		c_stdlib += benchmark(stdlib, l_stdlib);
		c_lcg += benchmark(lcg, l_lcg);
		c_mwc += benchmark(mwc, l_mwc);
		c_well += benchmark(well, l_well);
		c_mt += benchmark(mt, l_mt);

		clock_t min = MIN(MIN(c_stdlib, c_lcg), MIN(c_mwc, MIN(c_well, c_mt)));

		printf("\n   Name | total time ran | compare speed | last generated value\n\n");
		printBenchmark(" STDLIB", min, c_stdlib, l_stdlib);
		printBenchmark("    LCG", min, c_lcg, l_lcg);
		printBenchmark("    MWC", min, c_mwc, l_mwc);
		printBenchmark("   WELL", min, c_well, l_well);
		printBenchmark("     MT", min, c_mt, l_mt);
	}

	return 0;
}
