#include <iostream>
#include <vector>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <chrono>

using namespace std;

const double G = 6.674 * 1e-11;
const double softening = 1e-5;

struct particle{
    double mass;
    double x,y,z; //position
    double vx,vy,vz; //velocity
    double fx,fy,fz; //force
};

class nbodySim{
private:
    vector<particle>particles;
    double dt;
    int numsteps;
    int dumpInterval;

public:
    /**
     * Constructor for nbodySima
     *
     * @param numParticles number of particles to simulate
     * @param dt time step of simulation
     * @param numsteps number of steps to take
     * @param dumpInterval interval at which to dump data to stdout
     */
    nbodySim(int numParticles, double dt, int numsteps, int dumpInterval) //constructor
    : dt(dt), numsteps(numsteps), dumpInterval(dumpInterval){ //initialize variables
        particles.resize(numParticles); //resize vector of particles
        initializeParticles(); //initialize particles with random values
    }

    /**
     * Initializes particles with random values
     *
     * Random values are seeded with the current time.
     * The mass of each particle is a random double between 1 and 1000.
     * The position of each particle is a random integer between -50 and 500.
     * The velocity of each particle is a random integer between -50 and 50.
     * The force on each particle is initialized to 0.
     */
    void initializeParticles(){
        srand(time(0));
        for(auto &p : particles){
            p.mass = 1.0 + (rand() % 1000) / 1000.0; //random mass between 1 and 1000
            p.x = rand() % 100 - 500; //random position between -50 and 500
            p.y = rand() % 1000 - 500;
            p.z = rand() % 1000 - 500;
            p.vx = rand() % 100 - 50; //random velocity between -50 and 50
            p.vy = rand() % 100 - 50;
            p.vz = rand() % 100 - 50;
            p.fx = p.fy = p.fz = 0.0;
            }
        }

    void computeForces(){
        for(auto &p : particles){
            p.fx = p.fy = p.fz = 0.0; //reset force
            }
            for(size_t i = 0; i < particles.size(); i++){
                for(size_t j = i + 1; j < particles.size(); j++){
                    double dx = particles[j].x - particles[i].x;
                    double dy = particles[j].y - particles[i].y;
                    double dz = particles[j].z - particles[i].z;
                    double distanceSquared = dx * dx + dy * dy + dz * dz + softening*softening; //distance between particles
                    double distance = sqrt(distanceSquared); //distance between particles
                    double force = G*particles[i].mass*particles[j].mass/distanceSquared; //force between particles
                    double fx = force * dx / distance;
                    double fy = force * dy / distance;
                    double fz = force * dz / distance;
                    //add force to both particles
                    particles[i].fx += fx;
                    particles[i].fy += fy;
                    particles[i].fz += fz;
                    particles[j].fx -= fx;  
                    particles[j].fy -= fy;
                    particles[j].fz -= fz;
        }
    }
}

    /**
     * Updates the positions of particles based on their velocities and time step
     *
     * This function iterates over all particles and updates their positions
     * based on their current velocity and the time step of the simulation.
     */
    void updatePositions(){
        for (auto &p : particles) {
            //compute acceleration
            double ax = p.fx / p.mass;
            double ay = p.fy / p.mass;
            double az = p.fz / p.mass;
            //update velocity
            p.vx += ax * dt;
            p.vy += ay * dt;
            p.vz += az * dt;
            //update position
            p.x += p.vx * dt;
            p.y += p.vy * dt;
            p.z += p.vz * dt;
        }
    }

/**
 * Outputs the current state of all particles to the specified file.
 *
 * This function writes the number of particles followed by each particle's
 * properties (mass, position, velocity, and force components) to the provided
 * output file stream, separated by tabs. The state is output for the given
 * simulation step.
 *
 * @param step The current simulation step.
 * @param outFile The output file stream to write the state to.
 */

    void outputState(int step, ofstream &outFile) {
        outFile << particles.size() << "\t";
        for (const auto &p : particles) {
            outFile << p.mass << "\t" << p.x << "\t" << p.y << "\t" << p.z << "\t"
                    << p.vx << "\t" << p.vy << "\t" << p.vz << "\t"
                    << p.fx << "\t" << p.fy << "\t" << p.fz << "\t";
        }
        outFile << endl;
    }

/**
 * Runs the N-body simulation for the specified number of steps.
 *
 * This function runs the simulation for the specified number of steps, dumping
 * the state of the particles to the specified file at the given interval.
 */
    void runSimulation() {
        ofstream outFile("output.tsv");
        for (int step = 0; step < numsteps; ++step) {
            computeForces();
            updatePositions();
            if (step % dumpInterval == 0) {
                outputState(step, outFile);
            }
        }
        outFile.close();
    }
};

/**
 * The main entry point of the program.
 *
 * This function parses the command line arguments, creates an instance of
 * nbodySim, and runs the simulation.
 *
 * @param argc The number of command line arguments.
 * @param argv The command line arguments.
 * @return 0 if the program runs successfully, 1 if the usage is incorrect.
 */
int main(int argc, char *argv[]) {
    if (argc != 5) {
        cerr << "Usage: " << argv[0] << " <numParticles> <dt> <numSteps> <dumpInterval>" << endl;
        return 1;
    }

    int numParticles = atoi(argv[1]);
    double dt = atof(argv[2]);
    int numSteps = atoi(argv[3]);
    int dumpInterval = atoi(argv[4]);

    nbodySim simulation(numParticles, dt, numSteps, dumpInterval);
    auto startTime = chrono::high_resolution_clock::now();//start timer
    simulation.runSimulation();
    auto endTime = chrono::high_resolution_clock::now(); //end timer
    auto duration = chrono::duration_cast<chrono::milliseconds>(endTime - startTime); //calculate time taken
    cout << "Time taken: " << duration.count() << "ms" << endl;

    return 0;
}