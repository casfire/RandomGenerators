/* RandomGenerators v1.0.0 Date: 2013-08-24 */

#pragma once
#ifndef _RANDOMGENERATORS_HPP_
#define _RANDOMGENERATORS_HPP_
#define _RANDOMGENERATORS_VER_ 100

#include <stdint.h>
#include <stddef.h>

namespace Random {

	class Generator {
	public:
		virtual void srand(uint32_t seed) = 0;
		virtual void srand(Generator &filler) = 0;
		virtual uint32_t next() = 0;
		virtual uint32_t max() = 0;
		float frand(float min = 0.f, float max = 1.f);
		double drand(double min = 0., double max = 1.);
		void fill(size_t size, uint8_t *data);
		void fill(size_t size, uint32_t *data);
	};

	/* STDLIB's rand() (msvcrt's version) */
	class STDLIB : public Generator {
		unsigned int seed;
	public:
		STDLIB();
		STDLIB(uint32_t seed);
		STDLIB(Generator &filler);
		void srand(uint32_t seed);
		void srand(Generator &filler);
		uint32_t next();
		uint32_t max();
	};

	/* Linear congruential generator */
	class LCG : public Generator {
		uint32_t seed;
	public:
		LCG();
		LCG(uint32_t seed);
		LCG(Generator &filler);
		void srand(uint32_t seed);
		void srand(Generator &filler);
		uint32_t next();
		uint32_t max();
	};

	/* Multiply-with-carry (CMWC) */
	class MWC : public Generator {
		uint32_t Q[4096], c, i;
	public:
		MWC();
		MWC(uint32_t seed);
		MWC(Generator &filler);
		MWC(const uint32_t (&seed)[4096]);
		MWC(const uint8_t (&seed)[16384]);
		void srand(uint32_t seed);
		void srand(Generator &filler);
		void srand(const uint32_t (&seed)[4096]);
		void srand(const uint8_t (&seed)[16384]);
		uint32_t next();
		uint32_t max();
	};

	/* Well equidistributed long-period linear (WELL1024) */
	class WELL : public Generator {
		uint32_t state[32], index;
	public:
		WELL();
		WELL(uint32_t seed);
		WELL(Generator &filler);
		WELL(const uint32_t (&seed)[32]);
		WELL(const uint8_t (&seed)[128]);
		void srand(uint32_t seed);
		void srand(Generator &filler);
		void srand(const uint32_t (&seed)[32]);
		void srand(const uint8_t (&seed)[128]);
		uint32_t next();
		uint32_t max();
	};

	/* Mersenne twister (MT19937) */
	class MT : public Generator {
		uint32_t state[624], index;
	public:
		MT();
		MT(uint32_t seed);
		MT(Generator &filler);
		MT(const uint32_t (&seed)[624]);
		MT(const uint8_t (&seed)[2496]);
		void srand(uint32_t seed);
		void srand(Generator &filler);
		void srand(const uint32_t (&seed)[624]);
		void srand(const uint8_t (&seed)[2496]);
		uint32_t next();
		uint32_t max();
	};

} /* namespace Random */

#endif
