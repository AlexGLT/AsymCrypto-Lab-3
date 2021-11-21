#pragma once

#include "Utils.hpp"

mpz_class BlumBlumShub(unsigned char vers, const unsigned int& size = (1 << 21));

mpz_class LimitedNumberGenerator(const mpz_class& limit);