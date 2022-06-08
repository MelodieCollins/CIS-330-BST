#include "bst.h"

// ---------------------------------------
// Node class
// Default constructor
Node::Node() {
	parent = NULL;
	left = NULL;
	right = NULL;
	key = 0;
}
// Constructor
Node::Node(int in) {
	parent = NULL;
	left = NULL;
	right = NULL;
	key = in;
}
// Destructor
Node::~Node() {
// FIXME?
}

// Add parent 
void Node::add_parent(Node* in) {
	parent = in;
}
// Add to left of current node
void Node::add_left(Node* in) {
	left = in;
}
// Add to right of current node
void Node::add_right(Node* in) {
	right = in;
}

// Get key
int Node::get_key()
{
	return key;
}
// Get parent node
Node* Node::get_parent()
{
	return parent;
}
// Get left node
Node* Node::get_left()
{
	return left;
}
// Get right node
Node* Node::get_right()
{
	return right;
}
// Print the key to ostream to
// Do not change this
void Node::print_info(ostream& to)
{
    to << key << endl;
}
// ---------------------------------------


// ---------------------------------------
// BST class
// Walk the subtree from the given node
void BST::inorder_walk(Node* in, ostream& to)
{
	if(in != NULL){
		inorder_walk(in->get_left(), to);
		in->print_info(to);
		inorder_walk(in->get_right(), to);
	}
	else{
		// Break recursion when done
		return;
	}
}
// Constructor
BST::BST()
{
	root = NULL;
}
// Destructor
BST::~BST()
{
	if(root == NULL){ 
		return;
	}

	while(root != NULL){
		delete_node(root);
	}
}
// Insert a node to the subtree
void BST::insert_node(Node* in)
{
	Node *prev;
	prev = NULL;
	Node *curr = root;

	while(curr != NULL){
		// Keep track of parent node
		prev = curr;
		// Find lowest place on tree to insert node
		if(in->get_key() < curr->get_key()){
			curr = curr->get_left();
		}
		else{
			curr = curr->get_right();
		}
	}
	in->add_parent(prev);

	if(prev == NULL){
		root = in;
	}
	else if(in->get_key() < prev->get_key()){
		prev->add_left(in);
	}

	else{
		prev->add_right(in);
	}
	
	//delete prev;
	delete curr;
}
// Delete a node to the subtree
void BST::delete_node(Node* out)
{
	Node *node = tree_search(out->get_key());

	if(node->get_left() == NULL){
		// Replace one subtree as child of its parent with the subtree
		// rooted at current node and subtree rooted at previous node

		//transplant(node, node->get_right())
		if(node->get_parent() == NULL){
            		root = node->get_right();
		}
		else if(node == node->get_parent()->get_left()){
            		node->get_parent()->add_left(node->get_right());
		}
        	else{
           		node->get_parent()->add_right(node->get_right());
		}
        	if(node->get_right() !=  NULL){
            		node->get_right()->add_parent(node->get_parent());
		}
	}
	
	else if(node->get_right() == NULL){
		//transplant(node, node->get_left())
		if(node->get_parent() == NULL){
            		root = node->get_left();
		}
		else if(node == node->get_parent()->get_left()){
            		node->get_parent()->add_left(node->get_left());
		}
        	else{
           		node->get_parent()->add_right(node->get_left());
		}
        	if(node->get_left() !=  NULL){
            		node->get_left()->add_parent(node->get_parent());
		}
	}

	else{
		Node *prev = get_min(node->get_right());
		if(prev->get_parent() != node){
			//transplant(prev, prev->get_right())
			if(prev->get_parent() == NULL){
            			root = prev->get_right();
			}
			else if(prev == prev->get_parent()->get_left()){
            			prev->get_parent()->add_left(prev->get_right());
			}
        		else{
           			prev->get_parent()->add_right(prev->get_right());
			}
        		if(prev->get_right() !=  NULL){
            			prev->get_right()->add_parent(prev->get_parent());
			}

			prev->add_right(node->get_right());
			prev->get_right()->add_parent(prev);
		}
		//transplant(node, prev)
		if(node->get_parent() == NULL){
            		root = prev;
		}
		else if(node == node->get_parent()->get_left()){
            		node->get_parent()->add_left(prev);
		}
        	else{
           		node->get_parent()->add_right(prev);
		}
        	if(prev !=  NULL){
            		prev->add_parent(node->get_parent());
		}

		prev->add_left(node->get_left());
		prev->get_left()->add_parent(prev);
	}
	delete out;
}
// minimum key in the BST
Node* BST::tree_min()
{
	Node *min = root;
	while(min->get_left() != NULL){
                min = min->get_left();
        }
        return min;
}
// maximum key in the BST
Node* BST::tree_max()
{
	Node *max = root;
	while(max->get_right() != NULL){
                max = max->get_right();
        }
        return max;
}
// Get the minimum node from the subtree of given node
Node* BST::get_min(Node* in)
{
	while(in->get_left() != NULL){
		in = in->get_left();
	}
	return in;
}
// Get the maximum node from the subtree of given node
Node* BST::get_max(Node* in)
{
	while(in->get_right() != NULL){
		in = in->get_right();
	}
	return in;
}
// Get successor of the given node
Node* BST::get_succ(Node* in)
{
	if(in->get_right() != NULL){
		return get_min(in->get_right());
	}
	Node *y = in->get_parent();
	while(y != NULL and in == y->get_right()){
		in = y;
		y = y->get_parent();
	}
	return y;
}
// Get predecessor of the given node
Node* BST::get_pred(Node* in)
{
        if(in->get_left() != NULL){
                return get_max(in->get_left());
        }
        Node *y = in->get_parent();
        while(y != NULL and in == y->get_left()){
                in = y;
                y = y->get_parent();
        }
        return y;
}
// Walk the BST from min to max
void BST::walk(ostream& to)
{
	inorder_walk(root, to);
}
// Search the tree for a given key
Node* BST::tree_search(int search_key)
{
	Node *x = root;
	while(x != NULL and search_key != x->get_key()){
		if(search_key < x->get_key()){
			x = x->get_left();
		}
		else if(search_key > x->get_key()){
			x = x->get_right();
		}
	}
	return x;
}
// ---------------------------------------
