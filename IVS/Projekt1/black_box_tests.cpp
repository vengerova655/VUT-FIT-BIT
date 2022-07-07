//======== Copyright (c) 2017, FIT VUT Brno, All rights reserved. ============//
//
// Purpose:     Red-Black Tree - public interface tests
//
// $NoKeywords: $ivs_project_1 $black_box_tests.cpp
// $Author:     VERONIKA VENGEROVA <xvenge01@stud.fit.vutbr.cz>
// $Date:       $2017-01-04
//============================================================================//
/**
 * @file black_box_tests.cpp
 * @author VERONIKA VENGEROVA
 *
 * @brief Implementace testu binarniho stromu.
 */

#include <vector>

#include "gtest/gtest.h"

#include "red_black_tree.h"

//============================================================================//
// ** ZDE DOPLNTE TESTY **
//
// Zde doplnte testy Red-Black Tree, testujte nasledujici:
// 1. Verejne rozhrani stromu
//    - InsertNode/DeleteNode a FindNode
//    - Chovani techto metod testuje pro prazdny i neprazdny strom.
// 2. Axiomy (tedy vzdy platne vlastnosti) Red-Black Tree:
//    - Vsechny listove uzly stromu jsou *VZDY* cerne.
//    - Kazdy cerveny uzel muze mit *POUZE* cerne potomky.
//    - Vsechny cesty od kazdeho listoveho uzlu ke koreni stromu obsahuji
//      *STEJNY* pocet cernych uzlu.
//============================================================================//

TEST(EmptyTree, InsertNode) {
    BinaryTree binTree;

    EXPECT_EQ(binTree.GetRoot(), nullptr);

    int Value = 7;
    auto bnode = binTree.InsertNode(Value);
    auto pNextNode = bnode.second;
    EXPECT_TRUE(bnode.first);

    auto *root = binTree.GetRoot();
    EXPECT_EQ(root->pParent, nullptr);
    EXPECT_EQ(pNextNode->pParent, nullptr);
    EXPECT_EQ(pNextNode->key, Value);

    EXPECT_EQ(pNextNode->pLeft->pLeft, nullptr);
    EXPECT_EQ(pNextNode->pLeft->pRight, nullptr);
    EXPECT_EQ(pNextNode->pLeft->color, BLACK);

    EXPECT_EQ(pNextNode->pRight->pLeft, nullptr);
    EXPECT_EQ(pNextNode->pRight->pRight, nullptr);
    EXPECT_EQ(pNextNode->pRight->color, BLACK);

    EXPECT_EQ(pNextNode->color,BLACK);
    EXPECT_NE(binTree.FindNode(Value),nullptr);
}


TEST(EmptyTree, InsertNodeNeg) {
    BinaryTree binTree;

    EXPECT_EQ(binTree.GetRoot(), nullptr);

    int Value = -7;
    auto bnode = binTree.InsertNode(Value);
    auto pNextNode = bnode.second;
    EXPECT_TRUE(bnode.first);

    auto *root = binTree.GetRoot();
    EXPECT_EQ(root->pParent, nullptr);
    EXPECT_EQ(pNextNode->pParent, nullptr);
    EXPECT_EQ(pNextNode->key, Value);

    EXPECT_EQ(pNextNode->pLeft->pLeft, nullptr);
    EXPECT_EQ(pNextNode->pLeft->pRight, nullptr);
    EXPECT_EQ(pNextNode->pLeft->color, BLACK);

    EXPECT_EQ(pNextNode->pRight->pLeft, nullptr);
    EXPECT_EQ(pNextNode->pRight->pRight, nullptr);
    EXPECT_EQ(pNextNode->pRight->color, BLACK);

    EXPECT_EQ(pNextNode->color,BLACK);
    EXPECT_NE(binTree.FindNode(Value),nullptr);
}


