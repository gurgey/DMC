#ifndef ORDERSTAT
#define ORDERSTAT

#include <utility>
#include <algorithm>

template<typename T>
class OrderStat {
	struct Node {
		Node(Node *p):left(nullptr),right(nullptr),parent(p) {}
		unsigned int height;
		unsigned int sz;
		T data;
		Node *left;
		Node *right;
		Node *parent;
	};

	Node *root;

public:
	typedef T value_type;

	class const_iterator;

	class iterator {
		Node *n;
		bool end;
	public:
		friend class const_iterator;
		iterator(Node *node):n(node),end(n==nullptr) {}
		iterator(Node *node,bool isEnd):n(node),end(isEnd) {}

		bool operator==(const iterator &i) const { return end==i.end && n==i.n; }
		bool operator!=(const iterator &i) const { return !(*this==i); }
		value_type &operator*() { return n->data; }
		iterator &operator++() {
			if(n==nullptr) return *this;
			if(n->right==nullptr) {
				Node *tmp = n;
				Node *c = n;
				n = n->parent;
				while(n!=nullptr && c==n->right) {
					c = n;
					n = n->parent;
				}
				if(n==nullptr) {
					n = tmp;
					end = true;
				}
			} else {
				n = n->right;
				while(n->left!=nullptr) n = n->left;
			}
			return *this;
		}
		iterator &operator--() {
			if(end) end = !end;
			else {
				if(n->left==nullptr) {
					Node *tmp = n;
					Node *c = n;
					n = n->parent;
					while(n!=nullptr && c==n->left) {
						c = n;
						n = n->parent;
					}
					if(n==nullptr) {
						n = tmp;
						end = true;
					}
				} else {
					n = n->left;
					while(n->right!=nullptr) n = n->right;
				}
			}
			return *this;
		}
		iterator operator++(int) {
			iterator tmp(*this);
			++(*this);
			return tmp;
		}
		iterator operator--(int) {
			iterator tmp(*this);
			--(*this);
			return tmp;
		}
	};

	class const_iterator {
		Node *n;
		bool end;
	public:
		friend class OrderStat<T>;
		const_iterator(Node *node):n(node),end(n==nullptr) {}
		const_iterator(Node *node,bool isEnd):n(node),end(isEnd) {}
		const_iterator(const iterator &iter):n(iter.n),end(iter.end) {}

		bool operator==(const const_iterator &i) const { return end==i.end && n==i.n; }
		bool operator!=(const const_iterator &i) const { return !(*this==i); }
		value_type &operator*() { return n->data; }
		const_iterator &operator++() {
			if(n==nullptr) return *this;
			if(n->right==nullptr) {
				Node *tmp = n;
				Node *c = n;
				n = n->parent;
				while(n!=nullptr && c==n->right) {
					c = n;
					n = n->parent;
				}
				if(n==nullptr) {
					n = tmp;
					end = true;
				}
			} else {
				n = n->right;
				while(n->left) n = n->left;
			}
			return *this;
		}
		const_iterator &operator--() {
			if(end) end = !end;
			else {
				if(n->left==nullptr) {
					Node *tmp = n;
					Node *c = n;
					n = n->parent;
					while(n!=nullptr && c==n->left) {
						c = n;
						n = n->parent;
					}
					if(n==nullptr) {
						n = tmp;
						end = true;
					}
				} else {
					n = n->left;
					while(n->right!=nullptr) n = n->right;
				}
			}
			return *this;
		}
		const_iterator operator++(int) {
			const_iterator tmp(*this);
			++(*this);
			return tmp;
		}
		const_iterator operator--(int) {
			const_iterator tmp(*this);
			--(*this);
			return tmp;
		}
	};

	OrderStat(): root(nullptr) {}
	OrderStat(const OrderStat<T> &that) {
		root = recurCopy(that.root, nullptr);
	}
	~OrderStat() {
		deleteAll(root);
	}

	OrderStat &operator=(const OrderStat<T> &that) {
		if(&that == this) return *this;
		deleteAll(root);
		root = recurCopy(that.root,nullptr);
		return *this;
	}

