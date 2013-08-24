#include "RandomGenerators.hpp"

#if _RANDOMGENERATORS_VER_ != 100
	#warning Invalid RandomGenerators header version
#endif

namespace Random {

/* Generator */

float Generator::frand(float min, float max) {
	return min + ((float)this->next() / this->max()) * (max - min);
}

double Generator::drand(double min, double max) {
	return min + ((double)this->next() / this->max()) * (max - min);
}

void Generator::fill(size_t size, uint8_t *data) {
	while(size > 0){
		if(size < 4){
			uint32_t t = next();
			for(size_t n = 0; n < size; n++){
				data[n] = ((uint8_t*)(&t))[n];
			}
			return;
		}else{
			*((uint32_t*)data) = next();
			data += 4;
			size -= 4;
		}
	}
}

void Generator::fill(size_t size, uint32_t *data) {
	for(size_t n = 0; n < size; n++){
		data[n] = next();
	}
}

/* STDLIB */

STDLIB::STDLIB() {
	srand(0);
}

STDLIB::STDLIB(uint32_t seed) {
	srand(seed);
}

STDLIB::STDLIB(Generator &filler) {
	srand(filler);
}

void STDLIB::srand(uint32_t seed) {
	this->seed = seed;
}

void STDLIB::srand(Generator &filler) {
	seed = filler.next();
}

uint32_t STDLIB::next() {
	seed = seed * 0x343FD + 0x269EC3;
	return (seed >> 0x10) & 0x7FFF;
}

uint32_t STDLIB::max() {
	return 0x7FFF;
}

/* LCG */

LCG::LCG() {
	srand(0);
}

LCG::LCG(uint32_t seed) {
	srand(seed);
}

LCG::LCG(Generator &filler) {
	srand(filler);
}

void LCG::srand(uint32_t seed) {
	this->seed = seed;
}

void LCG::srand(Generator &filler) {
	seed = filler.next();
}

uint32_t LCG::next() {
	return (seed = 1664525 * seed + 1013904223);
}

uint32_t LCG::max() {
	return 0xFFFFFFFFU;
}

/* MWC */

MWC::MWC() {
	srand(0);
}

MWC::MWC(uint32_t seed) {
	srand(seed);
}

MWC::MWC(Generator &filler) {
	srand(filler);
}

MWC::MWC(const uint32_t (&seed)[4096]) {
	srand(seed);
}

MWC::MWC(const uint8_t (&seed)[16384]) {
	srand(seed);
}

void MWC::srand(uint32_t seed) {
	LCG filler(seed);
	srand(filler);
}

void MWC::srand(Generator &filler) {
	for(size_t n = 0; n < 4096; n++){
		Q[n] = filler.next();
	}
	c = 362436;
	i = 4095;
}

void MWC::srand(const uint32_t (&seed)[4096]) {
	for(size_t n = 0; n < 4096; n++){
		Q[n] = seed[n];
	}
	c = 362436;
	i = 4095;
}

void MWC::srand(const uint8_t (&seed)[16384]) {
	for(size_t n = 0; n < 4096; n++){
		Q[n] = ((const uint32_t*)seed)[n];
	}
	c = 362436;
	i = 4095;
}

uint32_t MWC::next() {
	uint64_t t, a = 18782LL;
	uint32_t x, r = 0xFFFFFFFE;
	i = (i + 1) & 4095;
	t = a * Q[i] + c;
	c = (t >> 32);
	x = t + c;
	if(x < c){ x++; c++; }
	return (Q[i] = r - x);
}

uint32_t MWC::max() {
	return 0xFFFFFFFFU;
}

/* WELL */

WELL::WELL() {
	srand(0);
}

WELL::WELL(uint32_t seed) {
	srand(seed);
}

WELL::WELL(Generator &filler) {
	srand(filler);
}

WELL::WELL(const uint32_t (&seed)[32]) {
	srand(seed);
}

WELL::WELL(const uint8_t (&seed)[128]) {
	srand(seed);
}

void WELL::srand(uint32_t seed) {
	LCG filler(seed);
	srand(filler);
}

void WELL::srand(Generator &filler) {
	for(size_t n = 0; n < 32; n++){
		state[n] = filler.next();
	}
	index = 0;
}

void WELL::srand(const uint32_t (&seed)[32]) {
	for(size_t n = 0; n < 32; n++){
		state[n] = seed[n];
	}
	index = 0;
}

void WELL::srand(const uint8_t (&seed)[128]) {
	for(size_t n = 0; n < 32; n++){
		state[n] = ((const uint32_t*)seed)[n];
	}
	index = 0;
}

uint32_t WELL::next() {
	uint32_t z0, z1, z2;
	uint32_t v = (index + 31) & 0x0000001FU;
	z0 = state[v];
	uint32_t vm1 = state[(index + 3) & 0x0000001FU];
	uint32_t vm2 = state[(index + 24) & 0x0000001FU];
	uint32_t vm3 = state[(index + 10) & 0x0000001FU];
	z1 = state[index] ^ (vm1 ^ (vm1 >> 8));
	z2 = (vm2 ^ (vm2 << 19)) ^ (vm3 ^ (vm3 << 14));
	state[index] = z1 ^ z2;
	state[v] = (z0 ^ (z0 << 11)) ^ (z1 ^ (z1 << 7)) ^ (z2 ^ (z2 << 13));
	index = v;
	return state[index];
}

uint32_t WELL::max() {
	return 0xFFFFFFFFU;
}

/* MT */

MT::MT(){
	srand(0);
}

MT::MT(uint32_t seed){
	srand(seed);
}

MT::MT(Generator &filler){
	srand(filler);
}

MT::MT(const uint32_t (&seed)[624]){
	srand(seed);
}

MT::MT(const uint8_t (&seed)[2496]){
	srand(seed);
}

void MT::srand(uint32_t seed){
	LCG filler(seed);
	srand(filler);
}

void MT::srand(Generator &filler){
	for(size_t n = 0; n < 624; n++){
		state[n] = filler.next();
	}
	index = 0;
}

void MT::srand(const uint32_t (&seed)[624]){
	for(size_t n = 0; n < 624; n++){
		state[n] = seed[n];
	}
	index = 0;
}

void MT::srand(const uint8_t (&seed)[2496]){
	for(size_t n = 0; n < 624; n++){
		state[n] = ((const uint32_t*)seed)[n];
	}
	index = 0;
}

uint32_t MT::next(){
	if(index == 0){
		for(size_t n = 0; n < 624; n++){
			uint32_t y = (state[n] & 0x80000000) + (state[(n + 1) % 624] & 0x7FFFFFFF);
			state[n] = state[(n + 397) % 624] ^ (y >> 1);
			if(y % 2 != 0)state[n] ^= 0x9908B0DF;
		}
	}
	uint32_t y = state[index];
	y ^= y >> 11;
	y ^= (y << 7) & 0x9D2C5680;
	y ^= (y << 15) & 0xEFC60000;
	y ^= (y >> 18);
	index = (index + 1) % 624;
	return y;
}

uint32_t MT::max(){
	return 0xFFFFFFFFU;
}

} /* namespace Random */
