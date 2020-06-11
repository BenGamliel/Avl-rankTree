
#ifndef AVLTREE_H_
#define AVLTREE_H_

#include <iostream>
#include <cassert>

using std::cout;
using std::endl;
using std::ostream;

template<class P>
P max(P obj_1, P obj_2) {
    return obj_1 > obj_2 ? obj_1 : obj_2;
}

template<class K,class T>
class avltree {
    K key;
    T data;
    avltree* left_son;
    avltree* right_son;
    avltree* father;
    int height;
    int balance_factor;
    int sub_tree_size;
public :
    avltree() : left_son(NULL), right_son(NULL), father(NULL),sub_tree_size(1){}
    avltree(const K& key_t, const T& data_t, avltree* father_p) : key(key_t), data(data_t), left_son(NULL), right_son(NULL), father(father_p), height(0), balance_factor(0),sub_tree_size(1) {}
    ~avltree() {
        deleteTree(this->left_son);
        this->left_son=NULL;
    }
    void deleteTree(avltree* deleted_tree){
        if (deleted_tree==NULL){
            return;
        }
        deleteTree(deleted_tree->left_son);
        deleted_tree->left_son=NULL;
        deleteTree(deleted_tree->right_son);
        deleted_tree->right_son=NULL;
        delete deleted_tree;
    }
    avltree* find(const K& key_t) {
        if (this!=NULL && this->father == NULL) {
            return this->left_son->find(key_t);
        }
        else if (this == NULL || this->key == key_t) {
            return this;
        }
        else if (key_t < this->key) {
            return this->left_son->find(key_t);
        }
        else {
            return this->right_son->find(key_t);
        }
    }
    void insert(const K& key_t, const T& data_t) {
        if (this->father == NULL && this->left_son==NULL) {
            this->left_son = new avltree(key_t, data_t, this);
            return;
        }
        else if (this->father == NULL) {
            this->left_son->insert(key_t,data_t);
        }
        else if (key_t < this->key && this->left_son == NULL) {
            data+=data_t;
            this->sub_tree_size++;
            this->left_son = new avltree(key_t, data_t, this);
        }
        else if (key_t < this->key) {
            data+=data_t;
            this->sub_tree_size++;
            this->left_son->insert(key_t, data_t);
        }
        else if (this->right_son == NULL) {
            this->sub_tree_size++;
            this->right_son = new avltree(key_t, data_t, this);
        }
        else {
            this->sub_tree_size++;
            this->right_son->insert(key_t, data_t);
        }
        avltree* v = this;
        while (v->father) {
            max(v->left_son->calculateHeight(), v->right_son->calculateHeight());
            int max_son_height = max(v->left_son->calculateHeight(), v->right_son->calculateHeight());
            v->balance_factor = v->balanceFactor();
            if (v->height >= max_son_height + 1) {
                return;
            }
            v->height = max_son_height + 1;
            if (v->balance_factor >= 2 || v->balance_factor <= -2) {
                v->fixTree();
                return;
            }
            v = v->father;
        }
    }
    void remove(const K& key_t) {
        avltree* v = find(key_t);
        if (!v) {
            return;
        }
        avltree* v_father = v->father;
        if (v_father == NULL) {
            this->left_son->remove(key_t);
            return;
        }
        else if (v->left_son == NULL && v->right_son == NULL) { // no sons
            if (v_father->left_son == v) {
                v_father->left_son = NULL;
            }
            else {
                assert(v_father->right_son == v);
                v_father->right_son = NULL;
            }
            delete v;
        }
        else if (v->left_son == NULL) { // only right son exist
            if (v_father->left_son == v) {
                v_father->left_son = v->right_son;
            }
            else {
                assert(v->father->right_son == v);
                v_father->right_son = v->right_son;
            }
            v->right_son->father = v_father;
            v->right_son = NULL;
            delete v;
        }
        else if (v->right_son == NULL) { // only left son exist
            if (v_father->left_son == v) {
                v_father->left_son = v->left_son;
            }
            else {
                v_father->right_son = v->left_son;
            }
            v->left_son->father = v->father;
            v->left_son = NULL;
            delete v;
        }
        else { // both sons exist
            avltree* v_successor = v->right_son;
            while (v_successor->left_son) {
                v_successor = v_successor->left_son;
            }
            T v_data=v->data;
            K v_key=v->key;
            v->key = v_successor->key;
            v->data = v_successor->data;
            assert(v_key==key_t);
            v_successor->key = v_key;
            v_successor->data=v_data;
            v_successor->remove(key_t);
            return;
        }
        v_father->TreeFixingForRemove();
    }
    avltree* select(int index){
        avltree* v=this->left_son;
        while(v) {
            int w=v->left_son->calculateSubSize();
            if (w == index - 1) {
                return v;
            }
            else if (w>index-1){
                v=v->left_son;
            }
            else {
                index=index-w-1;
                v=v->right_son;
            }
        }
        return v;
    }
    int calculateSubSize(){
        if (this==NULL){
            return 0;
        }
        return sub_tree_size;
    }
    T calculateSumOfDataToIndex(int index){
        if (index<=0){
            return T();//TODO changed from 0 to T()
        }
        avltree* index_tree=select(index);
        T data=index_tree->getData();
        assert(index_tree!=NULL);
        while (index_tree->father->father!=NULL) {
            if (index_tree==index_tree->father->right_son){
                data+=index_tree->father->getData();
            }
            index_tree=index_tree->father;
        }
        return data;
    }
    // functions that return information of a tree
    int calculateHeight() {
        if (this == NULL) {
            return -1;
        }
        return height;
    }
    int balanceFactor() const {
        if (this == NULL) {
            return 0;
        }
        return this->left_son->calculateHeight() - this->right_son->calculateHeight();
    }
    int heightRecursive() {
        if (this == NULL) {
            return -1;
        }
        return 1 + max(this->left_son->heightRecursive(), this->right_son->heightRecursive());
    }
    T& getData() {
        assert(this != NULL);
        if (this->father == NULL) {
            return this->left_son->getData();
        }
        return this->data;
    }
    K& getKey() {
        assert(this != NULL);
        if (this->father == NULL) {
            return this->left_son->getKey();
        }
        return this->key;
    }
    avltree* getMaximum() {
        if (this->father == NULL && this->left_son!=NULL) {
            return this->left_son->getMaximum();
        }
        else if (this->father == NULL && this->left_son == NULL) {
            return NULL;
        }
        else if (this->right_son != NULL) {
            return this->right_son->getMaximum();
        }
        else {
            return this;
        }
    }
    // functions to keep the avl tree structure correct( rolls functions )
    void updateHeightAndBfValues() {
        if (this == NULL) {
            return;
        }
        if (this->father == NULL) {
            this->left_son->updateHeightAndBfValues();
            return;
        }
        this->left_son->updateHeightAndBfValues();
        this->right_son->updateHeightAndBfValues();
        this->height = heightRecursive();
        this->balance_factor = this->balanceFactor();
    }
    void fixTree() {
        if (this == NULL || (balance_factor<2 && balance_factor>-2)) {
            return;
        }
        if (balance_factor == 2) {
            if (this->left_son->balance_factor >= 0) {
                LL_Roll();
            }
            else if (this->left_son->balance_factor == -1) {
                LR_Roll();
            }
        }
        else if (balance_factor == -2) {
            if (this->right_son->balance_factor <= 0) {
                RR_Roll();
            }
            else if (this->right_son->balance_factor == 1) {
                RL_Roll();
            }
        }
    }
    void TreeFixingForRemove() {
        avltree* v = this;
        while (v->father) {
            max(v->left_son->calculateHeight(), v->right_son->calculateHeight());
            int max_son_height = max(v->left_son->calculateHeight(), v->right_son->calculateHeight());
            v->balance_factor = v->balanceFactor();
            if (v->height >= max_son_height + 1) {
                return;
            }
            v->height = max_son_height + 1;
            if (v->balance_factor >= 2 || v->balance_factor <= -2) {
                v->fixTree();
            }
            v = v->father;
        }
    }
    void LL_Roll() {
        avltree* a_right = this->left_son->right_son;
        this->left_son->right_son = this;
        this->left_son->father = this->father;
        if (this == this->father->left_son) {
            this->father->left_son = this->left_son;
        }
        else {
            this->father->right_son = this->left_son;
        }
        this->father = this->left_son;
        this->left_son = a_right;
        if (a_right) {
            a_right->father = this;
        }
        int max_son_height = max(this->left_son->calculateHeight(), this->right_son->calculateHeight());
        this->height = max_son_height + 1;
        this->father->height = 1 + max(this->father->left_son->calculateHeight(), this->father->right_son->calculateHeight());
        this->balance_factor = this->balanceFactor();
        this->father->balance_factor = this->father->balanceFactor();
        this->sub_tree_size=this->left_son->calculateSubSize()+this->right_son->calculateSubSize()+1;
        this->father->sub_tree_size=this->calculateSubSize()+this->father->left_son->calculateSubSize()+1;
        this->data-=this->father->getData();
    }
    void LR_Roll() {
        this->left_son->RR_Roll();
        this->LL_Roll();
    }
    void RR_Roll() {
        avltree* a_left = this->right_son->left_son;
        this->right_son->left_son = this;
        this->right_son->father = this->father;
        if (this == this->father->left_son) {
            this->father->left_son = this->right_son;
        }
        else {
            this->father->right_son = this->right_son;
        }
        this->father = this->right_son;
        this->right_son = a_left;
        if (a_left) {
            a_left->father = this;
        }
        int max_son_height = max(this->left_son->calculateHeight(), this->right_son->calculateHeight());
        this->height = max_son_height + 1;
        this->father->height = 1 + max(this->father->left_son->calculateHeight(), this->father->right_son->calculateHeight());
        this->balance_factor = this->balanceFactor();
        this->father->balance_factor = this->father->balanceFactor();
        this->sub_tree_size=this->left_son->calculateSubSize()+this->right_son->calculateSubSize()+1;
        this->father->sub_tree_size=this->calculateSubSize()+this->father->right_son->calculateSubSize()+1;
        this->father->data+=this->getData();
    }
    void RL_Roll() {
        this->right_son->LL_Roll();
        this->RR_Roll();
    }
    //special functions , used for hw1wet
    void putTreeKeyInArrayByInOrder(K* key_array, int key_array_size, int* index_p) {
        if (key_array_size==0){
            return;
        }
        assert(*index_p < key_array_size);
        avltree* v = this;
        if (v == NULL) {
            return;
        }
        if (v->father == NULL) {
            v = v->left_son;
        }
        v->left_son->putTreeKeyInArrayByInOrder(key_array, key_array_size, index_p);
        K key_obj = v->key;
        key_array[*index_p] = key_obj;
        (*index_p)++;
        v->right_son->putTreeKeyInArrayByInOrder(key_array, key_array_size, index_p);
    }
    void putTreeDataInArrayByInOrder(T* data_array, int data_array_size, int* index_p) {
        if (data_array_size==0){
            return;
        }
        assert(*index_p <= data_array_size);
        avltree* v = this;
        if (v == NULL) {
            return;
        }
        if (v->father == NULL) {
            v = v->left_son;
        }
        v->left_son->putTreeDataInArrayByInOrder(data_array, data_array_size, index_p);
        T data_obj = v->data;
        data_array[*index_p] = data_obj;
        (*index_p)++;
        v->right_son->putTreeDataInArrayByInOrder(data_array, data_array_size, index_p);
    }
    void putTreeDataInArrayByInOrderRightFirst(T& data_array, int data_array_size, int* index_p) {
        //  assert(index_p!=NULL);
        if (data_array_size==0){
            return;
        }
        assert(*index_p <= data_array_size && *index_p>=0);
        avltree* v = this;
        if (v == NULL) {
            return;
        }
        if (v->father == NULL) {
            v = v->left_son;
        }
        v->right_son->putTreeDataInArrayByInOrderRightFirst(data_array, data_array_size, index_p);
        T data_obj = v->data;
        data_array[*index_p] = data_obj;
        (*index_p)++;
        v->left_son->putTreeDataInArrayByInOrderRightFirst(data_array, data_array_size, index_p);
    }
    void putTreeInArrayByInOrder(avltree* tree_array, int tree_array_size, int* index_p) const {
        if (tree_array_size==0){
            return;
        }
        assert(*index_p <= tree_array_size);
        const avltree* v = this;
        if (v == NULL) {
            return;
        }
        if (v->father == NULL) {
            v = v->left_son;
        }
        v->left_son->putTreeInArrayByInOrder(tree_array, tree_array_size, index_p);
        (tree_array[*index_p]).data = v->data;
        (tree_array[*index_p]).key = v->key;
        (*index_p)++;
        v->right_son->putTreeInArrayByInOrder(tree_array, tree_array_size, index_p);
    }
    void makeEmptyTree(avltree* new_tree, int tree_size) {
        assert(tree_size >= 0);
        if (tree_size == 0) {
            new_tree->left_son = NULL;
            return;
        }
        int tree_size_helper = 1;
        int i = 0;
        while (tree_size_helper <= tree_size) {
            i++;
            tree_size_helper *= 2;
        }
        tree_size_helper--;
        int leaves_removing = tree_size_helper - tree_size;
        assert(leaves_removing >= 0);
        new_tree->EmptyTreeCreate(i); // i is the new tree height
        new_tree->deleteLeaves(&leaves_removing);
        assert(leaves_removing == 0);
    }
    int calculateSize() const {
        if (this == NULL) {
            return 0;
        }
        if (this->father == NULL) {
            return this->left_son->calculateSize();
        }
        return 1 + this->left_son->calculateSize() + this->right_son->calculateSize();
    }
    void EmptyTreeCreate(int tree_size) {
        if (this->father == NULL) {
            this->left_son = new avltree();
            this->left_son->father = this;
            this->left_son->EmptyTreeCreate(tree_size-1);
            return;
        }
        if (tree_size == 0) {
            return;
        }
        this->left_son = new avltree();
        this->left_son->father = this;
        this->right_son = new avltree();
        this->right_son->father = this;
        this->left_son->EmptyTreeCreate(tree_size - 1);
        this->right_son->EmptyTreeCreate(tree_size - 1);
    }
    void deleteLeaves(int* leaves_removed_number) {
        assert(leaves_removed_number >= 0);
        if ((*leaves_removed_number) == 0) {
            return;
        }
        if (this == NULL) {
            return;
        }
        else if (this->father == NULL) {
            this->left_son->deleteLeaves(leaves_removed_number);
            return;
        }
        this->right_son->deleteLeaves(leaves_removed_number);
        if (this->left_son == NULL && this->right_son == NULL) {
            if (this->father->left_son == this) {
                this->father->left_son = NULL;
            }
            else {
                assert(this->father->right_son == this);
                this->father->right_son = NULL;
            }
            delete this;
            (*leaves_removed_number)--;
            return;
        }
        this->left_son->deleteLeaves(leaves_removed_number);
    }
    void mergeTrees(avltree& rhs) {
        int merge_to_size = this->calculateSize();
        int merge_from_size = rhs.calculateSize();
        avltree** merge_to_array = new avltree*;
        *merge_to_array = new avltree[merge_to_size];
        avltree** merge_from_array = new avltree*;
        *merge_from_array = new avltree[merge_from_size];
        int index = 0;
        this->putTreeInArrayByInOrder(*merge_to_array, merge_to_size, &index);
        index = 0;
        rhs.putTreeInArrayByInOrder(*merge_from_array, merge_from_size, &index);
        avltree* combined_array = new avltree[merge_to_size + merge_from_size];
        mergeArraysOfTrees(combined_array, *merge_to_array,merge_to_size, *merge_from_array,merge_from_size);
        this->deleteTree(this->left_son);
        this->left_son=NULL;
        makeEmptyTree(this, merge_to_size + merge_from_size);
        index = 0;
        this->insertArrayToTree(combined_array,merge_to_size+merge_from_size, &index);
        this->updateHeightAndBfValues();
        deleteTree(rhs.left_son);
        rhs.left_son=NULL;
        rhs.updateHeightAndBfValues();
        delete[] combined_array;
        delete[] * merge_from_array;
        delete[] * merge_to_array;
        delete merge_from_array;
        delete merge_to_array;
    }
    void insertArrayToTree(avltree* array_inserted,int array_size,int* index) {
        assert(*index <= array_size);
        if (array_size==0){
            return;
        }
        if (this == NULL) {
            return;
        }
        if (this->father == NULL) {
            this->left_son->insertArrayToTree(array_inserted, array_size, index);
            return;
        }
        this->left_son->insertArrayToTree(array_inserted, array_size, index);
        this->key = array_inserted[*index].key;
        this->data = array_inserted[*index].data;
        (*index)++;
        this->right_son->insertArrayToTree(array_inserted, array_size, index);
    }
    void mergeArraysOfTrees(avltree* combined_array,avltree* array_1,int size_array_1,avltree* array_2,int size_array_2) {
        int i = 0;
        int j = 0;
        int k = 0;
        int combined_size = size_array_1 + size_array_2;
        while (k < combined_size) {
            while (i < size_array_1 && j < size_array_2) {
                if (array_1[i].key < array_2[j].key) {
                    combined_array[k].key = array_1[i].key;
                    combined_array[k].data = array_1[i].data;
                    i++;
                }
                else {
                    combined_array[k].key = array_2[j].key;
                    combined_array[k].data = array_2[j].data;
                    j++;
                }
                k++;
            }
            while (i < size_array_1) {
                combined_array[k].key = array_1[i].key;
                combined_array[k].data = array_1[i].data;
                i++;
                k++;
            }
            while (j < size_array_2) {
                combined_array[k].key = array_2[j].key;
                combined_array[k].data = array_2[j].data;
                j++;
                k++;
            }
        }
    }
    template<class A,class P>
    void doSomethingOnAllTreeKey(A action,const P& parameter) {
        if (this == NULL) {
            return;
        }
        if (this->father == NULL) {
            this->left_son->doSomethingOnAllTreeKey(action,parameter);
            return;
        }
        this->left_son->doSomethingOnAllTreeKey(action,parameter);
        this->right_son->doSomethingOnAllTreeKey(action,parameter);
        action(this->getKey(), parameter);
    }
    template<class A>
    void doSomethingOnAllTreeData(A action) {
        if (this == NULL) {
            return;
        }
        if (this->father == NULL) {
            this->left_son->doSomethingOnAllTreeData(action);
            return;
        }
        action(this->getData());
        this->left_son->doSomethingOnAllTreeData(action);
        this->right_son->doSomethingOnAllTreeData(action);
    }
    template<class A,class P>
    void doSomethingOnAllTreeData(A action,const P& parameter) {
        if (this == NULL) {
            return;
        }
        if (this->father == NULL) {
            this->left_son->doSomethingOnAllTreeData(action,parameter);
            return;
        }
        action(this->getData(),parameter);
        this->left_son->doSomethingOnAllTreeData(action,parameter);
        this->right_son->doSomethingOnAllTreeData(action,parameter);
    }
};

#endif /* AVLTREE_H_ */
