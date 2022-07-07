//======== Copyright (c) 2017, FIT VUT Brno, All rights reserved. ============//
//
// Purpose:     White Box - Tests suite
//
// $NoKeywords: $ivs_project_1 $white_box_code.cpp
// $Author:     VERONIKA VENGEROVA <xvenge01@stud.fit.vutbr.cz>
// $Date:       $2017-01-04
//============================================================================//
/**
 * @file white_box_tests.cpp
 * @author VERONIKA VENGEROVA
 * 
 * @brief Implementace testu prace s maticemi.
 */

#include "gtest/gtest.h"
#include "white_box_code.h"

//============================================================================//
// ** ZDE DOPLNTE TESTY **
//
// Zde doplnte testy operaci nad maticemi. Cilem testovani je:
// 1. Dosahnout maximalniho pokryti kodu (white_box_code.cpp) testy.
// 2. Overit spravne chovani operaci nad maticemi v zavislosti na rozmerech 
//    matic.
//============================================================================//
class TestMatrix : public ::testing::Test {
protected:
    Matrix matrix;


};
TEST_F(TestMatrix, Constructor1x1) {
    EXPECT_TRUE(matrix.get(0,0)==0);
    std::vector<std::vector< double > > values = {{11,12,13},{21,22,23},{31,32,33}};
    EXPECT_FALSE(matrix.set(values));
    values = {{1}};
    EXPECT_TRUE(matrix.set(values));

}

TEST_F(TestMatrix, Constructor3x3) {
    auto matrix33 = Matrix(3,3);
    for(int i =0;i<3;i++){
        for(int j = 0;j<3; j++)
        EXPECT_TRUE(matrix33.get(i,j)==0);
    }
    std::vector<std::vector< double > > values = {{11,12,13},{21,22,23},{31,32,33}};
    EXPECT_TRUE(matrix33.set(values));

    values = {{1,2,3},{3,4},{5,6},{1}};
    EXPECT_FALSE(matrix33.set(values));

    values = {{1,2},{3,4}};
    EXPECT_FALSE(matrix33.set(values));
}

TEST_F(TestMatrix, ConstructorZero) {

    EXPECT_ANY_THROW(Matrix(0,1));
    EXPECT_ANY_THROW(Matrix(1,0));
    EXPECT_ANY_THROW(Matrix(0,0));

}

TEST_F(TestMatrix, Constructor3x2) {
    auto matrix32 = Matrix(3,2);
    for(int i =0;i<3;i++){
        for(int j = 0;j<2; j++)
            EXPECT_TRUE(matrix32.get(i,j)==0);
    }
    std::vector<std::vector< double > > values = {{1,2},{3,4},{5,6}};
    EXPECT_TRUE(matrix32.set(values));

    values = {{1,2,3},{3,4},{5,6}};
    EXPECT_FALSE(matrix32.set(values));

    values = {{1,2},{3,4}};
    EXPECT_FALSE(matrix32.set(values));

}


TEST_F(TestMatrix, SetTest3x3){
    //std::vector<std::vector< double > > values = {{1,2},{3,4},{5,6}};
    auto matrix33 = Matrix(3,3);
    EXPECT_TRUE(matrix33.set(1,2,40));
    EXPECT_FALSE(matrix33.set(4,4,1));
    EXPECT_EQ(matrix33.get(1,2), 40);
    EXPECT_ANY_THROW(matrix33.get(4,4));
}


TEST_F(TestMatrix, SetTest0x0){
    //std::vector<std::vector< double > > values = {{1,2},{3,4},{5,6}};
    EXPECT_ANY_THROW(Matrix(0,0));

}
//3x2
TEST_F(TestMatrix, SetTest3x2){
    //std::vector<std::vector< double > > values = {{1,2},{3,4},{5,6}};
    auto matrix32 = Matrix(3,2);
    double value = 4;
    EXPECT_TRUE(matrix32.set(1,1,value));
    EXPECT_FALSE(matrix32.set(4,4,1));
    EXPECT_EQ(matrix32.get(1,1), value);
}

//operand ==
TEST_F(TestMatrix, OperandEq){
    std::vector<std::vector< double > > values = {{1,2,3},{3,4,6},{3,4,6}};
    auto matrix133 = Matrix(3,3);
    matrix133.set(values);
    auto matrix233 = Matrix(3,3);
    matrix233.set(values);

    EXPECT_TRUE(matrix133==matrix233);
    values = {{1,2,3},{3,4,6},{3,4,7}};
    matrix233.set(values);
    EXPECT_FALSE(matrix133==matrix233);
    matrix233 = Matrix(2,3);
    values = {{1,2,3},{3,4,6}};
    matrix233.set(values);
    EXPECT_ANY_THROW(matrix133==matrix233);
}


