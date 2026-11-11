#include <gtest/gtest.h>
#include "BST.h"

TEST(BST, EmptyTree) {
    lab::BST<int> tree;
    EXPECT_TRUE(tree.IsEmpty());
    EXPECT_EQ(tree.Size(), 0);
    EXPECT_FALSE(tree.Search(10));
}

TEST(BST, InsertAndSearch) {
    lab::BST<int> tree;
    tree.Insert(50);
    tree.Insert(30);
    tree.Insert(70);
    tree.Insert(20);
    tree.Insert(40);

    EXPECT_EQ(tree.Size(), 5);
    EXPECT_FALSE(tree.IsEmpty());
    EXPECT_TRUE(tree.Search(50));
    EXPECT_TRUE(tree.Search(20));
    EXPECT_TRUE(tree.Search(40));
    EXPECT_FALSE(tree.Search(99));
}

TEST(BST, InsertDuplicates) {
    lab::BST<int> tree;
    tree.Insert(10);
    tree.Insert(10);
    tree.Insert(10);
    EXPECT_EQ(tree.Size(), 1);
}

TEST(BST, RemoveLeaf) {
    lab::BST<int> tree;
    tree.Insert(50);
    tree.Insert(30);
    tree.Insert(70);
    
    tree.Remove(30);
    EXPECT_EQ(tree.Size(), 2);
    EXPECT_FALSE(tree.Search(30));
    EXPECT_TRUE(tree.Search(50));
}

TEST(BST, RemoveNodeWithOneChild) {
    lab::BST<int> tree;
    tree.Insert(50);
    tree.Insert(30);
    tree.Insert(20);
    
    tree.Remove(30);
    EXPECT_EQ(tree.Size(), 2);
    EXPECT_FALSE(tree.Search(30));
    EXPECT_TRUE(tree.Search(20));
    EXPECT_TRUE(tree.Search(50));
}

TEST(BST, RemoveNodeWithTwoChildren) {
    lab::BST<int> tree;
    tree.Insert(50);
    tree.Insert(30);
    tree.Insert(70);
    tree.Insert(60);
    tree.Insert(80);
    
    tree.Remove(70);
    EXPECT_EQ(tree.Size(), 4);
    EXPECT_FALSE(tree.Search(70));
    EXPECT_TRUE(tree.Search(60));
    EXPECT_TRUE(tree.Search(80));
}

TEST(BST, RemoveRoot) {
    lab::BST<int> tree;
    tree.Insert(50);
    tree.Insert(30);
    tree.Insert(70);
    
    tree.Remove(50);
    EXPECT_EQ(tree.Size(), 2);
    EXPECT_FALSE(tree.Search(50));
    EXPECT_TRUE(tree.Search(30));
    EXPECT_TRUE(tree.Search(70));
}

TEST(BST, RemoveNonExistent) {
    lab::BST<int> tree;
    tree.Insert(10);
    tree.Remove(99);
    EXPECT_EQ(tree.Size(), 1);
}

TEST(BST, MoveConstructor) {
    lab::BST<int> tree1;
    tree1.Insert(10);
    tree1.Insert(20);

    lab::BST<int> tree2 = std::move(tree1);
    EXPECT_EQ(tree2.Size(), 2);
    EXPECT_TRUE(tree2.Search(10));
    EXPECT_TRUE(tree1.IsEmpty()); 
}

TEST(BST, Traversals) {
    lab::BST<int> tree;
    tree.Insert(50);
    tree.Insert(30);
    tree.Insert(70);
    tree.Insert(20);
    tree.Insert(40);
    tree.Insert(60);
    tree.Insert(80);

    std::vector<int> expected_pre  = {50, 30, 20, 40, 70, 60, 80};
    std::vector<int> expected_in   = {20, 30, 40, 50, 60, 70, 80};
    std::vector<int> expected_post = {20, 40, 30, 60, 80, 70, 50};
    
    std::vector<int> expected_rev_pre  = {50, 70, 80, 60, 30, 40, 20};
    std::vector<int> expected_rev_in   = {80, 70, 60, 50, 40, 30, 20};
    std::vector<int> expected_rev_post = {80, 60, 70, 40, 20, 30, 50};

    EXPECT_EQ(tree.Traverse(lab::Order::KLP), expected_pre);
    EXPECT_EQ(tree.Traverse(lab::Order::LKP), expected_in);
    EXPECT_EQ(tree.Traverse(lab::Order::LPK), expected_post);
    
    EXPECT_EQ(tree.Traverse(lab::Order::KPL), expected_rev_pre);
    EXPECT_EQ(tree.Traverse(lab::Order::PKL), expected_rev_in);
    EXPECT_EQ(tree.Traverse(lab::Order::PLK), expected_rev_post);
}

