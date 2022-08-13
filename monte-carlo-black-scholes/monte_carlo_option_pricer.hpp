#ifndef MONTE_CARLO_OPTION_PRICER
#define MONTE_CARLO_OPTION_PRICER

#include <vector>

class MonteCarloOptionPricer
{
public:
    MonteCarloOptionPricer(const double& stock_price,
                           const double& strike_price,
                           const double& risk_free_rate,
                           const double& volatility,
                           const double& time_to_maturity,
                           int steps,
                           int simulations,
                           bool inParallel);

    double run();

private:
    void generate_seeds();
    double runSimulation();
    double runSimulationAsync();

    const double stock_price_;
    const double strike_price_;
    const double volatility_;
    const double risk_free_rate_;
    const double time_to_maturity_;

    const int steps_;
    const int simulations_;
    const bool inParallel_;
    std::vector<int> seeds_;
};

#endif // MONTE_CARLO_OPTION_PRICER
