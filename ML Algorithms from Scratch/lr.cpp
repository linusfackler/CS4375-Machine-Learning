//
//  main.cpp
//  LogReg_Scratch
//
//  Created by Fernando Colman on 10/1/22.
//

#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <chrono>
#include <ctime>

using namespace std;

vector<double> sigmoid(vector<double> inputMatrix);
void printValues(vector<int> inputMatrix);
void printValues(vector<double> inputMatrix);
vector<double> matrix_mult(vector<vector<int>> fmat, vector<double> smat);
vector<double> calc_prob(vector<vector<int>> fmat, vector<double> smat);
vector<double> calc_err(vector<int> actual, vector<double> pred);
vector<double> update_weights(vector<vector<int>> data_matrix, vector<double> weights, vector<double> error);
double calc_accuracy(vector<int> actual, vector<double> prediction);
double calc_spec(vector<int> actual, vector<double> prediction);
double calc_sens(vector<int> actual, vector<double> prediction);


int main(int argc, const char * argv[]) {
  std::chrono::time_point<std::chrono::system_clock> start, end;
  string inputLine, dataInput;
  ifstream fileIn;
  
 // vectors to hold each attribute with train/test split
  vector<int> pclassTrain(1500);
  vector<int> survivedTrain(1500);
  vector<int> sexTrain(1500);
  vector<int> ageTrain(1500);
  
  vector<int> pclassTest(1500);
  vector<int> survivedTest(1500);
  vector<int> sexTest(1500);
  vector<int> ageTest(1500);
  
  
  
  
  
  // OPENING TITANIC FILE
  cout << "Opening titanic.csv";
  fileIn.open("titanic.csv");
  if(!fileIn.is_open()) {
    cout << "Failed to open titanic.csv" << endl;
    return 1;
  }
  
  getline(fileIn, inputLine, ',');
  getline(fileIn, inputLine);
  cout << "\nHeadings: " << inputLine << endl;
  
  // POPULATE VECTORS WITH TITANIC DATA
  // First 800 rows are train, rest are test data
  int numRows = 0;
  while(fileIn.good()) {
    if(numRows < 800) {
      getline(fileIn, dataInput, ','); // ignore passenger number
      
      getline(fileIn, dataInput, ','); // passengerClass
      pclassTrain.at(numRows) = stoi(dataInput);
      
      getline(fileIn, dataInput, ','); // survived
      survivedTrain.at(numRows) = stoi(dataInput);
      
      getline(fileIn, dataInput, ','); // sex
      sexTrain.at(numRows) = stoi(dataInput);
      
      getline(fileIn, dataInput, '\n'); // age
      ageTrain.at(numRows) = stoi(dataInput);
    }
    else {
      getline(fileIn, dataInput, ',');
      
      getline(fileIn, dataInput, ',');
      pclassTest.at(numRows-800) = stoi(dataInput);
      
      getline(fileIn, dataInput, ',');
      survivedTest.at(numRows-800) = stoi(dataInput);
      
      getline(fileIn, dataInput, ',');
      sexTest.at(numRows-800) = stoi(dataInput);
      
      getline(fileIn, dataInput, '\n');
      ageTest.at(numRows-800) = stoi(dataInput);
    }
    
    numRows++;
  }
  
  
  pclassTrain.resize(800);  //deallocate empty space
  pclassTest.resize(numRows-800);
  survivedTrain.resize(800);
  survivedTest.resize(numRows-800);
  sexTrain.resize(800);
  sexTest.resize(numRows-800);
  ageTrain.resize(800);
  ageTest.resize(numRows-800);
  
  // CLOSING TITANIC FILE
  cout << "Closing titanic.csv" << endl;
  fileIn.close();
  cout << "Number of observations: " << numRows << endl;
  
  // SET UP WEIGHT, DATA VECTORS, AND VARIABLES
  vector<double> probs;
  vector<double> error;
  vector<double> weights = {1, 1, 1, 1}; //c+1 x 1
  
  
  vector<vector<int>> data_matrix(pclassTrain.size()); // n x c+1
  for(int r = 0; r < pclassTrain.size(); r++) {
    data_matrix[r] = {1, pclassTrain.at(r), sexTrain.at(r), ageTrain.at(r)};
  }
  
  // GRADIENT DESCENT
  start = std::chrono::system_clock::now();
  for(int i = 0; i < 10000; i++) {
    probs = calc_prob(data_matrix, weights);
    error = calc_err(survivedTrain, probs);
    //printValues(weights);
    weights = update_weights(data_matrix, weights, error);
    
    //printValues(weights);
  }
  end = std::chrono::system_clock::now();
  
  cout << "Coefficients : " << endl;
  printValues(weights);
  
  // PREDICT WITH TEST
  vector<vector<int>> test_matrix(pclassTest.size()); // n x c+1
  for(int r = 0; r < pclassTest.size(); r++) {
    test_matrix[r] = {1, pclassTest.at(r), sexTest.at(r), ageTest.at(r)};
  }
  
  vector<double> predicted = calc_prob(test_matrix, weights);
  //error = calc_err(survivedTrain, probs);
  
  
  //printValues(error);
  
  double e = 2.7182;
  double sum;
  vector<double> probabilities;
  for(int i = 0; i < predicted.size(); i++) {
    sum = pow(e, predicted[i]) / (1 + pow(e, predicted[i]));
    probabilities.push_back(sum);
  }
  
  //printValues(probabilities);
  
  double accuracy = calc_accuracy(survivedTest, probabilities);
  double sensitivity = calc_sens(survivedTest, probabilities);
  double specificity = calc_spec(survivedTest, probabilities);
  cout << "Accuracy: " << accuracy << endl;
  cout << "Sensitivity: " << sensitivity << endl;
  cout << "Specificity: " << specificity << endl;
  
  std::chrono::duration<double> elapsed = end - start;
  cout << "Duration of Algorithm: " << elapsed.count() << endl;
  
  

  
   
  return 0;
  
}

