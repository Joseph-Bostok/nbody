# nbody
time calculations in ms(centaurus):
-solar system(8) at 200, 5000000, 500: 7645ms
-100 particles dt=1 for 10000 steps: Time taken: 2110ms
-1000 particles dt=1 for 10000 steps: 204470ms
_______random particle and steps below____________________________________________
500 particles dt= 1	50,000 steps 1,000 dump interval: 255766ms
5000 particles dt= 1 50,000 steps 1,0000 dump interval: (ran out of time)



time calculations in ms(local machine):
-solar system(8) at 200, 5000000, 500: 9520ms 
-100 particles dt=1 for 10000 steps: Time taken: 2789ms
-1000 particles dt=1 for 10000 steps: Time taken: 213930ms
_______random particle and steps below____________________________________________
500 particles dt= 1	50,000 steps 1,000 dump interval: 419334ms
5000 particles dt= 1 50,000 steps 1,0000 dump interval:(failed on local machine)


All of the PDF's are based on the assignment details: "How long does it take to simulate the solar system with dt = 200 and for 5000000
 steps? How long does it take to simulate 100 particles dt=1 for 10000 steps? How long does it take to
 simulate 1000 particles dt=1 for 10000 steps?"

 I did it on my machine and also on centaurus.


references for code:
https://www.geeksforgeeks.org/chrono-in-c/
chatGPT prompt: how can i run output.tsv on that python file | i asked how to run the commands to make the plot.py work with .tsv
chatGPT prompt: how would i approach this nbody problem? can you show me an example of nbody working?
https://rosettacode.org/wiki/N-body_problem
https://patterns.eecs.berkeley.edu/?page_id=193
https://www.geeksforgeeks.org/makefile-in-c-and-its-applications/

This was a very interesting assignment and I have never tackled anything like this before.

I added extensive comments throughout my code, and used IntelliSense to give a comprehsive overview of the functions.

After the lecture on 2/12, I got feedback about using namespace std. this code DOES use std since I created it before getting the feedback.

the code starts wth the creation of the gravity and softening constant. I used something found through the rosetta code website on the softening and just threw it in. the struct created is for the particle attributes such as mass, position, velocity and force. 

Next is the nbodySim() which is the constructor for the number of particles, time step, number of steps and the dump state interval.

initializeParticles() initializes particles with random values, they are given random values which are seeded, the mass is between 1-1000, position is -50-500, velocity is -50-50.

computeForces() computes the gravitational force on each particle to all other particles. the function basically interates over all particles and computes gravitational force on each particle. it starts by setting the force to 0, all the particles get set with values and then the forces get added to both particles.

updatePositions() updates the position of the particles. it computes acceleration, updates velocity and updates position.

the outputState() was difficult for me to figure out, ive never worked with tsv files before, but this did end up working after doing some research 

runSimulation() just runs the nbody sim for a certain amount of steps, the state and dumps to a specific file.

the main() pases the comand line and creates the instance of nbodySim(). I added a timer to see how fast everything was running on different sample sets.

Script to run the file on centaurus:

#!/bin/bash
#SBATCH --job-name=myjob
#SBATCH --partition=Centaurus
#SBATCH --time=02:00:00
#SBATCH --nodes=1
#SBATCH --ntasks-per-node=1
#SBATCH --mem=32G

#this will be read by slurm for arguments, then exectuted on each node(s) that you request
#ntasks is the core count

./nbody 8 200 5000000 5000
//I DID NOT RUN THE SCRIPT LIKE THIS, THIS WAS THE COMMAND I USED AND UPDATED PER BATCH
./nbody 100 1 10000 1000 
./nbody 1000 1 10000 100


