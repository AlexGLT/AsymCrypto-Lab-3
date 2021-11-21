#include "Utils.hpp"

#include <iostream>

randomSequence::randomSequence(const unsigned int& size)
{
    this->size = size;

    this->array = new unsigned char[this->size / 8];
}

randomSequence::~randomSequence()
{
    delete[] this->array;
}

char Number2Char(int number)
{
	if (number < 10) return static_cast<char>(number + 48);

	return static_cast<char>(number + 55);
}

unsigned int randomNumberGenerator(const unsigned int& power)
{
    randomizationEngine.seed(rd());

    switch (power)
    {
	case 1: return bitRandom(randomizationEngine);
	case 2: return bitRandom(randomizationEngine);
	case 4:	return hexRandom(randomizationEngine);

	default: return NULL;
    }
}

mpz_class randomBigIntegerGenerator(unsigned int size)
{
	std::string bigInteger(size, '0');

	for (int i = 0; i < bigInteger.length(); ++i)
	{
		bigInteger[i] = Number2Char(randomNumberGenerator(4));
	}

	return mpz_class(bigInteger, 16);
}

std::string convertToString(const unsigned char* numberArray, unsigned int numberByteSize)
{
	std::string numberString = "";

	for (unsigned int i = 0; i < numberByteSize; ++i) {
		auto byte = numberArray[i];

		for (unsigned int j = 0; j < 2; j++)
		{
			const auto hex = Number2Char((byte >> (1 - j) * 4) & 15);

			numberString += hex;
		}
	}

	return numberString;
}

mpz_class bigGCD(const mpz_class& firstNumber, const mpz_class& secondNumber)
{
	mpz_class gcd;

	mpz_gcd(gcd.get_mpz_t(), firstNumber.get_mpz_t(), secondNumber.get_mpz_t());

    return gcd;
}

mpz_class getMod(const mpz_class& number, const mpz_class& modulo)
{
	mpz_class result;

	mpz_mod(result.get_mpz_t(), number.get_mpz_t(), modulo.get_mpz_t());

	return result;
}

mpz_class modularInvert(const mpz_class& number, const mpz_class& modulo)
{
	mpz_class result;

	mpz_invert(result.get_mpz_t(), number.get_mpz_t(), modulo.get_mpz_t());

	return result;
}

mpz_class modularSquare(const mpz_class& number, const mpz_class& modulo)
{
    return (number * number) % modulo;
}

mpz_class modularPow(const mpz_class& base, const mpz_class& power, const mpz_class& modulo)
{
    mpz_class result;

    mpz_powm(result.get_mpz_t(), base.get_mpz_t(), power.get_mpz_t(), modulo.get_mpz_t());

    return result;
}

std::map<std::string, mpz_class> ExtendedEuclid(const mpz_class& p, const mpz_class& q, const mpz_class& n)
{
	mpz_class u, v, gcd;

	mpz_gcdext(gcd.get_mpz_t(), u.get_mpz_t(), v.get_mpz_t(), p.get_mpz_t(), q.get_mpz_t());

	return { { "u", getMod(u, n) }, { "v", getMod(v, n) }, { "gcd", gcd } };
}
