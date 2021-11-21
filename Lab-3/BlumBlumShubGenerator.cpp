#include "BlumBlumShubGenerator.hpp"

#include <iostream>

mpz_class BlumBlumShub(unsigned char vers, const unsigned int& size)
{
	auto sequence = new randomSequence(size - (size % 8));

	auto p = mpz_class("D5BBB96D30086EC484EBA3D7F9CAEB07", 16);
	auto q = mpz_class("425D2B9BFDB25B9CF6C416CC6E37B59C1F", 16);

	mpz_class n;
	mpz_mul(n.get_mpz_t(), q.get_mpz_t(), p.get_mpz_t());

	mpz_class state = randomBigIntegerGenerator(64);

	if (vers == 1)
	{
		unsigned int iteration = 0;
		unsigned char byte = 0;

		for (unsigned int i = 0; i < sequence->size; i++)
		{
			byte |= ((state.get_ui() & vers) << (i % 8));

			if ((i + 1) % 8 == 0)
			{
				sequence->array[iteration++] = byte;

				byte = 0;
			}


			state = modularSquare(state, p);
		}
	}
	else
	{
		for (unsigned int i = 0; i < sequence->size / 8; i++)
		{
			sequence->array[i] = state.get_ui() & vers;

			state = modularSquare(state, p);
		}
	}

	auto convertedToMpz = mpz_class(convertToString(sequence->array, sequence->size / 8), 16);

	delete sequence;

	return convertedToMpz;
}

mpz_class LimitedNumberGenerator(const mpz_class& limit)
{
	auto sizeInBits = mpz_sizeinbase(limit.get_mpz_t(), 2);

	mpz_class generatedNumber = BlumBlumShub(1, sizeInBits - (sizeInBits % 8));

	while (generatedNumber % limit == 0)
	{
		generatedNumber = BlumBlumShub(1, sizeInBits - (sizeInBits % 8));
	}

	return generatedNumber % limit;
}