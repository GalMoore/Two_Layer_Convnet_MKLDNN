1. Set build_environment/Release/makefile to your local version of DNNL 
2. Set paths to weights and biases to your local folders in Function_0
3. compile this project from build_environment/Release with:  
* make clean  
* make all  
  
4. Run the executable with:       
./CE_CPP /path/to/input_tensor.npy /path/to/output_MKL
