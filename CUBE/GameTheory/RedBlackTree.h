#ifndef CUBE_REDBLACKTREE_H
#define CUBE_REDBLACKTREE_H

#include <memory>
#include <stdexcept>
#include <iostream>

enum tree_node_color {BLACK, RED};

template<typename N>
struct TreeNode{
    N data;
    tree_node_color color; // true if Treenode is black false if Treenode is red
    std::shared_ptr<TreeNode<N>> parent;
    std::shared_ptr<TreeNode<N>> left;
    std::shared_ptr<TreeNode<N>> right;

    bool isLeaf(){
        return left == nullptr and right == nullptr;
    }
};

template<class T, class Comparator = std::less_equal<T>>
class RedBlackTree{
    /**
     * A red-black tree is a binary search tree following these 5 properties:
     * 1. Each node is black or red;
     * 2. The root is black;
     * 3. All leaves (nullptr) are black;
     * 4. The children of a red nodes are black;
     * 5. Every path from a given node to any of its descendant leaf goes through the same number of black nodes.
     */
public:

    void insert(T x){
        std::shared_ptr<TreeNode<T>> n = std::make_shared<TreeNode<T>>();
        n->data = x;
        if (root == nullptr){
            n->color = BLACK;
            root = n;
        }
        else{
            insertRecursion(root, n);
            repairInsertion(n);
        }
        numNodes++;
    }

    void print(){
        printHelper(root, "", true);
    }

    const std::shared_ptr<TreeNode<T>> &getRoot() const {
        return root;
    }

    std::shared_ptr<TreeNode<T>> search(T const& x){
        return searchHelper(root, x);
    }

    /**
     *
     * @param n the root of the subtree
     * @return the node with the smaller value in the subtree of root n
     */
    std::shared_ptr<TreeNode<T>> left_most_child(std::shared_ptr<TreeNode<T>> n){
        if (n->left == nullptr)
            return n;
        else
            return left_most_child(n->left);
    }

    bool isBlackNode(std::shared_ptr<TreeNode<T>> n){
        if (n == nullptr)
            return true;
        else
            return n->color == BLACK;
    }

    void remove(T x) {
        std::shared_ptr<TreeNode<T>> n = search(x);
        if (n != nullptr) {
            if (n->left != nullptr and n->right != nullptr) {
                std::shared_ptr<TreeNode<T>> m = left_most_child(n->right);
                // Node m has at most one right child by definition of the in-order
                n->data = m->data;
                deleteNodeWithAtMostOneChild(m);
            } else {
                deleteNodeWithAtMostOneChild(n);
            }
            numNodes--;
        }
    }

    size_t size(){
        return numNodes;
    }

    std::vector<T> values() {
        return valuesHelper(root);
    }


protected:

    std::shared_ptr<TreeNode<T>> root;

    size_t numNodes = 0;

    std::shared_ptr<TreeNode<T>> getParent(std::shared_ptr<TreeNode<T>> n) {
        return n == nullptr ? nullptr : n->parent;
    }

    std::shared_ptr<TreeNode<T>> getGrandParent(std::shared_ptr<TreeNode<T>> n){
        return getParent(getParent(n));
    }

    std::shared_ptr<TreeNode<T>> getSibling(std::shared_ptr<TreeNode<T>> n){
        std::shared_ptr<TreeNode<T>> p = getParent(n);
        if(p == nullptr)
            return nullptr;
        if (n == p->left)
            return p->right;
        else
            return p->left;
    }

    std::shared_ptr<TreeNode<T>> getUncle(std::shared_ptr<TreeNode<T>> n){
        return getSibling(getParent(n));
    }


    void rotateLeft(std::shared_ptr<TreeNode<T>> n){
        std::shared_ptr<TreeNode<T>> r_child = n->right;
        std::shared_ptr<TreeNode<T>> p = getParent(n);
        if (r_child == nullptr)
            throw std::invalid_argument("The right child of this Treenode is null. We cannot perform a left rotation.");

        n->right = r_child->left;
        r_child->left = n;
        n->parent = r_child;
        //Update child/parent pointers.
        if (n->right != nullptr)
            n->right->parent = n;

        if (p != nullptr) { // if n was not the root
            if (n == p->left)
                p->left = r_child;
            else
                p->right = r_child;
        }
        else // n was the root, r_child is the new root
            root = r_child;
        r_child->parent = p;
    }

