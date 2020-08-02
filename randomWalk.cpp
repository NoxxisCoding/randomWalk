#include <stdio.h>
#include <math.h>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <cstdlib>
#include <cmath>
#include <complex>

using namespace std;

const int numWalkers = 600;  //NUMBER OF WALKERS
const int numSteps = 1000;  //NUMBER OF STEPS

double q1 = 1 / sqrt(numSteps), q2 = 1, q3 = sqrt(numSteps);

int main(int argc, char const* argv[])
{
    //SET OUTPUT TEXT FILES
    ofstream probTwoOut;
    ofstream posMean;
    probTwoOut.open("positionVSProbabilityOutput2.csv"); //FILE FOR SECOND PROBABILITY OUTPUT
    posMean.open("positionVSMeanSquareOutput.csv");

  
    //aStepSquared IS WHERE A STEP OF A WALKER IS SQUARED
    int* aStepSquared = new int[numSteps];
    int* aStepSquared2 = new int[numSteps];

    double* cosRq1 = new double[numSteps];
    double* cosRq2 = new double[numSteps];
    double* cosRq3 = new double[numSteps];

    double* cosSqrdRq1 = new double[numSteps];
    double* cosSqrdRq2 = new double[numSteps];
    double* cosSqrdRq3 = new double[numSteps];

    for (int itime = 0; itime < numSteps; itime++) {
        aStepSquared[itime] = 0;
        aStepSquared2[itime] = 0;

        cosRq1[itime] = 0.0;
        cosRq2[itime] = 0.0;
        cosRq3[itime] = 0.0;

        cosSqrdRq1[itime] = 0.0;
        cosSqrdRq2[itime] = 0.0;
        cosSqrdRq3[itime] = 0.0;
    }

    //SET PROBABILITY VARIABLES
    double* probability = new double[2 * numSteps + 1]; //ARRAY TO HOLD PROBABILITY OF EACH POSITION TO BE VISITED
    int* countNumberOfVisits = new int[numSteps * 2 + 1]; //ARRAY TO COUNT THE NUMBER OF VISITS PER POSITION

    for (int position = 0; position < (2 * numSteps + 1); position++) {
        countNumberOfVisits[position] = 0;
    }

    //FOR LOOP FOR EACH WALKER
    for (int kWalkr = 0; kWalkr < numWalkers; kWalkr++) {
        int currentPosition = 0; //CURRENT POSITION OF WALKER

        //FOR LOOP - FOR EACH STEP TAKEN
        for (int itime = 0; itime < numSteps; itime++)
        {
            //HEAD OR TAIL CONCEPT
            int randomNum = rand() % 2;

            //==============================
            if (randomNum == 1)
                currentPosition++;
            else
                currentPosition--;
            //==============================
            //cout << "==== New position is: " << currentPosition << endl;

            //COMPUTE aStepSquared[itime] and cosRq1, cosRq2, cosRq3
            aStepSquared[itime] += (currentPosition * currentPosition);
            aStepSquared2[itime] += pow(currentPosition, 4);

            // cos (q*R)
            cosRq1[itime] += cos(q1 * currentPosition);
            cosRq2[itime] += cos(q2 * currentPosition);
            cosRq3[itime] += cos(q3 * currentPosition);

            //cos^2 (q*R)
            cosSqrdRq1[itime] += cos(q1 * currentPosition) * cos(q1 * currentPosition);
            cosSqrdRq2[itime] += cos(q2 * currentPosition) * cos(q2 * currentPosition);
            cosSqrdRq3[itime] += cos(q3 * currentPosition) * cos(q3 * currentPosition);

            //cout << "==== The squared displacement is x^2 = " << aStepSquared[itime] << endl << endl;

        }
        //cout << endl;
        //INCREMENT countNumberOfVisits[] EVERYTIME currentPosition IS VISITED
        countNumberOfVisits[currentPosition + numSteps] += 1;
    }

    //Mean Square
    for (int itime = 0; itime < numSteps; itime++) {
        double meanSquare = (double)aStepSquared[itime] / (double)numWalkers;
        double meanSquare2 = (double)aStepSquared2[itime] / (double)numWalkers;

        //<cos(q*R)>
        cosRq1[itime] = (double)cosRq1[itime] / (double)numWalkers;
        cosRq2[itime] = (double)cosRq2[itime] / (double)numWalkers;
        cosRq3[itime] = (double)cosRq3[itime] / (double)numWalkers;

        //<cos^2(q*R)>
        cosSqrdRq1[itime] = (double)cosSqrdRq1[itime] / (double)numWalkers;
        cosSqrdRq2[itime] = (double)cosSqrdRq2[itime] / (double)numWalkers;
        cosSqrdRq3[itime] = (double)cosSqrdRq3[itime] / (double)numWalkers;

        //standard deviation of the mean

        double stdevMeanSquare = (double)sqrt(meanSquare2 - meanSquare * meanSquare);
        double stdErrorOfMean = (double)(stdevMeanSquare / sqrt(numWalkers));

        double stdevCosSqrdRq1 = (double)sqrt(cosSqrdRq1[itime] - (cosRq1[itime] * cosRq1[itime]));
        double stdErrorCosSqrdRq1 = (double)(stdevCosSqrdRq1 / sqrt(numWalkers));

        double stdevCosSqrdRq2 = (double)sqrt(cosSqrdRq2[itime] - (cosRq2[itime] * cosRq2[itime]));
        double stdErrorCosSqrdRq2 = (double)(stdevCosSqrdRq2 / sqrt(numWalkers));

        double stdevCosSqrdRq3 = (double)sqrt(cosSqrdRq3[itime] - (cosRq3[itime] * cosRq3[itime]));
        double stdErrorCosSqrdRq3 = (double)(stdevCosSqrdRq3 / sqrt(numWalkers));

        // .csv file
        posMean << itime << ", " << meanSquare << "," << stdErrorOfMean << "," << cosRq1[itime] << "," << cosRq2[itime] << "," << cosRq3[itime] << "," << stdErrorCosSqrdRq1 << "," << stdErrorCosSqrdRq2 << "," << stdErrorCosSqrdRq3 << endl;
    }

    //cout << endl << endl;

    //OTHER PROBABILITY REQUESTED
    //UTILIZING ONLY THE VISITS ACTUALLY DONE
    //==================================

    //cout << endl;
    //cout << "Probability Values:" << endl << endl;

    for (int i = 0; i < (2 * numSteps + 1); i += 2)
    {
        //COMPUTE PROBABILITY OF POSITION i
        probability[i] = (double)countNumberOfVisits[i] / (double)numWalkers;
        //if (countNumberOfVisits[i] != 0) {
        //    cout << "Total Steps and Probability of Walker To Be At Distance " << i - numSteps << ": " << countNumberOfVisits[i] << ", " << probability[i] << endl;
        //}
        probTwoOut << i - numSteps << ", " << probability[i] << endl;  // Output(x,y): POSITION VS PROBABILITY
    }

    probTwoOut.close();
    posMean.close();

    cout << endl << "Done";

    return 0;

}
