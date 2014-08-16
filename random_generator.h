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

    double random01() // (0,1]
    {
        return (double)random(1, 100) / 100;
    }

    void setSeed(time_t s)
    {
        rng.seed(seed = s);
        qDebug() << "Seed: " << seed;
    }

    time_t getSeed()
    {
        return seed;
    }

private:
    RandomGenerator() {
        // 1407943248 interesting seed: 4 entitete vsaka ma 2 lonely gena  1407943591 1407943337 1407943324 1407943262 (extra cool 1407946622) 1407972922
        // 2.Best: 1407976810
        // BEST!!! 1408048805
        //1407976810 0, 52, 0, 35, 0 i%5 -> da dobis proper sound
        setSeed(std::time(0));
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