TEST(EmptyTree, InsertNodeZero) {
    BinaryTree binTree;

    EXPECT_EQ(binTree.GetRoot(), nullptr);

    int Value = 0;
    auto bnode = binTree.InsertNode(Value);
    auto pNextNode = bnode.second;
    EXPECT_TRUE(bnode.first);

    auto *root = binTree.GetRoot();
    EXPECT_EQ(root->pParent, nullptr);
    EXPECT_EQ(pNextNode->pParent, nullptr);
    EXPECT_EQ(pNextNode->key, Value);

    EXPECT_EQ(pNextNode->pLeft->pLeft, nullptr);
    EXPECT_EQ(pNextNode->pLeft->pRight, nullptr);
    EXPECT_EQ(pNextNode->pLeft->color, BLACK);

    EXPECT_EQ(pNextNode->pRight->pLeft, nullptr);
    EXPECT_EQ(pNextNode->pRight->pRight, nullptr);
    EXPECT_EQ(pNextNode->pRight->color, BLACK);

    EXPECT_EQ(pNextNode->color,BLACK);
    EXPECT_NE(binTree.FindNode(Value),nullptr);
}

TEST(EmptyTree, DeleteNode) {
    BinaryTree binTree;
    int Value = 7;
    EXPECT_FALSE(binTree.DeleteNode(Value));
    EXPECT_EQ(binTree.FindNode(Value),nullptr);

    EXPECT_TRUE(binTree.InsertNode(Value).first);
    EXPECT_TRUE(binTree.DeleteNode(Value));
    EXPECT_EQ(binTree.FindNode(Value),nullptr);

}

TEST(EmptyTree,FindNode) {
    BinaryTree binTree;
    int Value = 13;
    EXPECT_TRUE(binTree.InsertNode(Value).first);
    EXPECT_NE(binTree.FindNode(Value),nullptr);


    EXPECT_TRUE(binTree.InsertNode(2).first);
    EXPECT_TRUE(binTree.InsertNode(3).first);
    EXPECT_TRUE(binTree.InsertNode(4).first);
    Value = 10;
    EXPECT_EQ(binTree.FindNode(Value),nullptr);
}




//-------------------------------------------------------------------------------------
class NonEmptyTree : public ::testing::Test {
protected:
    BinaryTree binTree;
    void SetUp() override {
        EXPECT_TRUE(binTree.InsertNode(1).first);
        EXPECT_TRUE(binTree.InsertNode(2).first);
        EXPECT_TRUE(binTree.InsertNode(3).first);
    }

};

TEST_F(NonEmptyTree, InsertNode) {
    //BinaryTree binTree;


    int Value = 7;
    auto bnode = binTree.InsertNode(Value);
    auto pNextNode = bnode.second;

    EXPECT_TRUE(bnode.first);

    EXPECT_EQ(pNextNode->key, Value);
    EXPECT_TRUE(pNextNode->pLeft->key <= pNextNode->pLeft->key);

    EXPECT_NE(binTree.FindNode(Value),nullptr);


    EXPECT_FALSE(binTree.InsertNode(2).first);

}

TEST_F(NonEmptyTree, DeleteNode) {
BinaryTree BinTree;
int Value = 7;
EXPECT_FALSE(BinTree.DeleteNode(Value));
EXPECT_EQ(BinTree.FindNode(Value),nullptr);

EXPECT_TRUE(BinTree.InsertNode(Value).first);
EXPECT_TRUE(BinTree.DeleteNode(Value));
EXPECT_EQ(BinTree.FindNode(Value),nullptr);

}
TEST_F(NonEmptyTree,FindNode) {
BinaryTree BinTree;
int Value = 13;
EXPECT_TRUE(BinTree.InsertNode(Value).first);
EXPECT_NE(BinTree.FindNode(Value),nullptr);


EXPECT_TRUE(BinTree.InsertNode(2).first);
EXPECT_TRUE(BinTree.InsertNode(3).first);
EXPECT_TRUE(BinTree.InsertNode(4).first);
Value = 10;
EXPECT_EQ(BinTree.FindNode(Value),nullptr);
}

//-------------------------------------------------------------------------------------
/*
TEST(TreeAxioms, Axiom1) {
    std::vector<BinaryTree::Node_t*> &outLeafNodes)
}
 */
/*
TEST(TreeAxioms, Axiom2) {

}


TEST(TreeAxioms, Axiom3) {

}
 */
/*** Konec souboru black_box_tests.cpp ***/