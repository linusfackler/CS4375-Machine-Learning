#include <iostream>
#include <vector>
#include <fstream>
#include <cmath>
#include <math.h>
#include <algorithm>
#include <iomanip>
#include <chrono>
#include <unistd.h>

# define M_PI           3.14159265358979323846

using namespace std;

double vectorSum(vector<double> a);
double vectorMean(vector<double> a);
double vectorMedian(vector<double> a);
double vectorRange(vector<double> a);

void swapVector(double *x, double *y);
int partition (vector<double> &a, int start, int end);
void quickSort(vector<double> &a, int start, int end);

void print_stats(vector<double> a);

vector<double> vectorSplit(vector<double> a, int l, int r);

vector<double> apriori(vector<double> a);
int nrow(vector<double> a, double target_a, vector<double> b, double target_b);
double mean_condition(vector<double> a, vector<double> c, int condition);
double var_condition(vector<double> a, vector<double> c, int condition);

vector<double> calc_raw_prob(
    int pclass, int sex, double age, double lh_pclass[2][3], double lh_sex[2][2],
    vector<double> ap, vector<double> age_mean, vector<double> age_var);
double calc_age_lh(double v, double mean_v, double var_v);

int main(int argc, char** argv)
{
    ifstream inFS;
    string line;
    string rownum_in, pclass_in, survived_in, sex_in, age_in;

    const int MAX_LEN = 2000;
    vector<string> rownum(MAX_LEN);
    vector<double> pclass(MAX_LEN);
    vector<double> survived(MAX_LEN);
    vector<double> sex(MAX_LEN);
    vector<double> age(MAX_LEN);

    // vector<int> pclass(MAX_LEN);
    // vector<int> survived(MAX_LEN);
    // vector<int> sex(MAX_LEN);
    // vector<double> age(MAX_LEN);


    cout << "Opening file titanic_project.csv." << endl;
    // opening data file
    inFS.open("titanic_project.csv");
    if (!inFS.is_open()){
        cout << "Could not open file titanic_project.csv." << endl;
        return 1;
    }

    // reading in data from csv file
    cout << "Reading line 1" << endl;
    getline(inFS, line);

    cout << "heading: " << line << endl;

    int numObservations = 0;
    while (inFS.good()) {
        getline(inFS, rownum_in, ',');
        getline(inFS, pclass_in, ',');
        getline(inFS, survived_in, ',');
        getline(inFS, sex_in, ',');
        getline(inFS, age_in, '\n');

        rownum.at(numObservations) = rownum_in;
        pclass.at(numObservations) = stof(pclass_in);
        survived.at(numObservations) = stof(survived_in);
        sex.at(numObservations) = stof(sex_in);
        age.at(numObservations) = stof(age_in);

        numObservations++;
    }

    rownum.resize(numObservations);
    pclass.resize(numObservations);
    survived.resize(numObservations);
    sex.resize(numObservations);
    age.resize(numObservations);

    cout << "new length " << pclass.size() << endl;

    cout << "Closing file titanic_project.csv." << endl;
    inFS.close();

    cout << "Number of records: " << numObservations << endl;

    cout << "\nStats for pclass" << endl;
    print_stats(pclass);

    cout << "\nStats for survived" << endl;
    print_stats(survived);

    cout << "\nStats for sex" << endl;
    print_stats(sex);

    cout << "\nStats for age" << endl;
    print_stats(age);

    // cout << "\n Covariance = " << covar(rm, medv) << endl;
    // cout << "\n Correlation = " << cor(rm, medv) << endl;


    // ----------------------------------------------------------------------------
    // Split vector into train and test
    // ----------------------------------------------------------------------------

    // train vectors
    vector<double> pclass_train = vectorSplit(pclass, 0, 800);
    vector<double> survived_train = vectorSplit(survived, 0, 800);
    vector<double> sex_train = vectorSplit(sex, 0, 800);
    vector<double> age_train = vectorSplit(age, 0, 800);

    vector<vector<double>> train = {pclass_train, survived_train, sex_train, age_train};

    cout << "\nStats for pclass_train" << endl;
    print_stats(pclass_train);

    cout << "\nStats for survived_train" << endl;
    print_stats(survived_train);

    cout << "\nStats for sex_train" << endl;
    print_stats(sex_train);

    cout << "\nStats for age_train" << endl;
    print_stats(age_train);


    // test vectors
    vector<double> pclass_test = vectorSplit(pclass, 801, pclass.size() - 1);
    vector<double> survived_test = vectorSplit(survived, 801, survived.size() - 1);
    vector<double> sex_test = vectorSplit(sex, 801, sex.size() - 1);
    vector<double> age_test = vectorSplit(age, 801, age.size() - 1);

    vector<vector<double>> test = {pclass_test, survived_test, sex_test, age_test};

    cout << "\nStats for pclass_test" << endl;
    print_stats(pclass_test);

    cout << "\nStats for survived_test" << endl;
    print_stats(survived_test);

    cout << "\nStats for sex_test" << endl;
    print_stats(sex_test);

    cout << "\nStats for age_test" << endl;
    print_stats(age_test);

    cout << endl << endl;

    vector<double> ap = apriori(survived_train);

    cout << fixed << setprecision(6);
    cout << "Prior probability, survived=no, survived=yes." << endl;
    cout << ap[0] << " " << ap[1] << endl << endl;

    vector<int> count_survived(2);
    count_survived[0] = count(survived_train.begin(), survived_train.end(), 0);
    count_survived[1] = count(survived_train.begin(), survived_train.end(), 1);

    double lh_pclass[2][3];
    for (int sv = 0; sv <= 1; sv++)
        for (int pc = 1; pc <= 3; pc++)
            lh_pclass[sv][pc - 1] = (double)nrow(pclass_train, pc, survived_train, sv) / (double)count_survived[sv];

    double lh_sex[2][2];
    for (int sv = 0; sv <= 1; sv++)
        for (int sx = 0; sx <= 1; sx++)
            lh_sex[sv][sx] = (double)nrow(sex_train, sx, survived_train, sv) / (double)count_survived[sv];

    cout << "Likelihood values for p(pclass|survived):" << endl;
    
    for (int i = 0; i < 2; i++)
    {
        for (int j = 0; j < 3; j++)
            cout << lh_pclass[i][j] << " ";
        cout << endl;
    }

    cout << endl << "Likelihood values for p(sex|survived):" << endl;

    for (int i = 0; i < 2; i++)
    {
        for (int j = 0; j < 2; j++)
            cout << lh_sex[i][j] << " ";
        cout << endl;
    }

    vector<double> age_mean (2);
    vector<double> age_var (2);

    for (int sv = 0; sv <= 1; sv++)
    {
        age_mean[sv] = mean_condition(age_train, survived_train, sv);
        age_var[sv]  = var_condition(age_train, survived_train, sv);
    }

    cout << endl;
    cout << "Applied to first 5 test observations:" << endl;

    auto start = chrono::steady_clock::now();
 
    vector<double> raw(2);
    for (int i = 0; i < 5; i++)
    {
        raw = calc_raw_prob(test[0][i], test[2][i], test[3][i], lh_pclass, lh_sex, ap, age_mean, age_var);
        cout << raw[0] << " " << raw[1] << endl;
    }

    auto end = chrono::steady_clock::now();

    cout << "Elapsed time in milliseconds: "
        << chrono::duration_cast<chrono::milliseconds>(end - start).count()
        << " ms" << endl;

    cout << "\nProgram terminated.";

    return 0;
}



