#include "Random.hpp"

/* Generator */

void Random::Generator::seed(Generator &source) {
	std::size_t sSize = this->stateSize();
	std::uint32_t state[sSize];
	while (sSize-- > 0) state[sSize] = source.next();
	this->setState(reinterpret_cast<const std::uint32_t*>(&state));
}

void Random::Generator::seed(std::uint32_t source) {
	if (this->stateSize() <= 1) {
		this->setState(reinterpret_cast<const std::uint32_t*>(&source));
	} else {
		Random::LCG filler = Random::LCG(source);
		this->seed(filler);
	}
}

void Random::Generator::seed(const std::uint32_t *source, std::size_t size) {
	std::size_t sSize = this->stateSize();
	std::uint32_t state[sSize];
	while (sSize-- > 0) state[sSize] = 0;
	while (size-- > 0) {
		Random::LCG filler = Random::LCG(source[size]);
		sSize = this->stateSize();
		while (sSize-- > 0) state[sSize] ^= filler.next();
	}
	this->setState(reinterpret_cast<const std::uint32_t*>(&state));
}

void Random::Generator::seed(const std::uint8_t *source, std::size_t size) {
	std::size_t sSize = this->stateSize();
	std::uint32_t state[sSize];
	while (sSize-- > 0) state[sSize] = 0;
	for (std::size_t i = 0; i < size; i += 4) {
		uint32_t fillerSeed = source[i];
		if (i + 1 < size) fillerSeed |= source[i + 1] << 8;
		if (i + 2 < size) fillerSeed |= source[i + 2] << 16;
		if (i + 3 < size) fillerSeed |= source[i + 3] << 24;
		Random::LCG filler = Random::LCG(fillerSeed);
		sSize = this->stateSize();
		while (sSize-- > 0) state[sSize] ^= filler.next();
	}
}


double Random::Generator::fnext(double min, double max) {
	return min + (max - min) * (static_cast<double>(next()) / 4294967295.0);
}

std::uint32_t Random::Generator::inext(std::uint32_t min, std::uint32_t max) {
	return min + next() % (max - min);
}

double Random::Generator::fcurrent(double min, double max) const {
	return min + (max - min) * (static_cast<double>(current()) / 4294967295.0);
}

std::uint32_t Random::Generator::icurrent(std::uint32_t min, std::uint32_t max) const {
	return min + current() % (max - min);
}

/* STDLIB */

Random::STDLIB::STDLIB() : state_seed(1) {}
Random::STDLIB::STDLIB(unsigned int seed) : state_seed(seed) {}
Random::STDLIB::STDLIB(const STDLIB &obj) : state_seed(obj.state_seed) {}

void Random::STDLIB::setState(const std::uint32_t *state) {
	state_seed = *state;
}

void Random::STDLIB::getState(std::uint32_t *state) const {
	*state = state_seed;
}

std::uint32_t Random::STDLIB::max() const {
	return 0x7FFF;
}

std::size_t Random::STDLIB::stateSize() const {
	return 1;
}

std::uint32_t Random::STDLIB::next() {
	state_seed = state_seed * 0x343FD + 0x269EC3;
	return static_cast<std::uint32_t>((state_seed >> 0x10) & 0x7FFF);
}

std::uint32_t Random::STDLIB::current() const {
	return static_cast<std::uint32_t>(((state_seed * 0x343FD + 0x269EC3) >> 0x10) & 0x7FFF);
}

/* LCG */

Random::LCG::LCG() : state_seed(1) {}
Random::LCG::LCG(uint32_t seed) : state_seed(seed) {}
Random::LCG::LCG(const LCG &obj) : state_seed(obj.state_seed) {}

void Random::LCG::setState(const std::uint32_t *state) {
	state_seed = *state;
}

void Random::LCG::getState(std::uint32_t *state) const {
	*state = state_seed;
}

std::size_t Random::LCG::stateSize() const {
	return 1;
}

std::uint32_t Random::LCG::max() const {
	return 0xFFFFFFFFU;
}

std::uint32_t Random::LCG::next() {
	return (state_seed = 1664525 * state_seed + 1013904223);
}

std::uint32_t Random::LCG::current() const {
	return 1664525 * state_seed + 1013904223;
}

/* MWC */

Random::MWC::MWC() {
	Random::LCG filler = Random::LCG(1);
	this->seed(filler);
	//state_c = 362436;
	//state_i = 4095;
}

Random::MWC::MWC(uint32_t seed) {
	Random::LCG filler = Random::LCG(seed);
	this->seed(filler);
	//state_c = 362436;
	//state_i = 4095;
}

Random::MWC::MWC(const MWC &obj) : state_c(obj.state_c), state_i(obj.state_i) {
	for (std::size_t i = 0; i < 4096; i++) state_Q[i] = obj.state_Q[i];
}

void Random::MWC::setState(const std::uint32_t *state) {
	for (std::size_t i = 0; i < 4096; i++) state_Q[i] = state[i];
	state_c = state[4097];
	state_i = state[4096];
}

void Random::MWC::getState(std::uint32_t *state) const {
	for (std::size_t i = 0; i < 4096; i++) state[i] = state_Q[i];
	state[4097] = state_c;
	state[4096] = state_i;
}

std::size_t Random::MWC::stateSize() const {
	return 4098;
}

std::uint32_t Random::MWC::max() const {
	return 0xFFFFFFFFU;
}

