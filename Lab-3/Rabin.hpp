#pragma once

#include <map>

#include "MillerRabinPrimality.hpp"

std::pair<mpz_class, mpz_class> GenerateKeyPair(int bitSize = 512);
mpz_class FormatMessage(mpz_class message, mpz_class modulus);

class Rabin
{
private:
	mpz_class p;
	mpz_class q;
public:
	mpz_class n;
	mpz_class b;

	Rabin(const mpz_class& p, const mpz_class& q);
	std::map<std::string, mpz_class> GetPublicKey();
	std::map<std::string, mpz_class> Encrypt(const mpz_class x, const mpz_class n, const mpz_class b);
	mpz_class Decrypt(mpz_class y, mpz_class c_1, mpz_class c_2);
	std::pair<mpz_class, mpz_class> Sign(mpz_class message);
	bool Verify(const std::pair<mpz_class, mpz_class>& message, const mpz_class& n);
};
