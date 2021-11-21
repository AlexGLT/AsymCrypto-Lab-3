#include "Rabin.hpp"

#include <iostream>

mpz_class generateGreatPrimeNumber(const unsigned int& bitSize)
{
	mpz_class prime = BlumBlumShub(1, bitSize);

	while (!MillerRabinTest(prime, 100) || (prime % 4 != 3))
	{
		gmp_printf("\033[31mDamn... \033[0m%#Zx\r\n", prime);

		prime = BlumBlumShub(1, bitSize);
	}

	gmp_printf("\033[32mSuccess! \033[0m%#Zx\r\n", prime);	

	return prime;
}

std::pair<mpz_class, mpz_class> GenerateKeyPair(int bitSize)
{
	return std::pair(generateGreatPrimeNumber(bitSize), generateGreatPrimeNumber(bitSize));
}

mpz_class FormatMessage(mpz_class message, mpz_class modulus)
{
	return (mpz_class("ff", 16) << 8 * (mpz_sizeinbase(modulus.get_mpz_t(), 256) - 2)) + (message << 64) + BlumBlumShub(1, 64);
}

mpz_class UnformatMessage(mpz_class message, mpz_class modulus)
{
	return (message ^ (mpz_class("ff", 16) << 8 * (mpz_sizeinbase(modulus.get_mpz_t(), 256) - 2))) >> 64;
}

std::vector<mpz_class> GetSquareRoots(const mpz_class& y, const mpz_class& p, const mpz_class& q, const mpz_class& n)
{
	std::vector<mpz_class> roots;

	auto s_1 = modularPow(y, (p + 1) / 4, p);
	auto s_2 = modularPow(y, (q + 1) / 4, q);

	auto bezuCoefficients = ExtendedEuclid(p, q, n);

	for (char firstSign = 1; firstSign > -2; firstSign -= 2)
	{
		for (char secondSign = 1; secondSign > -2; secondSign -= 2)
		{
			roots.push_back((getMod(firstSign * bezuCoefficients["u"] * p * s_2, n) + getMod(secondSign * bezuCoefficients["v"] * q * s_1, n)) % n);
		}
	}

	return roots;
}

Rabin::Rabin(const mpz_class& p, const mpz_class& q)
{
	this->p = p;
	this->q = q;
	this->n = p * q;
	this->b = BlumBlumShub(1, mpz_sizeinbase(this->n.get_mpz_t(), 2) - 16);
}

std::map<std::string, mpz_class> Rabin::GetPublicKey()
{
	return { { "n", this->n }, { "b", this->b } };
}

std::map<std::string, mpz_class> Rabin::Encrypt(const mpz_class x, const mpz_class n, const mpz_class b)
{
	mpz_class y = (x * (x + b)) % n;
	mpz_class temp = (x + (b * modularInvert(mpz_class(2), n) % n)) % n;
	
	mpz_class c_1 = temp.get_ui() & 1;
	mpz_class c_2 = mpz_jacobi(temp.get_mpz_t(), n.get_mpz_t());

	return { {"y", y}, {"c_1", c_1}, {"c_2", c_2} };
}

mpz_class Rabin::Decrypt(mpz_class y, mpz_class c_1, mpz_class c_2)
{
	mpz_class modifiedY = (y + modularSquare(this->b, this->n) * modularInvert(mpz_class(4), this->n) % this->n) % this->n;

	auto roots = GetSquareRoots(modifiedY, this->p, this->q, this->n);

	auto s_1 = modularPow(modifiedY, (this->p + 1) / 4, this->p);
	auto s_2 = modularPow(modifiedY, (this->q + 1) / 4, this->q);

	auto bezuCoefficients = ExtendedEuclid(this->p, this->q, this->n);

	for (int i = 0; i < roots.size(); i++)
	{
		if ((roots[i] % 2 == c_1) && (mpz_jacobi(roots[i].get_mpz_t(), this->n.get_mpz_t()) == c_2))
		{
			auto trueRoot = getMod(roots[i] - b * modularInvert(mpz_class(2), this->n), this->n);

			return UnformatMessage(trueRoot, this->n);
		}
	}
}

std::pair<mpz_class, mpz_class> Rabin::Sign(mpz_class message)
{
	mpz_class x = FormatMessage(message, this->n);

	while ((mpz_jacobi(x.get_mpz_t(), this->p.get_mpz_t()) != 1) || (mpz_jacobi(x.get_mpz_t(), this->q.get_mpz_t()) != 1))
	{
		x = FormatMessage(message, this->n);
	}

	auto roots = GetSquareRoots(x, this->p, this->q, this->n);
	int rd = randomNumberGenerator(2);

	return std::pair(message, roots[rd]);
}

bool Rabin::Verify(const std::pair<mpz_class, mpz_class>& message, const mpz_class& n)
{
	return (message.first == UnformatMessage(modularSquare(message.second, n), n));
}
