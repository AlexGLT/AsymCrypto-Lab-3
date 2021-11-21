#include "MillerRabinPrimality.hpp"

bool trialDivision(mpz_class modulo)
{
	for (int i = 0; i < primes.size(); i++)
	{
		if (modulo % primes[i] == 0) return false;
	}

	return true;
}

bool MillerRabinTest(mpz_class modulo, unsigned int iterations)
{
	if (!trialDivision(modulo)) return false;

	mpz_class decomposed = modulo - 1;

	unsigned int powerOfTwo = 0;

	while (!(decomposed.get_ui() & 1))
	{
		++powerOfTwo;
		decomposed >>= 1;
	}
	
	for (unsigned int i = 0; i < iterations; ++i)
	{
		const auto generatedNumber = LimitedNumberGenerator(modulo);

		if (bigGCD(generatedNumber, modulo) > 1) return false;

		auto x_d = modularPow(generatedNumber, decomposed, modulo);

		if (x_d == 1 || x_d == (modulo - 1)) continue;

		for (unsigned int r = 1; r < powerOfTwo; ++r)
		{
			x_d = modularSquare(x_d, modulo);

			if (x_d == (modulo - 1)) break;
			if (x_d == 1) return false;
		}

		return false;
	}
	
	return true;
}