    void rotateRight(std::shared_ptr<TreeNode<T>> n){
        std::shared_ptr<TreeNode<T>> l_child = n->left;
        std::shared_ptr<TreeNode<T>> p = getParent(n);
        if (l_child == nullptr)
            throw std::invalid_argument("The left child of this Treenode is null. We cannot perform a right rotation.");

        n->left = l_child->right;
        l_child->right = n;
        n->parent = l_child;
        if(n->left != nullptr)
            n->left->parent = n;

        if (p != nullptr){
            if (n == p->left)
                p->left = l_child;
            else
                p->right = l_child;
        }
        else // n was the root, r_child is the new root
            root = l_child;
        l_child->parent = p;
    }

    void insertRecursion(std::shared_ptr<TreeNode<T>> current, std::shared_ptr<TreeNode<T>> n){

        if (Comparator()(n->data, current->data) and current->left == nullptr) {
            current->left = n;
            n->parent = current;
            n->color = RED;
        }
        else if (not Comparator()(n->data, current->data) and current->right == nullptr) {
            current->right = n;
            n->parent = current;
            n->color = RED;
        }

        else{
            if (Comparator()(n->data, current->data))
                insertRecursion(current->left, n);
            else
                insertRecursion(current->right, n);
        }
    }

    void repairInsertion(std::shared_ptr<TreeNode<T>> n){
        if (getParent(n) == nullptr){ // If n is the root, case already managed
            ;
        }
        else if(getParent(n)->color == BLACK){ //Nothing to do
            ;
        }
        //If we put theses two empty condition, it is to ease the following condition
        else if(getUncle(n) != nullptr and getUncle(n)->color == RED)
            insertCase3(n);
        else
            insertCase4(n);
    }

    void insertCase3(std::shared_ptr<TreeNode<T>> n){
        getParent(n)->color = BLACK;
        getUncle(n)->color = BLACK;
        std::shared_ptr<TreeNode<T>> g = getGrandParent(n);
        if (g != root)
            g->color = RED;
        repairInsertion(g); // g may be red and have a red parent
    }

    void insertCase4(std::shared_ptr<TreeNode<T>> n){
        std::shared_ptr<TreeNode<T>> p = getParent(n);
        std::shared_ptr<TreeNode<T>> g = getGrandParent(n);

        //If n is "inside" the tree, we must to put it "outside"
        if (n == p->right and p == g->left){
            rotateLeft(p);
            insertCase4Step2(n->left);
        }
        else if (n == p->left and p == g->right){
            rotateRight(p);
            insertCase4Step2(n->right);
        }
        else
            insertCase4Step2(n);

    }

    void insertCase4Step2(std::shared_ptr<TreeNode<T>> n){
        std::shared_ptr<TreeNode<T>> p = getParent(n);
        std::shared_ptr<TreeNode<T>> g = getGrandParent(n);

        if (n == p->left)
            rotateRight(g);
        else
            rotateLeft(g);
        p->color = BLACK;
        g->color = RED;

    }

    void printHelper(std::shared_ptr<TreeNode<T>> n, std::string indent, bool last){
        if (n != nullptr){
            std::cout << indent;
            if (last){
                if(n != root)
                    std::cout << "R----";
                indent += "     ";
            } else{
                std::cout<<"L----";
                indent += "|    ";
            }

            std::string s_color = n->color ? "RED" : "BLACK";
            std::cout << n->data << "(" << s_color << ")" << std::endl;
            printHelper(n->left, indent, false);
            printHelper(n->right, indent, true);

        }
    }

    std::shared_ptr<TreeNode<T>> searchHelper(std::shared_ptr<TreeNode<T>> current, T const& x) {
        if (current == nullptr)
            return nullptr;
        else {
            if (x == current->data)
                return current;
            else if (not Comparator()(x, current->data))
                return searchHelper(current->right, x);
            else
                return searchHelper(current->left, x);
        }
    }

    /**
     *
     * @param m supposed to have at most only one child
     */
    void deleteNodeWithAtMostOneChild(std::shared_ptr<TreeNode<T>> m){
        std::shared_ptr<TreeNode<T>> child = m->right != nullptr ? m->right :  m->left;
        std::shared_ptr<TreeNode<T>> p = getParent(m);
        if (m->color == BLACK and m->isLeaf()){
            deleteCase1(m);
        }
        else if(child != nullptr and child->color == RED){
            //The child the place and the color m
            child->color = BLACK; //Because of property 4 if child is red, m must be black
            child->parent = m->parent;
        }
        //Because of property 5, the case where m and child are black with child != nullptr can never occur.

        //It doesn't matter if child is nullptr or not, it becomes the new child of p
        if (p != nullptr) {
            if (m == p->left)
                p->left = child;
            else
                p->right = child;
        }
        else{
            root = child;
        }
        m->parent = nullptr;
        m->right = nullptr;
        m->left = nullptr;
    }

