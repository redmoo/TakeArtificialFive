#ifndef RANDOM_GENERATOR_H
#define RANDOM_GENERATOR_H

#include <boost/random/mersenne_twister.hpp>
#include <boost/random/uniform_int_distribution.hpp>
#include <ctime>
#include <QDebug>

class RandomGenerator
{

public:
    static RandomGenerator* get()
    {
        static RandomGenerator instance; // Guaranteed to be destroyed. Instantiated on first use.
        return &instance;
    }

    int random(int min, int max)
    {
        boost::random::uniform_int_distribution<> range(min, max);
        return range(rng);
    }

    time_t getSeed()
    {
        return seed;
    }

private:
    RandomGenerator() {
        // 1407943248 interesting seed: 4 entitete vsaka ma 2 lonely gena  1407943591 1407943337 1407943324 1407943262 (extra cool 1407946622)
        seed = std::time(0);
        qDebug() << "Seed: " << seed;
        rng.seed(1407946622);
    }

    // Dont forget to declare these two. You want to make sure they
    // are unaccessable otherwise you may accidently get copies of
    // your singleton appearing.
    RandomGenerator(RandomGenerator const&); // Don't Implement
    void operator=(RandomGenerator const&); // Don't implement

private:
    time_t seed;
    boost::random::mt19937 rng; // static?

};

#endif // RANDOM_GENERATOR_H
