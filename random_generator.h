#ifndef RANDOM_GENERATOR_H
#define RANDOM_GENERATOR_H

#include <boost/random/mersenne_twister.hpp>
#include <boost/random/uniform_int_distribution.hpp>
#include <ctime>

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

private:
    RandomGenerator() {
        rng.seed(std::time(0));
    }

    // Dont forget to declare these two. You want to make sure they
    // are unaccessable otherwise you may accidently get copies of
    // your singleton appearing.
    RandomGenerator(RandomGenerator const&); // Don't Implement
    void operator=(RandomGenerator const&); // Don't implement

private:
    boost::random::mt19937 rng; // static?

/* TEST
public:
    static void seed(int s)
    {
        rng.seed(s);
    }

    static int random(int min, int max)
    {
        boost::random::uniform_int_distribution<> range(min, max);
        return range(rng);
    }

private:
    static boost::random::mt19937 rng;
*/
};

#endif // RANDOM_GENERATOR_H
