/*
* Use the following to link and compile. Requires nasm compiler in the enviromental variables.
* And MinGW 64bits compiler.
* nasm -g -f elf64 -o matmul_n.o matmul.asm && g++ -c -g -no-pie -o matmul_c.o matmul.cpp && g++ -g -no-pie -o matmul matmul_n.o matmul_c.o
*
* TP2 Proyecto Integrador de Lenguaje Ensamblador.
* author B93986 Luis Alfonso Jiménez.
* author B95346 Jesús Alonso Moreno Montero.
* author B95092 Víctor Jesús Mora Abarca.
*/


#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <iomanip>
#include <conio.h>
#include <cmath>

float* loadMatrix(const char* filename, unsigned int &rows, unsigned int &cols ){
    std::ifstream matrix_file;
    matrix_file.open(filename);
    std::string data;   

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
    
    float* new_matrix;

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

bool compare(float* matrixA, float* matrixB, unsigned int rows, unsigned int cols){
    for (int i = 0; i < rows; ++i){
        for (int j = 0; j < cols; ++j){
            float num1 = matrixA[i*cols+j];
            float num2 = matrixB[i*cols+j];
            if (num1 != num2 && std::fabs(num1 - num2) < 0.001f ){
                return false;                
            }
        }
    }
    return true;
}

void printMatrix(float* matrix, unsigned int rows, unsigned int cols){
    for (int i = 0; i < rows; i++){
        for (int j = 0; j < cols; j++){
            std::cout << std::setw(8) << matrix[i*cols+j] << " ";          
        }
        std::cout << std::endl;
    }
}

float* transpose(float* matrix, unsigned int &rows, unsigned int &cols){
    float* matrixReturn = new float[cols*rows];
    for (int i = 0; i < rows; i++){
        for (int j = 0; j < cols; j++){
            matrixReturn[j*rows+i]= matrix[i*cols+j];
        }
    }
    int temp = rows;
    rows = cols;
    cols = temp;
    return matrixReturn;
}

extern "C" float _quickMatrixMul(float* matrix1, float* matrix2, float* matrix_output, unsigned int n, unsigned int m, unsigned int p);

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
    float* matrixO_expected;
    float* matrixO_result;
    
    unsigned int rows_matrix1;
    unsigned int cols_matrix1;

    unsigned int rows_matrix2;
    unsigned int cols_matrix2;

    unsigned int rows_matrixO;
    unsigned int cols_matrixO;

    matrix1 = loadMatrix(matrix1_file, rows_matrix1, cols_matrix1);
    matrix2 = loadMatrix(matrix2_file, rows_matrix2, cols_matrix2);
    matrixO_expected = loadMatrix(output_file, rows_matrixO, cols_matrixO);

    matrixO_result = new float[rows_matrixO*cols_matrixO];

    std::cout << "Loaded matrices :" << std::endl;
    printMatrix(matrix1, rows_matrix1, cols_matrix1);
    std::cout << std::endl;
    printMatrix(matrix2, rows_matrix2, cols_matrix2);
    std::cout << std::endl;

    matrix2 = transpose(matrix2, rows_matrix2, cols_matrix2);
    std::cout << std::endl;

    _quickMatrixMul(matrix1, matrix2, matrixO_result, rows_matrix1, cols_matrix1, cols_matrixO);

    std::cout << "Resulted matrix:" <<std::endl;
    std::cout << std::endl;
    printMatrix(matrixO_result, rows_matrixO, cols_matrixO);
    std::cout << std::endl;
    std::cout << "Expected matrix:" <<std::endl;
    printMatrix(matrixO_expected, rows_matrixO, cols_matrixO);
    std::cout << std::endl;

    std::cout << std::endl;
    if (compare(matrixO_result, matrixO_expected, rows_matrixO, cols_matrixO))
        std::cout << "TEST PASSED" << std::endl;
    else
        std::cout << "TEST FAILED" <<std::endl;

    matrix2 = transpose(matrix2, rows_matrix2, cols_matrix2);
    delete[] matrix1;
    delete[] matrix2;
    delete[] matrixO_expected;
    delete[] matrixO_result;
    return 0;
}

// Compilation and linking.
// nasm -g -f elf64 -o matmul_n.o matmul.asm && g++ -c -g -no-pie -o matmul_c.o matmul.cpp && g++ -g -no-pie -o matmul matmul_n.o matmul_c.o