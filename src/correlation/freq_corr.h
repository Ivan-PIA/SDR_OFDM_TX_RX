#include <iostream>
#include <vector>
#include <complex>
#include <cmath>
#include <iomanip>

using namespace std;

typedef complex<double> complex_t;

#define M_PI 3.14159265358979323846

template <typename T>

size_t argmax(vector<T>& v) {
    size_t max_index = 0;
    T max_value = v[0];
    for (size_t i = 1; i < v.size(); i++) {
        if (v[i] > max_value) {
            max_index = i;
            max_value = v[i];
        }
    }
    return max_index;
}


void frequency_correlation(const vector<complex_t>& pss, const vector<complex_t>& matrix_name, int m, vector<complex_t>& data_offset) {
    // Calculate the flipped and complex conjugated reference signal
    vector<complex_t> corr_coef(pss.rbegin(), pss.rend());
    for (size_t i = 0; i < corr_coef.size(); i++) {
        corr_coef[i] = conj(corr_coef[i]);
    }

    vector<complex_t> partA(pss.size() + matrix_name.size() - 1, 0);
    for (size_t i = 0; i < pss.size() / 2; i++) {
        for (size_t j = 0; j < matrix_name.size(); j++) {
            partA[i + j] += corr_coef[i] * matrix_name[j];
        }
    }

    vector<complex_t> xDelayed(matrix_name.size());
    for (size_t i = 0; i < matrix_name.size() ; i++) {
        
        if(i < pss.size() / 2){
            xDelayed[i] = 0;
        }
        else{
            xDelayed[i] = matrix_name[i];
        }
    }

    
    vector<complex_t> partB(pss.size() + matrix_name.size() - 1, 0);
    for (size_t i = pss.size() / 2; i < pss.size(); i++) {
        for (size_t j = 0; j < xDelayed.size(); j++) {
            partB[i + j] += corr_coef[i] * xDelayed[j];
        }
    }

    // Calculate correlation and phase difference
    vector<double> correlation(pss.size() + matrix_name.size() - 1, 0);
    for (size_t i = 0; i < correlation.size(); i++) {
        correlation[i] = abs(partA[i] + partB[i]);
    }
    vector<complex_t> phaseDiff(pss.size() + matrix_name.size() - 1, 0);
    for (size_t i = 0; i < phaseDiff.size(); i++) {
        phaseDiff[i] = partA[i] * conj(partB[i]);
    }


    size_t istart = argmax(correlation) ;
    complex_t phaseDiff_max = phaseDiff[istart];


    double CFO = arg(phaseDiff_max) / (M_PI * 1 / m);
    vector<double> t(matrix_name.size());
    for (size_t i = 0; i < t.size(); i++) {
        t[i] = i ;
    }

    //cout << "CFO :" << conj(CFO) << endl;
    //CFO = -7018.845944622955;
    // Offset the data
    data_offset.resize(matrix_name.size());
    for (size_t i = 0; i < matrix_name.size(); i++) {
        //cout  <<  exp(-1i * double(2) * M_PI * conj(CFO) * double(t[i])) << endl;
        data_offset[i] = matrix_name[i] * exp(-1i * double(2) * M_PI * conj(CFO) * double(t[i]/1920000));
    }
    
}