// ----------------------------------------------------------------------------
//  new functions
// ----------------------------------------------------------------------------

vector<double> calc_raw_prob(
    int pclass, int sex, double age, double lh_pclass[2][3], double lh_sex[2][2],
    vector<double> ap, vector<double> age_mean, vector<double> age_var)
{
    double num_s = lh_pclass[1][pclass - 1] * lh_sex[1][sex - 1] * ap[1] *
        calc_age_lh(age, age_mean[1], age_var[1]);
    double num_p = lh_pclass[0][pclass - 1] * lh_sex[0][sex - 1] * ap[0] *
        calc_age_lh(age, age_mean[0], age_var[0]);
    
    double denominator = lh_pclass[1][pclass - 1] * lh_sex[1][sex - 1] *
        calc_age_lh(age, age_mean[1], age_var[1]) * ap[1] +
        lh_pclass[0][pclass - 1] * lh_sex[0][sex - 1] *
        calc_age_lh(age, age_mean[0], age_var[0]) * ap[0];
    
    vector<double> result(2);
    result[0] = num_s / denominator;
    result[1] = num_p / denominator;
    return result;
}

double calc_age_lh(double v, double mean_v, double var_v)
{
    return (1 / sqrt(2 * M_PI * var_v) * exp(-(pow(v - mean_v, 2)) / (2 * var_v)));
}

double mean_condition(vector<double> a, vector<double> c, int condition)
{
    double sum = 0;
    int n = a.size();
    for (int i = 0; i < n; i++)
        if (c[i] == condition)
            sum += a[i];
    
    return sum / (double)n;
}

double var_condition(vector<double> a, vector<double> c, int condition)
{
    double mean = mean_condition(a, c, condition);
    double var = 0;
    int n = a.size();

    for (int i = 0; i < n; i++)
    {
        var += (a[i] - mean) * (a[i] - mean);
    }

    return var / (double)(n - 1);
}

vector<double> apriori(vector<double> a)
{
    vector<double> ap(2);
    ap[0] = count(a.begin(), a.end(), 0) / (double)a.size();
    ap[1] = count(a.begin(), a.end(), 1) / (double)a.size();

    return ap;
    // cout << fixed << setprecision(6);
    // cout << "Prior probability, " << header << "=no, " << header << "=yes." << endl;
    // cout << count(a.begin(), a.end(), 0) / (double)a.size() << '\t';
    // cout << count(a.begin(), a.end(), 1) / (double)a.size() << endl << endl;
}

int nrow(vector<double> a, double target_a, vector<double> b, double target_b)
{
    int n = a.size();
    int count = 0;
    for (int i = 0; i < n; i++)
        if (a[i] == target_a && b[i] == target_b)
            count++;
    return count;
}

vector<double> vectorSplit(vector<double> a, int l, int r)
{
    int n = a.size();
    if (r > n)
    {
        cout << "Vector doesn't have enough rows for this split" << endl;
        return a;
    }

    vector<double> x;
    x.resize(r - l);
    int j = 0;
    for (int i = l; i <= r; i++)
    {
        x[j] = a[i];
        j++;
    }

    return x;
}


// ----------------------------------------------------------------------------
// old functions
// ----------------------------------------------------------------------------

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


void print_stats(vector<double> a)
{
    cout << "Sum: " << vectorSum(a) << endl;
    cout << "Mean: " << vectorMean(a) << endl;
    cout << "Median: " << vectorMedian(a) << endl;
    cout << "Range: " << vectorRange(a) << endl;
    // print stats of vector
}