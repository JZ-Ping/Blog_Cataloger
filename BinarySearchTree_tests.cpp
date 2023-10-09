// Project UID db1f506d06d84ab787baf250c265e24e

#include "BinarySearchTree.h"
#include "unit_test_framework.h"
#include <string>
#include <iostream>

using namespace std;

TEST(test_small_tree) {
    BinarySearchTree<int> int_tree;
    ASSERT_TRUE(int_tree.empty());
    ASSERT_EQUAL(int_tree.height(),0);
    ASSERT_EQUAL(int_tree.size(), 0);
    int_tree.insert(2);
    ASSERT_EQUAL(int_tree.height(),1);
    ASSERT_EQUAL(int_tree.size(), 1);
    ASSERT_TRUE(!int_tree.empty());

    BinarySearchTree<int> int_tree2;
    int_tree2 = int_tree;
    int_tree2.insert(3);
    ASSERT_EQUAL(int_tree.height(),1);
    ASSERT_EQUAL(int_tree.size(), 1);
    ASSERT_TRUE(!int_tree.empty());

    ASSERT_EQUAL(int_tree2.height(),2);
    ASSERT_EQUAL(int_tree2.size(), 2);
    ASSERT_TRUE(!int_tree2.empty());

    BinarySearchTree<int> int_tree3(int_tree2);
    int_tree3.insert(5);
    int_tree3.insert(1);
    ASSERT_EQUAL(int_tree3.height(),3);
    ASSERT_EQUAL(int_tree3.size(), 4);
    ASSERT_TRUE(!int_tree3.empty());   
    ASSERT_EQUAL(int_tree2.height(),2);
    ASSERT_EQUAL(int_tree2.size(), 2);
    ASSERT_TRUE(!int_tree2.empty());
}

TEST(test_big_tree){
    BinarySearchTree<string> str_tree;
    ASSERT_TRUE(str_tree.empty());
    str_tree.insert("gamma");
    str_tree.insert("beta");
    str_tree.insert("phi");
    str_tree.insert("alpha");
    str_tree.insert("chi");
    str_tree.insert("delta");

    ASSERT_TRUE(str_tree.check_sorting_invariant());
    
    ostringstream oss_preorder;
    str_tree.traverse_preorder(oss_preorder);
    //cout<<"preorder"<<endl;
    //cout<<oss_preorder.str()<<endl<<endl;
    ASSERT_EQUAL(oss_preorder.str(), 
        "gamma beta alpha chi delta phi ");
    
    ostringstream oss_inorder;
    str_tree.traverse_inorder(oss_inorder);
    ASSERT_EQUAL(oss_inorder.str(), 
        "alpha beta chi delta gamma phi ");

    ASSERT_EQUAL(str_tree.height(), 4);
    ASSERT_TRUE(!str_tree.empty());
    ASSERT_EQUAL(str_tree.size(), 6);

    BinarySearchTree<string>::Iterator i; 
    i = str_tree.begin();
    ASSERT_EQUAL(*i, "alpha");
    //i++;
    ++i;
    ASSERT_TRUE(i!=str_tree.begin());
    ASSERT_EQUAL(*i, "beta");
    ++i;
    ASSERT_EQUAL(*i, "chi");
    //i++;
    ++i;
    ASSERT_EQUAL(*i, "delta");
    ++i;
    ASSERT_EQUAL(*i, "gamma");
    //i++;
    ++i;
    ASSERT_EQUAL(*i, "phi");
    ++i;
    ASSERT_EQUAL(i, str_tree.end());
}

TEST(test_traversal){
    BinarySearchTree<char> char_tree;
    ASSERT_TRUE(char_tree.empty());
    char_tree.insert('s');
    char_tree.insert('o');
    char_tree.insert('m');
    char_tree.insert('k');
    char_tree.insert('c');
    char_tree.insert('f');

    ostringstream oss_inorder;
    char_tree.traverse_inorder(oss_inorder);
    //cout<<oss_inorder.str()<<endl;
    ostringstream oss_inorder2;
    for(BinarySearchTree<char>::Iterator i = char_tree.begin();
        i != char_tree.end(); i++){
            oss_inorder2 << *i << ' ';
        };
    //cout<<oss_inorder2.str()<<endl;
    ASSERT_EQUAL(oss_inorder.str(), oss_inorder2.str());
    ASSERT_EQUAL(oss_inorder2.str(), "c f k m o s ");
}

