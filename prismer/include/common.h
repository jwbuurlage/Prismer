#include <random>

namespace GRandom
{

int genrand()
{

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(1, 1000000);
    return dis(gen);
}

} // namespace GRandom