TEST(BST, TraversalEmptyTree) {
    lab::BST<int> tree;
    EXPECT_TRUE(tree.Traverse(lab::Order::KLP).empty());
    EXPECT_TRUE(tree.Traverse(lab::Order::LKP).empty());
    EXPECT_TRUE(tree.Traverse(lab::Order::LPK).empty());
}

TEST(BST, MapOperation) {
    lab::BST<int> tree;
    tree.Insert(1);
    tree.Insert(2);
    tree.Insert(3);

    auto mapped = tree.Map([](int x) { return x * 10; });

    EXPECT_EQ(mapped.Size(), 3);
    EXPECT_TRUE(mapped.Search(10));
    EXPECT_TRUE(mapped.Search(20));
    EXPECT_TRUE(mapped.Search(30));
    EXPECT_FALSE(mapped.Search(1));
}

TEST(BST, WhereOperation) {
    lab::BST<int> tree;
    tree.Insert(1);
    tree.Insert(2);
    tree.Insert(3);
    tree.Insert(4);
    tree.Insert(5);

    auto filtered = tree.Where([](int x) { return x % 2 == 0; });

    EXPECT_EQ(filtered.Size(), 2);
    EXPECT_TRUE(filtered.Search(2));
    EXPECT_TRUE(filtered.Search(4));
    EXPECT_FALSE(filtered.Search(1));
}

TEST(BST, ReduceOperation) {
    lab::BST<int> tree;
    tree.Insert(1);
    tree.Insert(2);
    tree.Insert(3);
    tree.Insert(4);

    int sum = tree.Reduce([](int acc, int val) { return acc + val; }, 0);
    EXPECT_EQ(sum, 10);
}

TEST(BST, ReduceOnEmpty) {
    lab::BST<int> tree;
    int sum = tree.Reduce([](int acc, int val) { return acc + val; }, 42);
    EXPECT_EQ(sum, 42);
}

TEST(BST, AVLBalance) {
    lab::BST<int> tree;
    for (int i = 1; i <= 100; ++i) {
        tree.Insert(i);
    }
    
    auto inOrder = tree.Traverse(lab::Order::LKP);
    EXPECT_EQ(inOrder.size(), 100);
    EXPECT_EQ(inOrder.front(), 1);
    EXPECT_EQ(inOrder.back(), 100);
}

TEST(BST, AVLHeight) {
    lab::BST<int> tree;
    for (int i = 1; i <= 100; ++i) {
        tree.Insert(i);
    }
    int h = tree.Height();
    EXPECT_LE(h, 10);
}

TEST(BST, ThreadedInOrder) {
    lab::BST<int> tree;
    tree.Insert(50);
    tree.Insert(30);
    tree.Insert(70);
    tree.Insert(20);
    tree.Insert(40);

    tree.BuildThreads();
    
    auto expected = std::vector<int>{20, 30, 40, 50, 70};
    EXPECT_EQ(tree.ThreadedInOrder(), expected);
}

TEST(BST, SaveAndLoad) {
    lab::BST<int> tree;
    tree.Insert(50);
    tree.Insert(30);
    tree.Insert(70);

    std::string saved = tree.SaveToString("K(L)[R]");
    EXPECT_EQ(saved, "50(30)[70]");

    lab::BST<int> loaded;
    loaded.LoadFromString(saved);

    EXPECT_EQ(loaded.Size(), 3);
    EXPECT_TRUE(loaded.Search(50));
    EXPECT_TRUE(loaded.Search(30));
    EXPECT_TRUE(loaded.Search(70));
}