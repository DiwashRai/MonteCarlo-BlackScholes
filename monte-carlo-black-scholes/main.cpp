
#include <string>
#include <iostream>
#include "CLI11.hpp"
#include "monte_carlo_option_pricer.hpp"

int main(int argc, char** argv)
{
    CLI::App app{"Monte-Carlo Option Pricer"};
    std::string black_scholes_group = "Black-scholes";
    std::string simulation_grop = "Simulation";

    double stock_price = 0.0;
    double strike_price = 0.0;
    double risk_free_rate = 0.0;
    double volatility = 0.0;
    int days_to_expire = 0;
    app.add_option("-S,--stockprice", stock_price, "Stock Price")->required()->group(black_scholes_group);
    app.add_option("-K,--strikeprice",strike_price, "Volatility")->required()->group(black_scholes_group);
    app.add_option("-r,--riskfreerate", risk_free_rate, "Risk free rate")->required()->group(black_scholes_group);
    app.add_option("-v,--volatility", volatility, "Volatility")->required()->group(black_scholes_group);
    app.add_option("-d,--days", days_to_expire, "Days to expiration")->required()->group(black_scholes_group);

    int steps = 0;
    int simulations = 0;
    bool inParallel = false;
    app.add_option("-N,--steps", steps, "Time steps for each simulations")->required()->group(simulation_grop);
    app.add_option("-M,--simulations", simulations, "Number of simulations")->required()->group(simulation_grop);
    app.add_flag("-p,--parallel", inParallel, "Parallelise the simulation")->group(simulation_grop);

    CLI11_PARSE(app, argc, argv);
    std::cout << "Stock Price: " << stock_price << " | Strike Price: " << strike_price << '\n'
              << "Risk Free Rate : " << risk_free_rate << " | Volatility: " << volatility << '\n'
              << "Days: " << days_to_expire << '\n'
              << "Steps: " << steps << " | Simulations: " << simulations
              << std::endl;

    const double time_to_maturity = days_to_expire / 365.0;

    MonteCarloOptionPricer option_pricer(stock_price, strike_price, risk_free_rate, volatility, time_to_maturity, steps, simulations, inParallel);
    double call_price = option_pricer.run();
    std::cout << "Call price: " << call_price << '\n';
}
