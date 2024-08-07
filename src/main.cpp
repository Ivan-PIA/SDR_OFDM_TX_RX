#include <iostream>
#include <fstream>
#include <complex>
#include <vector>
using namespace std;

// #include </home/ivan/Desktop/Work_dir/1440/SDR_OFDM_TX_RX/src/settings_sdr/settinds_sdr.h> 
// #include </home/ivan/Desktop/Work_dir/1440/SDR_OFDM_TX_RX/src/fft/fft.h> 
//g++ main.cpp -o main -liio
#include </home/ivan/Desktop/Work_dir/1440/SDR_OFDM_TX_RX/src/correlation/freq_corr.h>

#include </home/ivan/Desktop/Work_dir/1440/SDR_OFDM_TX_RX/src/correlation/time_corr.h>
int main(){


	ifstream real_file1("../file_debag/resurs_file/data_after_time_corr/cor_real.txt");
	if (!real_file1.is_open()) {
		std::cerr << "Не удалось открыть файл12" << std::endl;
		return 1;
	}
	// файл с мнимой частью
	ifstream imag_file1("../file_debag/resurs_file/data_after_time_corr/cor_imag.txt");
	if (!imag_file1.is_open()) {
		std::cerr << "Не удалось открыть файл1" << std::endl;
		return 1;
	}

		ifstream real_file_pss("../file_debag/resurs_file/pss/pss_real.txt");
	if (!real_file1.is_open()) {
		std::cerr << "Не удалось открыть файл12" << std::endl;
		return 1;
	}
	// файл с мнимой частью
	ifstream imag_file_pss("../file_debag/resurs_file/pss/pss_imag.txt");
	if (!imag_file1.is_open()) {
		std::cerr << "Не удалось открыть файл1" << std::endl;
		return 1;
	}


	vector <complex<double>> rx_data_time_corr;
	vector <complex<double>> pss;
	double real_num, imag_num;

	while (real_file_pss >> real_num && imag_file_pss >> imag_num) {
		pss.push_back(complex<double>(real_num, imag_num));
        //cout<< real_num << " " << imag_num << endl;
	}

	

	while (real_file1 >> real_num && imag_file1 >> imag_num) {
		rx_data_time_corr.push_back(complex<double>(real_num, imag_num));
        //cout<< real_num << " " << imag_num << endl;
	}
	
	vector <complex_t> data_corr_freq;
    frequency_correlation(pss,rx_data_time_corr, 15000, data_corr_freq);
	
	double com;
	for(complex<double> com : data_corr_freq ){
		//cout << com << endl;
	}

	vector <int> sym_s;
	sym_s = symbol_sync(data_corr_freq, 32, 128);

	for(int ind1 : sym_s ){
		cout << ind1 << endl;
	}


}