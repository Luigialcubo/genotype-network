#ifndef SIS_DYN_HPP_INCLUDED
#define SIS_DYN_HPP_INCLUDED

#include <boost/multi_array.hpp>
//elements of BOOST library (for random number generator)
  #include <boost/random/mersenne_twister.hpp>
  #include <boost/random/uniform_real.hpp>
  #include <boost/random/variate_generator.hpp>

/**
 * @file    sis_dyn.hpp
 * @brief  ODE system for SIRS dynamics with underlying genotype network
 *
 * @author  LHD
 * @since   2018-01-08
 */

struct Sparam {
    const int N;
    const double mu; //mutation probability
    const double b; //transmission rate
    const double g; //recovery
    const double ad; //transcending imm
    const double a; //waning imm
    const std::vector< std::vector<int> > net;
    const std::vector< std::vector<int> > distance;
    const int dim;
}; // parameter structure

using namespace boost;

//********** function dydt definition **************************************************************
int dydt(double t, const double y[], double f[], void * param) {
// ODE system for mutating SIR dynamics with fixed genotype network

    // Cast parameters
    Sparam& p = *static_cast<Sparam* >(param);

    // Create multi_array reference to y and f
    typedef boost::multi_array_ref<const double,2> CSTmatref_type;
    typedef boost::multi_array_ref<double,2> matref_type;
    typedef CSTmatref_type::index indexref;
    CSTmatref_type yref(y,boost::extents[p.N+1][p.dim]);
    matref_type fref(f,boost::extents[p.N+1][p.dim]);

    std::vector<double> infection(1+p.N,0.0); //nodes infected by strain k
    std::vector<double> infection2(1+p.N,0.0); //R_k nodes that get infected by any strains

    for(int k=1; k<=p.N; ++k) {
        infection[k] += p.b*yref[0][0]*yref[k][0];
        for(int kp=1; kp<=p.N; ++kp) {
            infection[k] += p.b*yref[kp][1]*yref[k][0]*(1.0-exp(-p.ad*p.distance[k-1][kp-1]));
            infection2[kp] += p.b*yref[kp][1]*yref[k][0]*(1.0-exp(-p.ad*p.distance[k-1][kp-1]));
        }
    }
    
    fref[0][0] = 0.0;

    for(int k=1; k<=p.N; ++k) {
    
        fref[0][0] += p.a*yref[k][1] - p.b*yref[0][0]*yref[k][0];

        fref[k][0] = infection[k]*(1.0-p.mu*p.net[k-1].size()) - p.g*yref[k][0];
        
        for(int kp=0; kp<p.net[k-1].size(); ++kp) fref[k][0] += infection[p.net[k-1][kp]+1]*p.mu;///(1.0*p.net[p.net[k-1][kp]].size());

        fref[k][1] = p.g*yref[k][0] - p.a*yref[k][1] - infection2[k];

    }
    
    return GSL_SUCCESS;

} //********** end function dydt definition ********************************************************


#endif // SIS_DYN_HPP_INCLUDED
