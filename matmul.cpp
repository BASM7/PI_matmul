#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <iomanip>
#include <conio.h>

void quickMatrixMul(float* matrixNM, float* matrixMP, float* matrixNP, unsigned int n, unsigned int m);

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

void printMatrix(float* matrix, unsigned int rows, unsigned int cols){
    for (int i = 0; i < rows; i++){
        for (int j = 0; j < cols; j++){
            std::cout << std::setw(5) << matrix[i*cols+j] << " ";          
        }
        std::cout << std::endl;
    }
}

void transpose(float* matrix, unsigned int &rows, unsigned int &cols){
    for (int i = 0; i < rows; i++){
        for (int j = i + 1; j < cols; j++){
            float temp = matrix[i*cols+j];
            matrix[i*cols+j] = matrix[j*cols+i];
            matrix[j*cols+i] = temp;
        }
    }
    int temp = rows;
    rows = cols;
    cols = temp;
}

extern "C" int _test(int x,int y,int z, int  a,int b, int c);

extern "C" float _quickMatrixMul(float* matrix1, float* matrix2, float* matrix_output, unsigned int n, unsigned int m, unsigned int p);
extern "C" float _copy_row(float* matrix1, float* matrix_output);

int main(){
    //const char* matrix2_file = "Casos_prueba/test.txt";

    // const char* matrix1_file = "Casos_prueba/case0_matrix1.txt";
    // const char* matrix2_file = "Casos_prueba/case0_matrix2.txt";
    // const char* output_file = "Casos_prueba/case0_output.txt";

    //const char* matrix1_file = "Casos_prueba/case1_matrix1.txt";
    // const char* matrix2_file = "Casos_prueba/case1_matrix2.txt";
    // const char* output_file = "Casos_prueba/case1_output.txt";

    const char* matrix1_file = "Casos_prueba/case2_matrix1.txt";
    const char* matrix2_file = "Casos_prueba/case2_matrix2.txt";
    const char* output_file = "Casos_prueba/case2_output.txt";

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

    matrixO_result = new float[16];

    matrix1 = loadMatrix(matrix1_file, rows_matrix1, cols_matrix1);
    matrix2 = loadMatrix(matrix2_file, rows_matrix2, cols_matrix2);
    matrixO_expected = loadMatrix(output_file, rows_matrixO, cols_matrixO);

    std::cout << "Se cargaron las matrices :" << std::endl;
    printMatrix(matrix1, rows_matrix1, cols_matrix1);
    std::cout << std::endl;
    printMatrix(matrix2, rows_matrix2, cols_matrix2);
    std::cout << std::endl;

    //std::cout << compare(matrix1, matrix1, rows_matrix1*cols_matrix1) << std::endl;

    //float* matrix_test = new float[16];
    
    //std::cout << _test(1, 2, 3, 4, 5, 6) << std::endl;

    transpose(matrix2, rows_matrix2, cols_matrix2);
    _quickMatrixMul(matrix1, matrix2, matrixO_result, rows_matrix1*4, 4*4, cols_matrixO*4);


    // //std::cout << _quickMatrixMul(matrix1, matrix2, matrixO_result) << std::endl;
    printMatrix(matrixO_result, rows_matrixO, cols_matrixO);
    //std::cout << "Transpose: " << std::endl;
    //transpose(matrix1, rows_matrix1, cols_matrix1);
    //printMatrix(matrix1, cols_matrix1, rows_matrix1);
    //_copy_row(matrix1, matrix_test);
    //printMatrix_test(matrix_test, 4, 4);

    //_getch();

    return 0;
}

// Compilation.
// nasm -g -f elf64 -o matmul_n.o matmul.asm && g++ -c -g -no-pie -o matmul_c.o matmul.cpp && g++ -g -no-pie -o matmul matmul_n.o matmul_c.o