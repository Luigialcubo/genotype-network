/**
 * @file   tevol_source.cpp
 * @brief  ODE system for epidemics  with genotype networks
 *
 * Source code.
 * Requires the gsl (numerical integrator) and boost library (rng and data structure)
 * g++ -std=c++11 -O3 -o tevol_source ./tevol_source.cpp $(gsl-config --cflags) $(gsl-config --libs)
 *
 * @author  LHD
 * @since   2014-06-10
 * @date    2022-07-20
 */

#include <iostream>

#include <cmath>
#include <algorithm>
#include <vector>
#include <fstream>
#include <sstream>

#include <boost/multi_array.hpp>
#include <gsl/gsl_errno.h>
#include <gsl/gsl_odeiv.h>

#include "./src/sirmuta_dyn.hpp"
//elements of BOOST library (for random number generator)
  #include <boost/random/mersenne_twister.hpp>
  #include <boost/random/uniform_real.hpp>
  #include <boost/random/variate_generator.hpp>

//Random number generator
  boost::mt19937 generator(static_cast<unsigned int>(time(NULL))); //
  boost::uniform_real<> uni_dist(0,1);
  boost::variate_generator<boost::mt19937&, boost::uniform_real<> > uni(generator, uni_dist);


using namespace std;


int main(int argc, const char *argv[]) {

//READING ARGUMENTS
int N = atoi(argv[1]); //number of strains
double mu = atof(argv[2]); //mutation probability
double b = atof(argv[3]); //transmission rate
string b_str = (argv[3]); //transmission rate string
double g = atof(argv[4]); //recovery rate
double ad0 = atof(argv[5]); //exponential rate of transcending immunity
string ad0_str = (argv[5]); //transcending immunity string
double ad = 1.0/ad0;
if(ad0 == 0) ad = 666; //avoid a divide by zero for no transcending immunity
double a = atof(argv[6]); //waning immunity rate
string network = argv[7]; //full path to network file (./data/SimulationInput/BWetal2022_LCC.edge)
string paths = argv[8]; //full path to network file (./data/SimulationInput/BWetal2022_LCC.paths)

double density = atof(argv[9]);
double kernel = atof(argv[10]);

string base = "./last_run_timeevol.dat"; //outputfile for time series
string ext = ".dat";
string outputname = base+ext;
ofstream output_time(outputname.c_str());

//READING INPUT
//reading genotype network
vector< vector<int> > GenoNet; GenoNet.resize(N);
ifstream EdgeList(network.c_str());
vector<int> d(N,0);
if (!EdgeList.is_open()) {
	cerr<<"network file does not exist. SNAFU!"<<endl;
 	return EXIT_FAILURE;
} //end if
else {
	int node1; int node2;
	while(!EdgeList.eof()) {
		EdgeList >> node1 >> std::ws >> node2 >> std::ws;
		GenoNet[node1].push_back(node2);
		GenoNet[node2].push_back(node1);
		++d[node1]; ++d[node2];
	} //end while
} //end else

//check for strain to be vaccinated against
//int v = int(floor(uni()*N));
//for(int test=0; test<N; ++test) if(d[test]>d[v]) v = test;

//reading shortest paths matrix
vector< vector<int> > distance; distance.resize(N);
ifstream PathMat (paths.c_str());
if (!PathMat.is_open()) {
	cerr<<"shortest path length file does not exist. SNAFU!"<<endl;
 	return EXIT_FAILURE;
} //end if
else {
	int first = 0; int voisin = 0; int ell;
	while(!PathMat.eof()) {
		PathMat >> ell >> std::ws;
		distance[first].push_back(ell); ++voisin;
		if(voisin==N) {++first; voisin=0;}
	} //end while
} //end else

int dim = 2;

//Objects passed to the function to be integrated
Sparam param = {N,mu,b,g,ad,a,GenoNet,distance,dim};

// Integrator parameters
double t = 0;
double dt = 1e-6;
double t_step = 10; //time step for output of time series
const double eps_abs = 1e-8;
const double eps_rel = 1e-8;

// Setting initial conditions
const double epsilon = 1e-5;//-1e-6; //initial infection
typedef boost::multi_array<double,2> mat_type;
typedef mat_type::index index;
mat_type y(boost::extents[N+1][dim]);
fill(y.data(),y.data()+y.num_elements(),0.0);

// Point initial conditions
int pzero = int(floor(uni()*N));
y[0][0] = (1.0-epsilon);
y[pzero][0] = epsilon;
double I = epsilon;
double S = 1.0-epsilon;

//General outbreak initial conditions
//for(int strain=1; strain<=N; ++strain) y[strain][0] = 1.0/N;
//double I = 1.0;
//double S = 0.0;

// Define GSL odeiv parameters
const gsl_odeiv_step_type * step_type = gsl_odeiv_step_rkf45;
gsl_odeiv_step * step = gsl_odeiv_step_alloc (step_type, 1.0*(N+1)*dim);
gsl_odeiv_control * control = gsl_odeiv_control_y_new (eps_abs,eps_rel);
gsl_odeiv_evolve * evolve = gsl_odeiv_evolve_alloc (1.0*(N+1)*dim);
gsl_odeiv_system sys = {dydt, NULL, (N+1)*dim, &param};

double lastI = 0.0;
double lastS = 1.0;
double lastR = 0.0;
double diff = abs(I-lastI);
for(int strain=1; strain<=N; ++strain) lastI += y[strain][0];
		
//Integration
int status(GSL_SUCCESS);
for (double t_target = t+t_step; t < 25000; t_target += t_step ) {

    while (t < t_target) {
        status = gsl_odeiv_evolve_apply (evolve,control,step,&sys,&t,t_target,&dt,y.data());
        if (status != GSL_SUCCESS) {
    		cout << "SNAFU" << endl;
            break;
    	}
    } // end while
    I = 0.0; S = y[0][0]; double N2=0.0; double maxI = 0.0;
    for(int strain=1; strain<=N; ++strain) {I += y[strain][0]; N2+=y[strain][0]+y[strain][1]; if(y[strain][0]>maxI) maxI=y[strain][0];}
    diff = abs(I-lastI) + abs(N2-I-lastR);
    lastI = I;
    lastS = S;
    lastR = N2 - I;
    output_time << t << " "  << y[0][0]  << " " << I;
    for(int strain=1; strain<=N; ++strain) output_time << " " << y[strain][0];
    output_time << "\n";
} // end for

//output final state
I = 0.0;double N2=0.0; double maxI = 0.0;
for(int strain=1; strain<=N; ++strain) {I += y[strain][0]; N2+=y[strain][0]+y[strain][1]; if(y[strain][0]>maxI) maxI=y[strain][0];}
cout << b  << " " << kernel << " " << ad0 << " "  << I << " " << maxI << " " << N2-I << "\n";

// Free memory
gsl_odeiv_evolve_free(evolve);
gsl_odeiv_control_free(control);
gsl_odeiv_step_free(step);
 
return 0;

}

