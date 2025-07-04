#pragma once
#include <random>
class Hash {
public:
	static int cantor(int a, int b) {
		return (a + b + 1) * (a + b) / 2 + b;
	}

	static int getRandomInt(int a, int b, int seed1, int seed2) {
		// Create a combined seed from the two integers
		std::seed_seq seed{ seed1, seed2 };
		std::mt19937 rng(seed); // Mersenne Twister seeded with combined values

		// Ensure a <= b
		if (a > b) std::swap(a, b);

		std::uniform_int_distribution<int> dist(a, b);
		return dist(rng);
	}

	static void getRandomColor(int seed1, int seed2,int * r, int * g, int * b) {
		// Create a combined seed from the two integers
		std::seed_seq seed{ seed1, seed2 };
		std::mt19937 rng(seed); // Mersenne Twister seeded with combined values

		std::uniform_int_distribution<int> dist(0, 255);

		*r = dist(rng);
		*g = dist(rng);
		*b = dist(rng);
	}
};