    /**
     * If m->parent is null, we are removing or changing it with another black node (see deleteCase 3).
     * So nothing to do to conserve the properties.
     * @param m
     */
    void deleteCase1(std::shared_ptr<TreeNode<T>> m){
        if (m->parent != nullptr)
            deleteCase2(m);
        //If m->parent is null, we are removing or changing it with another black node (see deleteCase 3).
        // So nothing to do to conserve the properties
    }

    /**
     * If m has a red sibling s. It can have black children. So remove m can lead to lead to violate property 5.
     * We change the color of s and the parent p and we perform a rotation on p. So now, m has red parent and black
     * sibling. These situation is managed on deleteCase4, 5 or 6.
     * @param m
     */
    void deleteCase2(std::shared_ptr<TreeNode<T>> m){
        std::shared_ptr<TreeNode<T>> s = getSibling(m);
        std::shared_ptr<TreeNode<T>> p = getParent(m);

        if(s != nullptr and s->color == RED){
            p->color = RED;
            s->color = BLACK;
            if (m == p->left)
                rotateLeft(p);
            else
                rotateRight(p);
        }
        //At this point s is black
        deleteCase3(m);
    }

    /**
     * Turning s red and removing decrease the black height of path pass trough p by 1. However, the black height of
     * path not passing trough p don't decrease. We need to re-balance the tree from p.
     * @param m
     */
    void deleteCase3(std::shared_ptr<TreeNode<T>> m){
        std::shared_ptr<TreeNode<T>> s = getSibling(m);
        std::shared_ptr<TreeNode<T>> p = getParent(m);

        if(p->color == BLACK and s != nullptr and s->color == BLACK
        and isBlackNode(s->left) and isBlackNode(s->right)){
            //At this point s is black and have null children or red children. Red children are managed in further cases
            s->color = RED;
            deleteCase1(p);
        }
        else{
            deleteCase4(m);
        }
    }

    /**
     * If parent p is red and sibling s is black, by switching their color, the black height doesn't change
     * when we remove m.
     * @param m
     */
    void deleteCase4(std::shared_ptr<TreeNode<T>> m){
        std::shared_ptr<TreeNode<T>> s = getSibling(m);
        std::shared_ptr<TreeNode<T>> p = getParent(m);

        if (p->color == RED and s != nullptr and s->color == BLACK
        and isBlackNode(s->left) and isBlackNode(s->right)){
            //At this point s is black and have null children or red children. Red children are managed in further cases
            p->color = BLACK;
            s->color = RED;
        }
        else{
            deleteCase5(m);
        }
    }

    /**
     * Some color permutation and rotation to fit in deleteCase6
     * @param m
     */
    void deleteCase5(std::shared_ptr<TreeNode<T>> m){
        std::shared_ptr<TreeNode<T>> s = getSibling(m);
        std::shared_ptr<TreeNode<T>> p = getParent(m);

        //At this point, s s has at least one red child
        if (s != nullptr and s->color == BLACK){
            if (m == p->left and s->left != nullptr and s->left->color == RED and isBlackNode(s->right)){
                //By property 5, as m and s are black, s can have not null black child. The case where the right child
                // of s is red is managed in deleteCase6
                s->color = RED;
                s->left->color = BLACK;
                rotateRight(s);
            } else if (m == p->right and s->right != nullptr and s->right->color == RED and isBlackNode(s->left)){
                s->color = RED;
                s->right->color = BLACK;
                rotateLeft(s);
            }
        }
        deleteCase6(m);
    }

    void deleteCase6(std::shared_ptr<TreeNode<T>> m){
        std::shared_ptr<TreeNode<T>> s = getSibling(m);
        std::shared_ptr<TreeNode<T>> p = getParent(m);

        s->color = p->color;
        p->color = BLACK;

        if (m == p->left){
            //At this point, we are sur that s has a red right child
            s->right->color = BLACK;
            rotateLeft(p);
        }
        else{
            s->left->color = BLACK;
            rotateRight(p);
        }
    }

    std::vector<T> valuesHelper(std::shared_ptr<TreeNode<T>> current){
        std::vector<T> to_return;
        if (current->isLeaf()){
            to_return.push_back(current->data);
            return to_return;
        } else{
            if (current->left != nullptr){
                std::vector<T> left_values = valuesHelper(current->left);
                to_return.insert(to_return.end(), left_values.begin(), left_values.end());
            }
            to_return.push_back(current->data);
            if (current->right != nullptr){
                std::vector<T> right_values = valuesHelper(current->right);
                to_return.insert(to_return.end(), right_values.begin(), right_values.end());
            }
            return to_return;
        }
    }
};

#endif //CUBE_REDBLACKTREE_H