//operand +
TEST_F(TestMatrix, OperandPlus){
    std::vector<std::vector< double > > values = {{1,2,3},{3,4,6},{3,4,6}};
    auto matrix133 = Matrix(3,3);
    matrix133.set(values);
    auto matrix233 = Matrix(3,2);
    values = {{0,0,0},{0,0,0}};
    matrix233.set(values);
    EXPECT_ANY_THROW(matrix133+matrix233);

    matrix233 = Matrix(3,3);
    values = {{1,2,3},{3,4,6},{3,4,6}};
    matrix233.set(values);
    auto result = Matrix(3,3);
    values = {{2,4,6},{6,8,12},{6,8,12}};
    result.set(values);
    EXPECT_TRUE((matrix133+matrix233)==result);



    values = {{0,0,0},{0,0,0},{0,0,0}};
    matrix233.set(values);
    values = {{1,2,3},{3,4,6},{3,4,6}};
    result.set(values);
    EXPECT_TRUE((matrix133+matrix233)==result);

}


//operand *
TEST_F(TestMatrix, OperandSubt){
    std::vector<std::vector< double > > values = {{1,2,3},{3,4,6},{3,4,6}};
    auto matrix133 = Matrix(3,3);
    matrix133.set(values);
    auto matrix23 = Matrix(2,3);
    values = {{0,0,0},{0,0,0}};
    matrix23.set(values);
    EXPECT_ANY_THROW(matrix133*matrix23);

    auto matrix233 = Matrix(3,3);
    values = {{1,0,0},{0,1,0},{0,0,1}};
    matrix233.set(values);
    auto result = Matrix(3,3);
    values = {{1,2,3},{3,4,6},{3,4,6}};
    result.set(values);
    EXPECT_TRUE((matrix133*matrix233)==result);
    values = {{1,2,4},{3,4,6},{3,4,6}};
    result.set(values);
    EXPECT_FALSE((matrix133*matrix233)==result);

    values = {{1,2,3},{4,5,6}};
    matrix23.set(values);

    auto matrix32 = Matrix(3,2);
    values = {{1,2},{3,4},{5,6}};
    matrix32.set(values);


    result = Matrix(2,2);
    values = {{22,28},{49,64}};
    result.set(values);

    EXPECT_TRUE((matrix23*matrix32)==result);

    result = Matrix(3,3);
    values = {{1,0,0},{0,1,0},{0,0,1}};
    matrix233.set(values);
    values = {{1,2,3},{3,4,6},{3,4,6}};
    result.set(values);
    EXPECT_TRUE((matrix133*matrix233)==result);

}


//operand *
TEST_F(TestMatrix, OperandSubt1){
    std::vector<std::vector< double > > values = {{1,2,3},{3,4,6},{3,4,6}};
    auto matrix133 = Matrix(3,3);
    matrix133.set(values);

    auto result = Matrix(3,3);
    values = {{2,4,6},{6,8,12},{6,8,12}};
    result.set(values);
    EXPECT_TRUE((matrix133*2==result));
}


TEST_F(TestMatrix, SolveEq){
    std::vector<std::vector< double > > values = {{1,2,3},{3,4,6},{3,4,6}};
    auto matrix133 = Matrix(3,3);
    matrix133.set(values);

    auto matrix32 = Matrix(3,2);
    values = {{1,2},{3,4},{1,2}};
    matrix32.set(values);

    std::vector<double> b = {1,2};
    EXPECT_ANY_THROW(matrix32.solveEquation(b));
    b = {1,2,3};

     EXPECT_ANY_THROW(matrix133.solveEquation(b));
    b = {1,2};
    EXPECT_ANY_THROW(matrix133.solveEquation(b));
    b = {1,2,3};
    EXPECT_ANY_THROW(matrix32.solveEquation(b));

    values = {{1,2,3},{3,4,6},{3,4,5}};
    matrix133.set(values);
    b = {1,2,2};
    std::vector<double> res = std::vector<double>(9, 0);
    res = {{0},{0.5},{0}};
    EXPECT_TRUE(matrix133.solveEquation(b) == res);
     }

/*** Konec souboru white_box_tests.cpp ***/
