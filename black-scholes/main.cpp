
#define _USE_MATH_DEFINES

#include <iostream>
#include <cmath>

double d_1(const double& S, const double& K, const double& r, const double& v, const double&  T)
{
    return (std::log(S / K) + (r + ((v * v) * 0.5)) * T) / (v * std::pow(T, 0.5));
}

double d_2(const double& d1, const double& v, const double&  T)
{
    return d1 - v * std::pow(T, 0.5);
}

double d_j(const int& j, const double& S, const double& K, const double& r, const double& v, const double& T) {
    return (std::log(S/K) + (r + (std::pow(-1,j-1))*0.5*v*v)*T)/(v*(std::pow(T,0.5)));
}

double normalPDF(const double& x)
{
    return (1.0 / std::pow(2*M_PI, 0.5)) * std::exp(-0.5 * x * x);
}

double normalCDF(const double& x)
{
   return 0.5 * std::erfc(-x * M_SQRT1_2);
}

double calculate_put_price(const double& S, const double& K, const double& r, const double& v, const double& T)
{
    const double d1 = d_1(S, K, r, v, T);
    const double d2 = d_2(d1, v, T);
    return (K * std::exp(-r * T)) - S + ((S * normalCDF(d1)) - (K * std::exp(-r * T) * normalCDF(d2)));
}

double calculate_call_price(const double& S, const double& K, const double& r, const double& v, const double& T)
{
    const double d1 = d_1(S, K, r, v, T);
    const double d2 = d_2(d1, v, T);
    return (S * normalCDF(d1)) - (K * std::exp(-r * T) * normalCDF(d2));
}

int main()
{
    double S = 108.0;  // Option price
    double K = 100.0;  // Strike price
    double r = 0.02;   // Risk-free rate (5%)
    double v = 0.2;    // Volatility of the underlying (20%)
    double T = 0.5;    // One year until expiry

    double call_price = calculate_call_price(S, K, r, v, T);
    double put_price = calculate_put_price(S, K, r, v, T);

    std::cout << "Underlying:      " << S << std::endl;
    std::cout << "Strike:          " << K << std::endl;
    std::cout << "Risk-Free Rate:  " << r << std::endl;
    std::cout << "Volatility:      " << v << std::endl;
    std::cout << "Maturity:        " << T << std::endl;

    std::cout << "Call Price:      " << call_price << std::endl;
    std::cout << "Put Price:       " << put_price << std::endl;
}
