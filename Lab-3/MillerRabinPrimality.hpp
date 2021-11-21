#pragma once

#include "BlumBlumShubGenerator.hpp"

static std::array primes = { 2, 3, 5, 7, 11, 13, 17, 19 };

bool TrialDivision();
bool MillerRabinTest(mpz_class modulo, unsigned int iterations);