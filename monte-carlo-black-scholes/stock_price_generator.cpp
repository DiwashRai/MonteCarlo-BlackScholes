
#include "stock_price_generator.hpp"
#include <cmath>
#include <random>

using std::mt19937_64;

StockPriceGenerator::StockPriceGenerator(const double& S, const double& vol, const double& r, const double& T, const int& N)
    : init_stock_price_{S}
    , volatility_{vol}
    , risk_free_rate_{r}
    , time_to_expiration_{T}
    , time_steps{N}
    , dt_{T / N}
{ }

std::vector<double> StockPriceGenerator::generate(int seed) const
{
    std::vector<double> prices;
    mt19937_64 random(seed);
    std::normal_distribution<> nd;
    auto next_price = [this](double prev_price, double norm)
    {
        double expLeftArg = (risk_free_rate_ - ((volatility_ * volatility_) * 0.5)) * dt_;
        double expRightArg = (volatility_ * norm * std::sqrt(dt_));
        return prev_price * std::exp(expLeftArg + expRightArg);
    };

    prices.push_back(init_stock_price_);

    double stock_price = init_stock_price_;
    for (int i = 1; i <= time_steps; ++i)
    {
        double nrm = nd(random);
        stock_price = next_price(stock_price, nrm);
        prices.push_back(stock_price);
    }

    return prices;
}

