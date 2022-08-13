#ifndef STOCK_PRICE_GENERATOR
#define STOCK_PRICE_GENERATOR

#include <vector>

struct params {
    double stock_price;
    double strike_price;
    double volatility;
    double risk_free_rate;
    double time_to_maturity;
};


class StockPriceGenerator {
public:
    StockPriceGenerator(const double& S, const double& vol, const double& r, const double& T, const int& N);

    std::vector<double> generate(int seed) const;

private:
    const double init_stock_price_;
    const double volatility_;
    const double risk_free_rate_;
    const double time_to_expiration_;
    const int time_steps;
    const double dt_;
};

#endif // STOCK_PRICE_GENERATOR