	bool empty() const { return root == nullptr; }

	unsigned size() const { return root ? root->sz : 0; }

	value_type &front();
	const value_type &front() const;
	value_type &back();
	const value_type &back() const;

	iterator insert(const_iterator position,const value_type& val);

	template <class InputIterator>
	void insert(const_iterator position,InputIterator first, InputIterator last) {
		for(auto iter = first; iter!=last; ++iter) {
			insert(*iter);
		}
	}

	iterator erase(const_iterator position);

	void clear();

	value_type &operator[](int index);

	void push_front(const value_type& val);
	void pop_front();
	void push_back(const value_type& val);
	void pop_back();

	bool operator==(const OrderStat<T>& rhs) const;

	bool operator!=(const OrderStat<T>& rhs) const;


	iterator begin() {
		if(root==nullptr) return iterator(root);
		Node *n = root;
		while(n->left!=nullptr) n = n->left;
		return iterator(n);
	}

	const_iterator begin() const {
		if(root==nullptr) return iterator(root);
		Node *n = root;
		while(n->left!=nullptr) n = n->left;
		return const_iterator(n);
	}

	iterator end() {
		if(root==nullptr) return iterator(root);
		Node *n = root;
		while(n->right!=nullptr) n = n->right;
		return iterator(n,true);
	}

	const_iterator end() const {
		if(root==nullptr) return const_iterator(root);
		Node *n = root;
		while(n->right!=nullptr) n = n->right;
		return const_iterator(n,true);
	}

	const_iterator cbegin() const {
		if(root==nullptr) return const_iterator(root);
		Node *n = root;
		while(n->left!=nullptr) n = n->left;
		return const_iterator(n);
	}

	const_iterator cend() const {
		Node *n = root;
		while(n->right!=nullptr) n = n->right;
		return const_iterator(n,true);
	}

	//helpers
	void deleteAll(Node *n) {
		if(n!=nullptr) {
			deleteAll(n->left);
			deleteAll(n->right);
			delete(n);
		}
	}

	Node *recurCopy(Node *n,Node *p) {
		if(n==nullptr) return nullptr;
		Node *n2 = new Node(p);
		n2->data = n->data;
		n2->height = n->height;  
		n2->sz = n->sz;           
		n2->left = recurCopy(n->left,n2);
		n2->right = recurCopy(n->right,n2);
		return n2;
	}

	void transplant(Node *u,Node *v) {
		if(u->parent == nullptr) {
			root = v;
		} else if(u==u->parent->left) {
			u->parent->left = v;
		} else u->parent->right = v;
		if(v) v->parent = u->parent;
	}

	void removeNode(Node *z) {
		Node* del;
		//if(!z->left && !z->right) {
		//	del = z->parent;
		//	transplant(z,z->right);  //??
		if(z->left==nullptr) {
			del = z->right;
			transplant(z,z->right);
		} else if(z->right==nullptr) {
			del = z->left;
			transplant(z,z->left);
		} else {
			Node* y;
			if(getSize(z->left) < getSize(z->right)) {							// for efficiency, try taking out either "if" or "else".
				y = z->right;
				while(y->left) y = y->left;
			} else {													//
				y = z->left;										//
				while(y->right) y = y->right;							
			}					
			del = y;                                                //
			if(y->parent!=z) {
				transplant(y,y->right);
				y->right = z->right;
				y->right->parent = y;
			}
			transplant(z,y);
			y->left = z->left;
			y->left->parent = y;
		}
		checkBalance(del);											//
		delete z;
		//z = nullptr;
	}
	/*
	void removeNode(Node *z) {
	if(z->left==nullptr) {
	Node *zr = z->right;
	transplant(z,z->right);
	if(zr) {
	checkBalance(zr);
	} else {
	checkBalance(z->parent);
	}
	} else if(z->right==nullptr) {
	Node *zl = z->left;
	transplant(z,z->left);
	checkBalance(zl);
	} else {
	Node *y = z->right;
	while(y->left!=nullptr) y = y->left;
	Node *lowest = (y->right==nullptr) ? ((y==z->right) ? y : y->parent) : y->right;
	if(y->parent!=z) {
	transplant(y,y->right);
	y->right = z->right;
	y->right->parent = y;
	}
	transplant(z,y);
	y->left = z->left;
	y->left->parent = y;
	checkBalance(lowest);
	}
	delete z;
	}
	*/

