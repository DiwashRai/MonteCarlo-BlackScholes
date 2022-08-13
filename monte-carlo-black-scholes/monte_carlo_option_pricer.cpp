
#include "monte_carlo_option_pricer.hpp"
#include "stock_price_generator.hpp"

#include <numeric>
#include <cmath>
#include <future>
#include <iostream>
#include <algorithm>

MonteCarloOptionPricer::MonteCarloOptionPricer(const double& stock_price,
                                               const double& strike_price,
                                               const double& risk_free_rate,
                                               const double& volatility,
                                               const double& time_to_maturity,
                                               int steps,
                                               int simulations,
                                               bool inParallel)
: stock_price_(stock_price)
, strike_price_(strike_price)
, volatility_(volatility)
, risk_free_rate_(risk_free_rate)
, time_to_maturity_(time_to_maturity)
, steps_(steps)
, simulations_(simulations)
, inParallel_(inParallel)
, seeds_(std::vector<int>(simulations))
{
}

void MonteCarloOptionPricer::generate_seeds()
{
    std::srand(unsigned(std::time(nullptr)));
    std::generate(seeds_.begin(), seeds_.end(), std::rand);
}

double MonteCarloOptionPricer::run()
{
    if (inParallel_)
    {
        std::cout << "Running in parallel\n";
        return runSimulationAsync();
    }
    else
    {
        std::cout << "Running sequentially\n";
        return runSimulation();
    }
}

double MonteCarloOptionPricer::runSimulation()
{
    StockPriceGenerator price_generator(stock_price_,
                                        volatility_,
                                        risk_free_rate_,
                                        time_to_maturity_,
                                        steps_);
    generate_seeds();

    std::vector<std::vector<double>> price_movements;

    // run simulations sequentially
    for (const auto& seed : seeds_) {
        price_movements.push_back(price_generator.generate(seed));
    }

    std::vector<double> discounted_payoffs;
    for (const auto& movement : price_movements) {
        const double terminal_price = movement.back();
        const double payoff = std::max(0.0, terminal_price - strike_price_);
        discounted_payoffs.push_back(std::exp((-risk_free_rate_) * time_to_maturity_) * payoff);
    }

    const double total_payoffs = std::accumulate(discounted_payoffs.begin(), discounted_payoffs.end(), 0.0);
    const double call_price = total_payoffs / simulations_;
    return call_price;
}

double MonteCarloOptionPricer::runSimulationAsync()
{
    StockPriceGenerator price_generator(stock_price_,
                                        volatility_,
                                        risk_free_rate_,
                                        time_to_maturity_,
                                        steps_);
    generate_seeds();

    std::vector<std::future<std::vector<double>>> price_movement_futures;
    price_movement_futures.reserve(simulations_);

    auto callback = [&price_generator](int seed){ return price_generator.generate(seed);};

    // run simulations in parallel
    for (const auto& seed : seeds_) {
        price_movement_futures.push_back(std::async(callback, seed));
    }

    std::vector<double> discounted_payoffs;
    discounted_payoffs.reserve(simulations_);

    for (auto& future : price_movement_futures) {
        const double terminal_price = future.get().back();
        const double payoff = std::max(0.0, terminal_price - strike_price_);
        discounted_payoffs.push_back(std::exp((-risk_free_rate_) * time_to_maturity_) * payoff);
    }

    const double total_payoffs = std::accumulate(discounted_payoffs.begin(), discounted_payoffs.end(), 0.0);
    const double call_price = total_payoffs / simulations_;
    return call_price;
}

