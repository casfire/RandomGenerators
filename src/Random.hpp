/**************************
 * Author: Gasper Medved  *
 * Date:   2014-08-24     *
 **************************/
 
#pragma once
#ifndef _RANDOM_HPP_
#define _RANDOM_HPP_

#include <cstdint>
#include <cstddef>

namespace Random {
	
	class Generator {
	public:
		
		void seed(Generator &source);
		void seed(std::uint32_t source);
		void seed(const std::uint32_t *source, std::size_t size);
		void seed(const std::uint8_t *source, std::size_t size);
		
		virtual void setState(const std::uint32_t *state) = 0;
		virtual void getState(std::uint32_t *state) const = 0;
		virtual std::size_t stateSize() const = 0;
		virtual std::uint32_t max() const = 0;
		virtual std::uint32_t next() = 0;
		virtual std::uint32_t current() const = 0;
		
		/* These don't work for STDLIB */
		double fnext(double min, double max);
		std::uint32_t inext(std::uint32_t min, std::uint32_t max);
		double fcurrent(double min, double max) const;
		std::uint32_t icurrent(std::uint32_t min, std::uint32_t max) const;
		
	};
	
	/* stdlib rand(), msvcrt version */
	class STDLIB : public Generator {
	public:
		STDLIB();
		STDLIB(unsigned int seed);
		STDLIB(const STDLIB &obj);
		
		void setState(const std::uint32_t *state);
		void getState(std::uint32_t *state) const;
		std::size_t stateSize() const;
		std::uint32_t max() const;
		std::uint32_t next();
		std::uint32_t current() const;
	private:
		unsigned int state_seed;
	};
	
	/* Linear congruential generator */
	class LCG : public Generator {
	public:
		LCG();
		LCG(uint32_t seed);
		LCG(const LCG &obj);
		
		void setState(const std::uint32_t *state);
		void getState(std::uint32_t *state) const;
		std::size_t stateSize() const;
		std::uint32_t max() const;
		std::uint32_t next();
		std::uint32_t current() const;
	private:
		uint32_t state_seed;
	};
	
	/* Multiply-with-carry (CMWC) */
	class MWC : public Generator {
	public:
		MWC();
		MWC(uint32_t seed);
		MWC(const MWC &obj);
		
		void setState(const std::uint32_t *state);
		void getState(std::uint32_t *state) const;
		std::size_t stateSize() const;
		std::uint32_t max() const;
		std::uint32_t next();
		std::uint32_t current() const;
	private:
		uint32_t state_Q[4096], state_c, state_i;
	};
	
	/* Well equidistributed long-period linear (WELL1024) */
	class WELL : public Generator {
	public:
		WELL();
		WELL(uint32_t seed);
		WELL(const WELL &obj);
		
		void setState(const std::uint32_t *state);
		void getState(std::uint32_t *state) const;
		std::size_t stateSize() const;
		std::uint32_t max() const;
		std::uint32_t next();
		std::uint32_t current() const;
	private:
		uint32_t state_seed[32], state_index;
	};
	
	/* Mersenne twister (MT19937) */
	class MT : public Generator {
	public:
		MT();
		MT(uint32_t seed);
		MT(const MT &obj);
		
		void setState(const std::uint32_t *state);
		void getState(std::uint32_t *state) const;
		std::size_t stateSize() const;
		std::uint32_t max() const;
		std::uint32_t next();
		std::uint32_t current() const;
	private:
		uint32_t state_seed[624], state_index;
	};
	
}

#endif