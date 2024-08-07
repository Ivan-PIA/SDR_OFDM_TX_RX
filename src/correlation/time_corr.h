#include <iostream>
#include <vector>
#include <complex> 
#include <iterator>
#include <fstream>
#include <algorithm>
#include <numeric>
#include <cmath>
#include <span>
#include <chrono>

using namespace std;


// Свертка двух массивов, возвращает нормированный массив 
vector<double> convolve(vector<complex<double>>x, vector<complex<double>>h) {
    
    int n = x.size() + h.size() - 1;
    //cout << h.size() << " " << x.size() << endl;
    vector<complex<double>> y(n);

    x.insert(x.begin(), h.size()-1, complex<double>(0, 0));
        
    // cout << y.size() << endl;
    // cout << x.size() << endl;
    //complex <double> norm_x;
    //complex <double> norm_h;
    double norm_x;
    double norm_h;

    for(int i = 0; i < x.size(); i ++){
        norm_x += abs(x[i]) * abs(x[i]);
    }
    for(int i = 0; i < h.size(); i ++){
        norm_h += abs(h[i]) * abs(h[i]);
    }
    
    vector <double> norm_y;

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < h.size(); j++) {       
            y[i] += x[i+j] * h[j];
        }
        norm_y.push_back((abs(y[i]) / (sqrt(norm_h) * sqrt(norm_x)))*10);
        
    }
    // vector <double> abs_y(n);
    // int count = 0;
    // for (int i = 0; i < n; i++){
    //     abs_y[i] = abs(y[i]);
    //     if (abs_y[i]> 7000){
    //         count++;
    //     }
    // }
    //cout << "conv1 = " << count << endl;


    return norm_y;
}


double norm_corr(vector<complex<double>> x, vector<complex<double>> h){
    

    double norm_x;
    double norm_h;

    for(int i = 0; i < x.size(); i ++){
        norm_x += abs(x[i]) * abs(x[i]);
    }
    for(int i = 0; i < h.size(); i ++){
        norm_h += abs(h[i]) * abs(h[i]);
    }

    double correlate;
    complex<double> t;
    
    for (int i = 0 ; i < x.size(); i++){
        t = x[i] * h[i];
        correlate += abs(t)/ (sqrt(norm_h) * sqrt(norm_x));
    }
    return correlate;
}



vector<int> symbol_sync(const std::vector<complex<double>> rx, int cp, int fft_len) {
    std::vector<double> corr; // Массив корреляции 
    std::vector<complex<double>> rx_copy = rx; // Копируем входной массив

    for (size_t i = 0; i < rx.size() - fft_len - cp; i++) {
        //auto start = std::chrono::high_resolution_clock::now();
        double o = norm_corr(std::vector<complex<double>>(rx_copy.begin()+ i , rx_copy.begin() + cp + i), 
                             std::vector<complex<double>>(rx_copy.begin() + fft_len + i , rx_copy.begin() + fft_len + cp + i));
    

        corr.push_back(o);
        
        // auto end = std::chrono::high_resolution_clock::now();
        // auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
        // cout << duration.count() << endl;
    }


    // Нормирование
    double max_corr = *std::max_element(corr.begin(), corr.end());
    for (auto& value : corr) {
        value /= max_corr;
    }

    for(double ind1 : corr ){
		// cout << ind1 << endl;
	}

    
    size_t max_len_cycle = corr.size();
    
    size_t ind = std::distance(corr.begin(), std::max_element(corr.begin(), corr.begin() + (fft_len + cp) / 2));
    
    

    std::vector<int> arr_index; 
    arr_index.push_back(ind);
    
    for (size_t i = (fft_len + cp)/2 ; i < max_len_cycle; i += (fft_len + cp)) {
        double max = *std::max_element(corr.begin() + i, corr.begin() + i + (fft_len + cp));
        //cout << max << endl;
        if (max > 0.90) {
            ind = std::distance(corr.begin(), std::max_element(corr.begin() + i, corr.begin() + i + (fft_len + cp)));
            //cout << ind<< " " << std::distance(corr.begin(), std::max_element(corr.begin() + i, corr.begin() + i + (fft_len + cp))) << endl;
            if (ind < corr.size()) {
                arr_index.push_back(ind);
            }
        }
    }

    return arr_index;
}