	int getHeight(Node *n) {
		if(n) return n->height;
		else return -1;
	}

	int getSize(Node *n) {
		if(n) return n->sz;
		else return 0;
	}

	void resetHS(Node *n) {
		if(n) { 
			n->height = std::max(getHeight(n->left), getHeight(n->right))+1;
			n->sz = getSize(n->left)+getSize(n->right)+1;
		}
		else { 
			n->height = -1;												// dc
			n->sz = 0;													// dc		
		}
	}

	void checkBalance(Node *v) {
		Node* n = v;
		while(n) { 
			if(getHeight(n->left) - getHeight(n->right) == 2) {
				if(getHeight(n->left->right) - getHeight(n->left->left) == 1) {
					rotateLeft(n->left);
				}
				rotateRight(n);
			} else if(getHeight(n->left) - getHeight(n->right) == -2) {
				if(getHeight(n->right->left) - getHeight(n->right->right) == 1) {
					rotateRight(n->right);
				}
				rotateLeft(n);
			}
			resetHS(n);
			n = n->parent;
		}
	}

	void rotateLeft(Node *v);
	void rotateRight(Node *v);

	Node* minimum(Node *x) const;
	Node* maximum(Node *x) const;

	/*
	void printTreeRecur(Node *n) {
	if(n!=nullptr) {
	std::cout << "(";
	printTreeRecur(n->left);
	std::cout << n->data << "-" << n->height << " ";
	printTreeRecur(n->right);
	std::cout << ")";
	}
	}

	*/
	bool verifyBal(Node *n) {
		if(n) {
			if(std::abs(getHeight(n->left)- getHeight(n->right)) > 1) return false;
			else return verifyBal(n->left) && verifyBal(n->right);
		} else return true;
	}

public:
	/*
	void printTree() {
	printTreeRecur(root);
	std::cout << std::endl;
	}
	*/
	bool isBalanced() {
		return verifyBal(root);
	}

	iterator getIterator(unsigned int index) {
		auto ret = begin();
		for(unsigned int i = 0; i < index; ++i) {
			++ret;
		}
		return ret;
	}

	const_iterator getConstIterator(unsigned int index) {
		auto ret = begin();
		for(unsigned int i = 0; i < index; ++i) {
			++ret;
		}
		return ret;
	}
};

template<typename T>
void OrderStat<T>::rotateLeft(Node* v) {
	Node *r = v->right; 

	v->right = r->left;
	if(r->left) r->left->parent= v;
	r->parent = v->parent;
	if(!(v->parent)) root = r;
	else if(v == v->parent->left) v->parent->left = r;
	else v->parent->right = r;
	r->left = v;
	v->parent = r;

	resetHS(v);
	resetHS(r);
}

template<typename T>
void OrderStat<T>::rotateRight(Node* v) {
	Node *l = v->left; 

	v->left = l->right;
	if(l->right) l->right->parent= v;
	l->parent = v->parent;
	if(!(v->parent)) root = l;
	else if(v == v->parent->right) v->parent->right = l;
	else v->parent->left = l;
	l->right = v;
	v->parent = l;

	resetHS(v);
	resetHS(l);
}

template<typename T>
typename OrderStat<T>::Node* OrderStat<T>::minimum(Node* x) const {
	while(x->left) x = x->left;
	return x;
}

template<typename T>
typename OrderStat<T>::Node* OrderStat<T>::maximum(Node* x) const {
	while(x->right) x = x->right;
	return x;
}

template<typename T>
typename OrderStat<T>::value_type &OrderStat<T>::front() { return minimum(root)->data; }

