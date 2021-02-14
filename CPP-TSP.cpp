#include <iostream>
#include <algorithm>
#include <array>
#include <vector>
#include <random>
#include <ctime>
#include <cstdlib>
using namespace std;


class TravelingSalesPerson {
private:
    //create data set, travel matrix, and population
    vector<int> dataSet;
    vector<vector<int>> travelMatrix;
    vector<int> populationSet[1000];
    const int POPULATION_SIZE = 1000;
public:
   
    TravelingSalesPerson(vector<int>, vector<vector<int>>);
    vector<int> getDataset();
    vector<int> runGeneticAlgorithm();
    void createPopulation();
    int calculateFitnessValue(vector<int>&);
    vector<int> randomSelection(vector<int>&, int);
    vector<int> reproduce(vector<int>&, vector<int>&);
    vector<int> mutate(vector<int>&);
};

//takes in a vetor with givenData and a vector of vectors containig the travel matrix. sets vars to appropriate vals
TravelingSalesPerson::TravelingSalesPerson(vector<int> givenData, vector<vector<int>> givenMatrix) {
    dataSet = givenData;
    travelMatrix = givenMatrix;

}

//fitness func
int TravelingSalesPerson::calculateFitnessValue(vector<int>& individual) {
    int fitnessValue = 0;
    //go though indiv vector and add all the distances 
    for (int i = 0; i < individual.size() - 1; i++) {
        int value_1 = individual.at(i) - 1;
        int value_2 = individual.at(i + 1) - 1;
        fitnessValue += travelMatrix[value_1][value_2];
    }
    //return the total distance aka fitness
    return fitnessValue;
}

//get Dataset
vector<int> TravelingSalesPerson::getDataset() {
    return dataSet;
}

vector<int> TravelingSalesPerson::runGeneticAlgorithm() {
    //init
    vector<int> fitnessValues;
    vector<int> newPopulation[1000];
    int tempFitnessValue;
    int totalFitness = 0;
    int fitnessRatio = 0;

    //go through pop, get fitness for all and push onto fitnessValues
    for (int i = 0; i < POPULATION_SIZE; i++) {
        vector<int> individual = populationSet[i];
        tempFitnessValue = calculateFitnessValue(individual);
        fitnessValues.push_back(tempFitnessValue);
        totalFitness += fitnessValues[i];
    }

    int randomInt = 0;
    //amount of generations 
    for (int p = 0; p < 30; p++) {
        int popSize = 0;
        for (int k = 0; k < POPULATION_SIZE; k++) {
            // Choosing x
            vector<int> parent_x;
            vector<int> parent_y;
            vector<int> child;
            for (int j = 0; j < POPULATION_SIZE; j++) {
                randomInt = rand() % totalFitness;
                fitnessRatio = (totalFitness / fitnessValues[j]);
                if (randomInt < fitnessRatio) {
                    parent_x = populationSet[j];
                    break;
                }
            }
            // choosing y
            for (int n = 0; n < POPULATION_SIZE; n++) {
                randomInt = rand() % totalFitness;
                fitnessRatio = (totalFitness / fitnessValues[n]);
                if (randomInt < fitnessRatio) {
                    parent_y = populationSet[n];
                    break;
                }
            }
            // a child is born...woah
            child = reproduce(parent_x, parent_y);

            //mutate randomly 
            randomInt = rand() % 100;
            if (randomInt < 2) {
                child = mutate(child);
            }
            newPopulation[k] = child;
        }
        while (popSize < POPULATION_SIZE) {
            populationSet[popSize] = newPopulation[popSize];
            popSize++;
        }

    }

    //go through all individuals and return highest fitness
    vector<int> bestIndividual;
    int bestFitValue = 0;
    for (int i = 0; i < POPULATION_SIZE; i++) {
        vector<int> individual = populationSet[i];
        int indFitValue = calculateFitnessValue(individual);
        if (bestFitValue == 0) {
            bestIndividual = individual;
            bestFitValue = indFitValue;
        }
        else if (bestFitValue > indFitValue) {
            bestIndividual = individual;
            bestFitValue = indFitValue;
        }
        else {
            continue;
        }
    }

    return bestIndividual;

}

vector<int> TravelingSalesPerson::mutate(vector<int>& child) {
    vector<int> newChild = child;
    int randomInt = rand() % 9;
    int randomInt2 = rand() % 8;

    if (randomInt <= randomInt2) {
        randomInt2++;
    }

    swap(newChild[randomInt], newChild[randomInt2]);

    return newChild;
}

//makin babies 
vector<int> TravelingSalesPerson::reproduce(vector<int>& x, vector<int>& y) {
    vector<int> child;
    int startBP = rand() % 5;
    int endBP = startBP + 3;
    vector<int> subString;
    bool duplicate = false;
    int k = 0;
    int childCount = 0;
    int yCount = 0;
    //grab a piece of one parents genes
    for (int i = startBP; i < endBP; i++) {
        subString.push_back(x[i]);
    }
    //creating child
    while (yCount < 9) {
        if (childCount >= startBP && childCount < endBP) {
            child.push_back(x[childCount]);
            childCount++;
        }//test for duplicate cities
        else {
            for (int j = 0; j < subString.size(); j++) {
                if (y[yCount] == subString[j]) {
                    duplicate = true;
                    break;
                }
            }
            
            if (!duplicate) {
                child.push_back(y[yCount]);
                childCount++;
            }
            duplicate = false;
            yCount++;
        }

    }

    return child;
}
void TravelingSalesPerson::createPopulation() {
    auto tempPermutation = default_random_engine{};
    for (int i = 0; i < POPULATION_SIZE; i++) {

        shuffle(dataSet.begin(), dataSet.end(), tempPermutation);
        populationSet[i] = dataSet;
    }
}




int main() {
    srand(time(NULL));
    vector<int> cities = { 1,2,3,4,5,6,7,8,9 };
    vector<vector<int>> travelMatrix = {
                                {0,2,11,3,18,14,20,12,5},
                                {2,0,13,10,5,3,8,20,17},
                                {11,13,0,5,19,21,2,5,8},
                                {3,10,5,0,6,4,12,15,1},
                                {18,5,19,6,0,12,6,9,7},
                                {14,3,21,4,12,0,19,7,4},
                                {20,8,2,12,6,19,0,21,13},
                                {12,20,5,15,9,7,21,0,6},
                                {5,17,8,1,7,4,13,6,0}
    };
    TravelingSalesPerson tsp(cities, travelMatrix);

    tsp.createPopulation();
    vector<int> bestIndividual = tsp.runGeneticAlgorithm();

    cout << "The best individual fit found in this order:" << endl;
    for (int i = 0; i < bestIndividual.size(); i++) {
        cout << bestIndividual[i] << " ";
    }
    cout << endl;

    for (int i = 0; i < bestIndividual.size() - 1; i++) {
        int firstCity = bestIndividual[i];
        int secondCity = bestIndividual[i + 1];
        cout << firstCity << " -> " << secondCity << " is " << travelMatrix[firstCity - 1][secondCity - 1] << " units." << endl;
    }
    cout << endl;

    cout << "Total distance: " << tsp.calculateFitnessValue(bestIndividual) << endl;;


    return 0;
}