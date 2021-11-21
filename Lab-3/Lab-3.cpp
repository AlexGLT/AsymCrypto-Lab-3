#include <iostream>

#include "Rabin.hpp"

int main2()
{
	mpz_class a = mpz_class("FF", 16);

	std::cout << mpz_sizeinbase(a.get_mpz_t(), 256);
}

int main()
{
	auto AlicePair = GenerateKeyPair();

	auto RabinCryptosystem = new Rabin(AlicePair.first, AlicePair.second);

	auto AlicePublicKey = RabinCryptosystem->GetPublicKey();

	printf("\n");

	std::map<std::string, mpz_class> BobPublicKey = {};

	std::string temp;

	std::cout << "Give n for Bob's RabinCryptosystem:" << std::endl;
	std::cin >> temp;
	BobPublicKey["n"] = mpz_class(temp, 16);

	std::cout << "Give b for Bob's RabinCryptosystem:" << std::endl;
	std::cin >> temp;
	BobPublicKey["b"] = mpz_class(temp, 16);

	printf("\n");

	printf("\033[32mAlice RabinCryptosystem:\033[0m\n");
	gmp_printf("\033[96mAlice p: \033[0m\n%#Zx\n", AlicePair.first.get_mpz_t());
	gmp_printf("\033[96mAlice q: \033[0m\n%#Zx\n", AlicePair.second);
	gmp_printf("\033[96mAlice n: \033[0m\n%#Zx\n", AlicePublicKey["n"]);
	gmp_printf("\033[96mAlice b: \033[0m\n%#Zx\n\n", AlicePublicKey["b"]);

	printf("\033[32mBob RabinCryptosystem:\033[0m\n");
	gmp_printf("\033[96mBob n: \033[0m\n%#Zx\n", BobPublicKey["n"]);
	gmp_printf("\033[96mBob b: \033[0m\n%#Zx\n\n", BobPublicKey["b"]);

	auto message = BlumBlumShub(1, 8 * (mpz_sizeinbase(AlicePublicKey["n"].get_mpz_t(), 256) - 10));

	printf("\033[32mMessage:\033[0m\n");
	gmp_printf("%#Zx\n\n", message);
	
	auto formatMessageByBobPublicKey = FormatMessage(message, BobPublicKey["n"]);

	printf("\033[32mFormat message:\033[0m\n");
	gmp_printf("%#Zx\n\n", formatMessageByBobPublicKey);

	printf("\033[32mEncryption and decryption:\033[0m\n");

	auto EncryptedMessageByBobPublicKey = RabinCryptosystem->Encrypt(formatMessageByBobPublicKey, BobPublicKey["n"], BobPublicKey["b"]);
	printf("\033[96mEncrypted message by Bob public key\033[0m\n");
	gmp_printf("  1. y = %#Zx\n", EncryptedMessageByBobPublicKey["y"]);
	gmp_printf("  2. c_1 = %#Zx\n", EncryptedMessageByBobPublicKey["c_1"]);
	gmp_printf("  1. c_2 = %#Zx\n\n", EncryptedMessageByBobPublicKey["c_2"]);

	std::cout << "Give encrypted message by AlicePublicKey:" << std::endl;
	std::cout << "  1. y = ";
	std::cin >> temp;
	auto y = mpz_class(temp, 16);
	std::cout << "  1. c_1 = ";
	std::cin >> temp;
	auto c_1 = mpz_class(temp, 16);
	std::cout << "  1. c_2 = ";
	std::cin >> temp;
	auto c_2 = mpz_class(temp, 16);

	printf("\n");

	auto DecryptedMessageByAlicePublicKey = RabinCryptosystem->Decrypt(y, c_1, c_2);
	printf("\033[96mDecrypted message:\033[0m\n");
	gmp_printf("%#Zx\n\n", DecryptedMessageByAlicePublicKey);

	printf("\033[32mMessage sign:\033[0m\n");

	auto AliceSignedMessage = RabinCryptosystem->Sign(message);
	printf("\033[96mAlice sign:\033[0m\n");
	gmp_printf("%#Zx\n\n", AliceSignedMessage.second);
	
	std::cout << "Give signed message by Bob:" << std::endl;
	std::cin >> temp;
	auto BobSignedMessage = mpz_class(temp, 16);

	auto AliceVerifiedBobSign = RabinCryptosystem->Verify(std::pair(message, BobSignedMessage), BobPublicKey["n"]);

	printf("\033[95mSign verified\033[0m = %s\n\n", AliceVerifiedBobSign ? "\033[32mtrue\033[0m" : "\033[31mfalse\033[0m");

	printf("\033[32mZero-knowledge protocol:\033[0m\n");
	printf("\033[96mGive bob key:\033[0m\n");
	std::cin >> temp;
	auto n = mpz_class(temp, 16);

	printf("\n");

	auto t = getMod(BlumBlumShub(1, mpz_sizeinbase(n.get_mpz_t(), 2)), n);
	gmp_printf("\033[95mt =\033[0m %#Zx\n\n", t);
	gmp_printf("\033[95mt^2 =\033[0m %#Zx\n\n", modularSquare(t, n));

	printf("\033[96mGive Bob's remainder:\033[0m\n");
	std::cin >> temp;
	auto z = mpz_class(temp, 16);

	printf("\n");

	int attemptCount = 1;

	while (z == t || z == mpz_class(n - t))
	{
		gmp_printf("\033[31mFail... \033[0m\n\n");

		t = BlumBlumShub(1, mpz_sizeinbase(n.get_mpz_t(), 2));
		gmp_printf("\033[95mNew t =\033[0m %#Zx\n\n", t);
		gmp_printf("\033[95mNew t^2 =\033[0m %#Zx\n\n", modularSquare(t, n));

		printf("\033[96mGive Bob's new remainder:\033[0m\n");
		std::cin >> temp;
		z = mpz_class(temp, 16);

		printf("\n");
	}

	printf("\033[32mSuccess! Count of attempts = %d!\n\n", attemptCount);

	auto p = bigGCD(t + z, n);
	mpz_class q = n / p;

	gmp_printf("\033[32mp =\033[0m %#Zx\n\n", p);
	gmp_printf("\033[32mq =\033[0m %#Zx\n\n", q);

	gmp_printf("\033[32mp * q =\033[0m %#Zx\n\n", mpz_class(p * q));
}
