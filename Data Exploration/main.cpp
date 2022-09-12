#include <iostream>
#include <vector>
#include <fstream>
#include <cmath>

using namespace std;

double vectorSum(vector<double> a);
double vectorMean(vector<double> a);
double vectorMedian(vector<double> a);
double vectorRange(vector<double> a);

void swapVector(double *x, double *y);
int partition (vector<double> &a, int start, int end);
void quickSort(vector<double> &a, int start, int end);

double covar(vector<double> a, vector<double> b);
double cor(vector<double> a, vector<double> b);
void print_stats(vector<double> a);

int main(int argc, char** argv)
{
    ifstream inFS;
    string line;
    string rm_in, medv_in;
    const int MAX_LEN = 1000;
    vector<double> rm(MAX_LEN);
    vector<double> medv(MAX_LEN);

    cout << "Opening file Boston.csv." << endl;
    // opening data file
    inFS.open("Boston.csv");
    if (!inFS.is_open()){
        cout << "Could not open file Boston.csv." << endl;
        return 1;
    }

    // reading in data from csv file
    cout << "Reading line 1" << endl;
    getline(inFS, line);

    cout << "heading: " << line << endl;

    int numObservations = 0;
    while (inFS.good()) {
        getline(inFS, rm_in, ',');
        getline(inFS, medv_in, '\n');

        rm.at(numObservations) = stof(rm_in);
        medv.at(numObservations) = stof(medv_in);

        numObservations++;
    }

    rm.resize(numObservations);
    medv.resize(numObservations);

    cout << "new length " << rm.size() << endl;

    cout << "Closing file Boston.csv." << endl;
    inFS.close();

    cout << "Number of records: " << numObservations << endl;

    cout << "\nStats for rm" << endl;
    print_stats(rm);

    cout << "\nStats for medv" << endl;
    print_stats(medv);

    cout << "\n Covariance = " << covar(rm, medv) << endl;
    cout << "\n Correlation = " << cor(rm, medv) << endl;

    cout << "\nProgram terminated.";

    return 0;
}

double vectorSum(vector<double> a)
{
    // returns sum of vector a from 0 to n-1
    int n = a.size();
    double sum = 0.0;
    for(int i = 0; i < n; i++)
        sum += a[i];
    
    return sum;
}

double vectorMean(vector<double> a)
{
    // using prev. coded funtion vectorSum to create sum
    // div by number of elements
    return vectorSum(a) / a.size();
}

double vectorMedian(vector<double> a)
{
    vector<double> temp = a;
    // creating temp vector so we don't sort original vector
    quickSort(temp, 0, a.size() - 1);
    // need to sort the vector first to be able to get middle value
    int n = temp.size();

    if (n % 2 == 0)
        return (((temp[n / 2]) + temp[(n / 2) - 1]) / 2);
        // if n is even, return average of 2 middle values

    return temp[(n + 1) / 2];
    // if n is odd, return middle value
}

double vectorRange(vector<double> a)
{
    vector<double> temp = a;
    // creating temp vector so we don't sort original vector
    int n = temp.size();
    quickSort(temp, 0, n - 1);
    // need to sort first to easily find min and max value
    return (temp[n - 1] - temp[0]);
    // return max - min of vector temp
}

void swapVector(double *x, double *y)
{
    double temp = *x;
    *x = *y;
    *y = temp;
    // simple swap code, was not sure if we could use built-in
}

int partition (vector<double> &a, int start, int end)
{
    int s = start, piv = end;
    for (int i = start; i < end; i++)
    {
        if (a[i] < a[piv]) {
            swapVector(&a[i], &a[s]);
            s++;
        }
    }
    swapVector(&a[s], &a[piv]);
    return s;
}

void quickSort(vector<double> &a, int start, int end)
{
    if (start < end) {
        int p = partition(a, start, end);
        quickSort(a, start, p - 1);
        quickSort(a, p + 1, end);
    }
    // Quick Sort algorithm to sort vector
    // since we can't use built-in functions
    // Worst Case Complexity O(n^2)
}

double covar(vector<double> a, vector<double> b)
{
    if (a.size() != b.size()) {
        cout << "Vectors are not of same size." << endl;
        return 0;
        // cannot calculate covariance if vector sizes are not the same
    }
        
    double temp = 0.0;
    double meanA = vectorMean(a);
    double meanB = vectorMean(b);
    int n = a.size();
    
    for (int i = 0; i < n; i++)
        temp += (a[i] - meanA) * (b[i] - meanB);

    return (temp / (n - 1));
}

double cor(vector<double> a, vector<double> b)
{
    double cov = covar(a, b);
    double sigmaA = sqrt(covar(a, a));
    double sigmaB = sqrt(covar(b, b));
    return (cov / (sigmaA * sigmaB));
    // returns correlation of a and b
}

void print_stats(vector<double> a)
{
    cout << "Sum: " << vectorSum(a) << endl;
    cout << "Mean: " << vectorMean(a) << endl;
    cout << "Median: " << vectorMedian(a) << endl;
    cout << "Range: " << vectorRange(a) << endl;
    // print stats of vector
}