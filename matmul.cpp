#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <iomanip>

/*
void quickMatrixMul(float* matrixNM, float* matrixMP, float* matrixNP, unsigned n, unsigned m, unsigned p){

}
*/

float* loadMatrix(const char* filename, unsigned int &rows, unsigned int &cols ){
    std::ifstream matrix_file;
    matrix_file.open(filename);
    std::string data;

    float* new_matrix;

    rows, cols = 0;

    // read the row and col line.
    if (getline(matrix_file, data)){
        std::stringstream ss(data);
        while (ss.good()){
            getline(ss, data, ',');
            rows = std::stoi(data);
            getline(ss, data, ',');
            cols = std::stoi(data);
        }
    }       
    
    // read the rest of the matrix.
    if (rows > 0 && cols > 0){
        new_matrix = new float[rows*cols];
        std::string data;
        int index = 0;
        while(getline(matrix_file, data)){
            std::stringstream ss(data);
            while(ss.good()){
                std::string value;
                getline(ss, value, ',');
                // store the vale on the flat matrix.
                new_matrix[index++] = std::stof(value);
            }
        }
    }
    matrix_file.close();
    return new_matrix;
}

bool compare(float* matrixA, float* matrixB, unsigned int elements){
    bool are_equal = true;
    for (int i = 0; i < elements; i++){
        if (matrixA[i] != matrixB[i]){
            are_equal = false;
            break;
        }
    }
    return are_equal;
}

void printMatrix_test(float* matrix, unsigned int rows, unsigned int cols){
    for (int i = 0; i < rows; ++i){
        for (int j = 0; j < cols; ++j){
            std::cout << std::setw(5) << matrix[i*cols+j] << " ";          
        }
        std::cout << std::endl;
    }
}

int main(){

    const char* matrix1_file = "Casos_prueba/case0_matrix1.txt";
    const char* matrix2_file = "Casos_prueba/case0_matrix2.txt";
    const char* output_file = "Casos_prueba/case0_output.txt";

    // const char* matrix1_file = "Casos_prueba/case1_matrix1.txt";
    // const char* matrix2_file = "Casos_prueba/case1_matrix2.txt";
    // const char* output_file = "Casos_prueba/case1_output.txt";

    // const char* matrix1_file = "Casos_prueba/case2_matrix1.txt";
    // const char* matrix2_file = "Casos_prueba/case2_matrix2.txt";
    // const char* output_file = "Casos_prueba/case2_output.txt";

    float* matrix1;
    float* matrix2;
    float* matrixO;

    unsigned int rows_matrix1;
    unsigned int cols_matrix1;
    unsigned int rows_matrix2;
    unsigned int cols_matrix2;
    unsigned int rows_matrixO;
    unsigned int cols_matrixO;

    matrix1 = loadMatrix(matrix1_file, rows_matrix1, cols_matrix1);
    matrix2 = loadMatrix(matrix2_file, rows_matrix2, cols_matrix2);
    matrixO = loadMatrix(output_file, rows_matrixO, cols_matrixO);

    std::cout << "Se cargaron las matrices :" << std::endl;
    printMatrix_test(matrix1, rows_matrix1, cols_matrix1);
    std::cout << std::endl;
    printMatrix_test(matrix2, rows_matrix2, cols_matrix2);
    std::cout << std::endl;

    //std::cout << compare(matrix1, matrix1, rows_matrix1*cols_matrix1) << std::endl; 
}

// Compilation.
// g++ -c -g -no-pie -o matmul_c.o matmul.cpp && g++ -g -no-pie -o matmul matmul_c.o