TEST(test_find_insert){
    BinarySearchTree<int> int_tree;
    BinarySearchTree<int>::Iterator itr1 = int_tree.insert(5);
    BinarySearchTree<int>::Iterator itr2 = int_tree.insert(3);
    BinarySearchTree<int>::Iterator itr3 = int_tree.insert(2);
    BinarySearchTree<int>::Iterator itr4 = int_tree.insert(7);
    BinarySearchTree<int>::Iterator itr5 = int_tree.insert(10);

    ASSERT_EQUAL(int_tree.size(), 5);
    ASSERT_EQUAL(int_tree.find(5), itr1);
    ASSERT_EQUAL(int_tree.find(3), itr2);
    ASSERT_EQUAL(int_tree.find(2), itr3);
    ASSERT_EQUAL(int_tree.find(7), itr4);
    ASSERT_EQUAL(int_tree.find(10), itr5);

    ASSERT_EQUAL(int_tree.min_greater_than(2), itr2);
    ASSERT_EQUAL(int_tree.min_greater_than(3), itr1);
    ASSERT_EQUAL(int_tree.min_greater_than(5), itr4);
    ASSERT_EQUAL(int_tree.min_greater_than(7), itr5);
    ASSERT_EQUAL(int_tree.min_greater_than(10), int_tree.end());

    ASSERT_EQUAL(int_tree.min_element(), itr3);
    ASSERT_EQUAL(int_tree.max_element(), itr5);

    BinarySearchTree<int>::Iterator itr6 = int_tree.insert(4);
    BinarySearchTree<int>::Iterator itr7 = int_tree.insert(18);
    BinarySearchTree<int>::Iterator itr8 = int_tree.insert(0);
    ASSERT_EQUAL(int_tree.min_element(), itr8);
    ASSERT_EQUAL(int_tree.max_element(), itr7);
    ASSERT_EQUAL(int_tree.min_greater_than(4), itr1);
    ASSERT_EQUAL(int_tree.min_greater_than(10), itr7);
    ASSERT_EQUAL(int_tree.min_greater_than(3), itr6);
    ASSERT_TRUE(int_tree.check_sorting_invariant());
}

TEST(test_empty_tree){
    BinarySearchTree<string> empty_tree;
    ASSERT_TRUE(empty_tree.empty());
    ASSERT_TRUE(empty_tree.check_sorting_invariant());
    ASSERT_EQUAL(empty_tree.max_element(), empty_tree.end());
    ASSERT_EQUAL(empty_tree.min_element(), empty_tree.end());
    ASSERT_EQUAL(empty_tree.min_greater_than("big"), empty_tree.end());
    ASSERT_EQUAL(empty_tree.find("small"), empty_tree.end());
    ASSERT_EQUAL(empty_tree.begin(), empty_tree.end());
    ASSERT_EQUAL(empty_tree.end(), BinarySearchTree<string>::Iterator());
    BinarySearchTree<string> empty_tree2(empty_tree);
    ASSERT_TRUE(empty_tree2.empty());
    ASSERT_EQUAL(empty_tree2.size(), 0);
    ASSERT_EQUAL(empty_tree2.height(), 0);
    ASSERT_TRUE(empty_tree2.check_sorting_invariant());
    BinarySearchTree<string> empty_tree3 = empty_tree;
    ASSERT_TRUE(empty_tree3.empty());
    ASSERT_EQUAL(empty_tree3.size(), 0);
    ASSERT_EQUAL(empty_tree3.height(), 0);  
    ASSERT_TRUE(empty_tree3.check_sorting_invariant());
}

// TEST(test_ducks){

//      class Duck {
//  public:
//    Duck() : wealth(0) {} // tree requires default-constructible
//    Duck(int wealth_in) : wealth(wealth_in) {}
//    int getWealth() const { return wealth; }

//  private:
//    int wealth;

//  };

//  class DuckWealthLess {
//      public:
//      bool operator() (const Duck &d1, const Duck &d2) const {
//          return d1.getWealth() < d2.getWealth();
//      }
//      };   
// ostream &operator<<(ostream &os, const Duck &duck) {
//              return os << "Duck: $" << duck.getWealth();
//      };


// }


TEST_MAIN()
