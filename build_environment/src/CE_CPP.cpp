//============================================================================
// Name        : CE_CPP_2D.cpp
// Author      :
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <fstream>
#include <iostream>
#include <chrono>
#include <pthread.h>
#include <omp.h>
#include <vector>
#include <algorithm>
#include <iterator>
#include <math.h>
//#include <stdio.h>

using namespace std;
using namespace std::chrono;

struct CPURuntimeContext;
struct CPURuntimeContextCG;

extern "C" CPURuntimeContext* init_cpu_ctx();
extern "C" void destroy_cpu_ctx(CPURuntimeContext* cpu_ctx);
extern "C" CPURuntimeContextCG* init_cg_ctx();
extern "C" void destroy_cg_ctx(CPURuntimeContextCG* cg_ctx);
extern "C" void Function_0(void** inputs, void** outputs, CPURuntimeContext* ctx, CPURuntimeContextCG* cg_ctx);
extern "C" void load_params();

int main(int argc, char* argv[]) {

    omp_set_dynamic(0);
    omp_set_num_threads(1);
    cout << "starting new inference session" << endl;
    cout << "*****************************" << endl;
    cout << "using this many cores: " << omp_get_max_threads() << endl << endl; //

	CPURuntimeContext* ctx = init_cpu_ctx();
	CPURuntimeContextCG* cg_ctx = init_cg_ctx();
	int total_number_of_input_samples = 1; // multiplied below to accoutn for real and imaginary
    int num_inference_runs = 100000;
	float* input = new float[(total_number_of_input_samples*2)*1320*14]; // alloc mem for one 2D sample
//	float* input = new float[80*2*1320*14]; // alloc mem for whole 2D dataset
	float* output = new float[2*1320*14];

	vector<float> inference_runs;
	vector<float> deltas;
	vector<float> squared_deltas;
	float sum_sq_deltas;
	float variance_final;
	std:ifstream inputs_file;

    if (argc != 3){
        cout << "argc should be 3 and it is not 3!" << " expected 3 arguments!"<< endl;
        cout << "argv[0] is running the exec (./CE_CPP)" << endl;
        cout << "argv[1] should be used as input bin path:  " << argv[1] << endl;
        cout << "argv[2] path to save files: " << argv[2] << endl << endl;
        }
    else{
        cout << "got all 3 required cmd line args" << endl;
        };

	std::string inputs_file_name = argv[1];
	std::string base_filename = inputs_file_name.substr(inputs_file_name.find_last_of("/\\")+1);
	std::string::size_type const p(base_filename.find_last_of('.'));
    std::string file_without_extension = base_filename.substr(0, p);
	inputs_file.open(inputs_file_name, ios::in|ios::binary); // open file
	if (inputs_file.fail())
	{cout << "Failed to load at file: " << __FILE__ << " line: " << __LINE__ << endl << endl;
	    exit(0);}
	load_params();

	auto total_dur = 0;
	char* ptr = (char*)input; // tell ptr to start from the char*th position of input
//	inputs_file.read(ptr,1182720); // fill memory from file: from position ptr for the size of one sample=1182720bytes
    inputs_file.read(ptr,147840);
	float avg_infer;
	// run once to get over lengthy loading of network - which we do not want to time
	Function_0((void**)&ptr, (void**)&output, ctx, cg_ctx);

	cout << "running inference for " << total_number_of_input_samples << " samples "<< endl;
	for(int i = 0; i<num_inference_runs ;i++)
	{
		inference_runs.clear();
		deltas.clear();
		squared_deltas.clear();
		sum_sq_deltas =0;
		variance_final = 0;
		total_dur = 0;
		avg_infer = 0;

        auto start = high_resolution_clock::now(); // start time
        Function_0((void**)&ptr, (void**)&output, ctx, cg_ctx); // inferenece
        auto stop = high_resolution_clock::now(); // stop time
        auto duration = duration_cast<microseconds>(stop - start);

        inference_runs.push_back(float(duration.count()));
        ptr = ptr + 147840; // move pointer in mem to get next sample

        cout << "current run (microseconds)" << duration.count() << endl;
//				cout << " " << endl;

//			}
//		}
	}

    std::string string_two = "_out_from_cpp.dat";
	std::ofstream res_file;
	std::string save_dir(argv[2]); // This is how you put argv input into string var
	std::string output_file_name_gal = save_dir + "/" + file_without_extension + string_two ;
    cout << "saving output to: " << output_file_name_gal << endl;
	std::string res_file_name = output_file_name_gal;
	res_file.open(res_file_name, std::ios::out|std::ios::binary);
	res_file.write((char*)output, 147840);
	cout << " " << endl << endl;
	res_file.close();
	inputs_file.close();
	return 0;
}