std::uint32_t Random::MWC::next() {
	state_i = (state_i + 1) & 4095;
	uint64_t t = 18782LL * state_Q[state_i] + state_c;
	state_c = (t >> 32);
	uint32_t x = t + state_c;
	if (x < state_c) {
		x++;
		state_c++;
	}
	return (state_Q[state_i] = 0xFFFFFFFE - x);
}

std::uint32_t Random::MWC::current() const {
	uint32_t i = (state_i + 1) & 4095;
	uint64_t t = 18782LL * state_Q[i] + state_c;
	uint32_t c = (t >> 32);
	uint32_t x = t + c;
	if(x < c) x++;
	return 0xFFFFFFFE - x;
}

/* WELL */

Random::WELL::WELL() {
	Random::LCG filler = Random::LCG(1);
	this->seed(filler);
	//state_index = 0;
}

Random::WELL::WELL(uint32_t seed) {
	Random::LCG filler = Random::LCG(seed);
	this->seed(filler);
	//state_index = 0;
}

Random::WELL::WELL(const WELL &obj) : state_index(obj.state_index) {
	for (std::size_t i = 0; i < 32; i++) state_seed[i] = obj.state_seed[i];
}

void Random::WELL::setState(const std::uint32_t *state) {
	for (std::size_t i = 0; i < 32; i++) state_seed[i] = state[i];
	state_index = state[32] % 32;
}

void Random::WELL::getState(std::uint32_t *state) const {
	for (std::size_t i = 0; i < 32; i++) state[i] = state_seed[i];
	state[32] = state_index;
}

std::size_t Random::WELL::stateSize() const {
	return 33;
}

std::uint32_t Random::WELL::max() const {
	return 0xFFFFFFFFU;
}

std::uint32_t Random::WELL::next() {
	uint32_t z0, z1, z2;
	uint32_t v = (state_index + 31) & 0x0000001FU;
	z0 = state_seed[v];
	uint32_t vm1 = state_seed[(state_index + 3) & 0x0000001FU];
	uint32_t vm2 = state_seed[(state_index + 24) & 0x0000001FU];
	uint32_t vm3 = state_seed[(state_index + 10) & 0x0000001FU];
	z1 = state_seed[state_index] ^ (vm1 ^ (vm1 >> 8));
	z2 = (vm2 ^ (vm2 << 19)) ^ (vm3 ^ (vm3 << 14));
	state_seed[state_index] = z1 ^ z2;
	state_seed[v] = (z0 ^ (z0 << 11)) ^ (z1 ^ (z1 << 7)) ^ (z2 ^ (z2 << 13));
	state_index = v;
	return state_seed[state_index];
}

std::uint32_t Random::WELL::current() const {
	uint32_t z0, z1, z2;
	uint32_t v = (state_index + 31) & 0x0000001FU;
	z0 = state_seed[v];
	uint32_t vm1 = state_seed[(state_index + 3) & 0x0000001FU];
	uint32_t vm2 = state_seed[(state_index + 24) & 0x0000001FU];
	uint32_t vm3 = state_seed[(state_index + 10) & 0x0000001FU];
	z1 = state_seed[state_index] ^ (vm1 ^ (vm1 >> 8));
	z2 = (vm2 ^ (vm2 << 19)) ^ (vm3 ^ (vm3 << 14));
	return (z0 ^ (z0 << 11)) ^ (z1 ^ (z1 << 7)) ^ (z2 ^ (z2 << 13));
}

/* MT */

Random::MT::MT() {
	Random::LCG filler = Random::LCG(1);
	this->seed(filler);
	//state_index = 0;
}

Random::MT::MT(uint32_t seed) {
	Random::LCG filler = Random::LCG(seed);
	this->seed(filler);
	//state_index = 0;
}

Random::MT::MT(const MT &obj) : state_index(obj.state_index) {
	for (std::size_t i = 0; i < 624; i++) state_seed[i] = obj.state_seed[i];
}

void Random::MT::setState(const std::uint32_t *state) {
	for (std::size_t i = 0; i < 624; i++) state_seed[i] = state[i];
	state_index = state[624] % 624;
}

void Random::MT::getState(std::uint32_t *state) const {
	for (std::size_t i = 0; i < 624; i++) state[i] = state_seed[i];
	state[624] = state_index;
}

std::size_t Random::MT::stateSize() const {
	return 625;
}

std::uint32_t Random::MT::max() const {
	return 0xFFFFFFFFU;
}

std::uint32_t Random::MT::next() {
	if (state_index == 0) {
		for (std::size_t n = 0; n < 624; n++) {
			uint32_t y = (state_seed[n] & 0x80000000) + (state_seed[(n + 1) % 624] & 0x7FFFFFFF);
			state_seed[n] = state_seed[(n + 397) % 624] ^ (y >> 1);
			if (y % 2 != 0) state_seed[n] ^= 0x9908B0DF;
		}
	}
	uint32_t y = state_seed[state_index];
	y ^= y >> 11;
	y ^= (y << 7) & 0x9D2C5680;
	y ^= (y << 15) & 0xEFC60000;
	y ^= (y >> 18);
	state_index = (state_index + 1) % 624;
	return y;
}

std::uint32_t Random::MT::current() const {
	uint32_t y;
	if (state_index == 0) {
		uint32_t x = (state_seed[0] & 0x80000000) + (state_seed[1] & 0x7FFFFFFF);
		y = state_seed[397] ^ (x >> 1);
		if (x % 2 != 0) y ^= 0x9908B0DF;
	} else y = state_seed[state_index];
	y ^= y >> 11;
	y ^= (y << 7) & 0x9D2C5680;
	y ^= (y << 15) & 0xEFC60000;
	y ^= (y >> 18);
	return y;
}