template<typename T>
const  typename OrderStat<T>::value_type &OrderStat<T>::front() const { return minimum(root)->data; }

template<typename T>
typename OrderStat<T>::value_type &OrderStat<T>::back() { return maximum(root)->data; }

template<typename T>
const typename OrderStat<T>::value_type &OrderStat<T>::back() const { return maximum(root)->data; }

template<typename T>
typename OrderStat<T>::iterator OrderStat<T>::insert(const_iterator position, const value_type& val) {
	Node* rover = position.n;
	Node* prev = nullptr;
	Node* n;
	if(!root) {
		n = new Node(nullptr);
		n->data = val;
		root = n;
	} else if(position == end()) {
		push_back(val);
		return iterator(position.n);  //position or ++position
		//} else if(!rover) {
		//	n = new Node(rover);
		//	n->data = val;
		//	//if((--position).n)
		//	Node *x = (--position).n; 
		//	n->parent = x;//(--position).n;
		//	n->parent->right = n;
	} else if(rover->left) {
		rover = rover->left;
		while(rover->right) {
			prev = rover;
			rover = rover->right; 
		}
		n = new Node(rover);
		n->data = val;
		rover->right = n;
	} else {
		n = new Node(rover);
		n->data = val;
		rover->left = n;
	}	
	checkBalance(n);
	return iterator(n);
}

template<typename T>
typename OrderStat<T>::iterator OrderStat<T>::erase(const_iterator position) {
	const_iterator tmp = position;
	++tmp;
	removeNode(position.n);
	return iterator(tmp.n);
}

template<typename T>
void OrderStat<T>::clear() {
	deleteAll(root);
	root = nullptr;
}
/* version 1
template<typename T>
typename OrderStat<T>::value_type &OrderStat<T>::operator[](int index) {
unsigned int i = index;
Node *rover = root;
unsigned int comp = getSize(rover->left)+1;
//Node *prev = nullptr;
while(rover) {
comp = getSize(rover->left)+1;
if(i == comp-1) return rover->data;
else if(comp == 1) return rover->right->data;    //??????
//prev = rover;
else if(i < comp) rover = rover->left;
else {
i -= comp;
rover = rover->right;
}
}
}
*/
template<typename T>
typename OrderStat<T>::value_type &OrderStat<T>::operator[](int index) {
	unsigned int i = index;
	Node *rover = root;
	unsigned int comp;
	//Node *prev = nullptr;
	while(rover) {
		comp = getSize(rover->left);
		if(i == comp) return rover->data;
		//prev = rover;
		else if(i < comp) rover = rover->left;
		else {
			i -= (comp+1);
			rover = rover->right;
		}
	}
}

template<typename T>
void OrderStat<T>::push_front(const value_type& val) {
	if(root) {
		Node *oldFront = minimum(root);
		Node *newFront = new Node(oldFront);
		newFront->data = val;
		oldFront->left = newFront;
		checkBalance(newFront);
	} else {
		Node *newFront = new Node(nullptr);
		newFront->data = val;
		root = newFront;
		checkBalance(root);
	}
}

template<typename T>
void OrderStat<T>::pop_front() { erase(begin()); }

template<typename T>
void OrderStat<T>::push_back(const value_type& val) {
	if(root) {
		Node* oldBack = maximum(root);
		Node* newBack = new Node(oldBack);
		newBack->data = val;
		oldBack->right = newBack;
		checkBalance(newBack);
	} else {
		Node* newBack = new Node(nullptr);
		newBack->data = val;
		root = newBack;
		checkBalance(root);
	}
}

template<typename T>
void OrderStat<T>::pop_back() { erase(--end()); }

template<typename T>
bool OrderStat<T>::operator==(const OrderStat<T>& rhs) const {
	if(size()!=rhs.size()) return false;
	auto iter = begin();
	for(auto &p:rhs) {
		if(p != *iter) return false;
		++iter;
	}
	return true;
}

template<typename T>
bool OrderStat<T>::operator!=(const OrderStat<T>& rhs) const { return !(*this==rhs); }

#endif