// sigmoid needs to be 2D vectors
vector<double> sigmoid(vector<double> inputMatrix) {
  
  vector<double> res;
  double e = 2.7182;
  
  for(int i = 0; i < inputMatrix.size(); i++) {
    res.push_back(1.0 / (1.0 + pow(e, -(inputMatrix.at(i)))));
  }
  
  return res;
}


void printValues(vector<int> inputMatrix) {
  int i = 0;
  for(int num: inputMatrix) {
    cout << i << ": " << num << endl;
    i++;
  }
}

void printValues(vector<double> inputMatrix) {
  int i = 0;
  for(double num: inputMatrix) {
    cout << i << ": " << num << endl;
    i++;
  }
}

double calc_accuracy(vector<int> actual, vector<double> prediction) {
  int wrong = 0;
  int correct = 0;
  for(int i = 0; i < actual.size(); i++) {
    if(prediction[i] > 0.5 && actual[i] == 1)
      correct++;
    else if(prediction[i] > 0.5 && actual[i] == 0)
      wrong++;
    else if(prediction[i] < 0.5 && actual[i] == 1)
      wrong++;
    else
      correct++;
  }
  
  return correct/(wrong+correct);
}

double calc_spec(vector<int> actual, vector<double> prediction) {
  int wrong = 0;
  int correct = 0;
  int trueNeg = 0, falsePos = 0;
  for(int i = 0; i < actual.size(); i++) {
    if(prediction[i] > 0.5 && actual[i] == 1) {
      correct++;
    }
    else if(prediction[i] > 0.5 && actual[i] == 0) {
      wrong++;
      falsePos++;
    }
    else if(prediction[i] < 0.5 && actual[i] == 1) {
      wrong++;
    }
    else {
      correct++;
      trueNeg++;
    }
  }
  
  return trueNeg/(trueNeg+falsePos);
}

double calc_sens(vector<int> actual, vector<double> prediction) {
  int wrong = 0;
  int correct = 0;
  int truePos = 0, falseNeg = 0;
  for(int i = 0; i < actual.size(); i++) {
    if(prediction[i] > 0.5 && actual[i] == 1) {
      correct++;
      truePos++;
    }
    else if(prediction[i] > 0.5 && actual[i] == 0) {
      wrong++;
      
    }
    else if(prediction[i] < 0.5 && actual[i] == 1) {
      wrong++;
      falseNeg++;
    }
    else {
      correct++;
      
    }
  }
  
  return truePos/(truePos+falseNeg);
}


vector<double> calc_err(vector<int> actual, vector<double> pred) {
  vector<double> err;
  for(int r = 0; r < actual.size(); r++) {
    err.push_back(actual[r] - pred[r]);
  }
  return err;
}

vector<double> calc_prob(vector<vector<int>> data_matrix, vector<double> weights){
  vector<double> hold = matrix_mult(data_matrix, weights);
  //printValues(hold);
  
  vector<double> prob_vector = sigmoid(hold);
  //printValues(prob_vector);
  return prob_vector;
}

vector<double> matrix_mult(vector<vector<int>> fmat, vector<double> smat) {
  vector<double> ans;
  int sum;
  
  for(int r = 0; r < fmat.size(); r++) {
    sum = fmat[r][0]*smat[0] + fmat[r][1]*smat[1] + fmat[r][2]*smat[2] + fmat[r][3]*smat[3];
    ans.push_back(sum);
  }
  
  return ans;
  
}

vector<double> update_weights(vector<vector<int>> data_matrix, vector<double> weights, vector<double> error) {
  vector<double> dataErrors;
  double learningRate = 0.001;
  
  // MULTIPLY DATA_MATRIX BY ERROR
  double errorHold = 0;
  for(int r = 0; r < data_matrix[0].size(); r++) {
    for(int c = 0; c < data_matrix.size(); c++) {
      errorHold += error[c]*data_matrix[c][r]; // transpose data_matrix
    }
    dataErrors.push_back(errorHold * learningRate + weights[r]);
  }
  
  return dataErrors;
}
