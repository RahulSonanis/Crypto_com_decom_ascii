#include <stdio.h>
#include <iostream>
#include <algorithm>
#include <stdlib.h>
#include <queue>
#include <vector>
#include <fstream>

using namespace std;
vector<bool> encoding[128];


struct tree_node{
	struct tree_node* left;
	struct tree_node* right;
	char c;
};


struct heap_node{
	struct tree_node* root;
	int weight;

	bool operator<(const struct heap_node& rhs) const
    {
        return weight > rhs.weight;
    }
};

void print_tree(struct tree_node* root){
	if(root == NULL) return;
	if(root->left == NULL && root->right == NULL) printf("%c ",root->c);
	else printf("O ");
	print_tree(root->left);
	print_tree(root->right);
}

void print_encoding(vector<bool> bitstream){
	vector<bool>::iterator it;
	for(it=bitstream.begin() ; it < bitstream.end(); it++){
		if((*it) == false) printf("0");
		else printf("1");
	}
}

void get_encoding(struct tree_node* root, vector<bool> bitstream){
	if(root == NULL) return;
	if(root->left == NULL && root->right == NULL){
		encoding[root->c] = bitstream;
		printf("%c - ",root->c);
		print_encoding(bitstream);
		printf("\n");
	}
	bitstream.push_back(false);
	get_encoding(root->left,bitstream);
	bitstream.pop_back();
	bitstream.push_back(true);
	get_encoding(root->right,bitstream);
}

priority_queue<heap_node> heap;



int main(){

	char input[1000];
	int weights[150];
	int i;
	struct tree_node* tree_node_ptr;
	struct heap_node temp_heap_node,min1,min2;


	ifstream fin;
	fin.open("input.txt", ios::in);
	i = 0;
	while (!fin.eof() ) {
		fin.get(input[i++]);
	}
	input[i] = '\0';

	//scanf("%s",input);

	printf("%s\n",input);

	for(i=0;i<128;i++) weights[i] = 0;
	i = 0;
	while(input[i]){
		weights[input[i++]]++;
	}

	for(i=0;i<128;i++){
		if(weights[i] > 0) {
			tree_node_ptr = (struct tree_node*)(malloc(sizeof(struct tree_node)));
			tree_node_ptr->left = NULL;
			tree_node_ptr->right = NULL;
			tree_node_ptr->c = i;

			temp_heap_node.root = tree_node_ptr;
			temp_heap_node.weight = weights[i];
			heap.push(temp_heap_node);

		}
	}


	while(heap.size() > 1){
		min1 = heap.top();
		heap.pop();
		min2 = heap.top();
		heap.pop();
		tree_node_ptr = (struct tree_node*)(malloc(sizeof(struct tree_node)));
		tree_node_ptr->left = min1.root;
		tree_node_ptr->right = min2.root;
		temp_heap_node.root = tree_node_ptr;
		temp_heap_node.weight = min1.weight + min2.weight;
		heap.push(temp_heap_node);
	}

	tree_node_ptr = heap.top().root;
	print_tree(tree_node_ptr);
	cout<<endl;
	vector<bool> bitstream;
	get_encoding(tree_node_ptr,bitstream);



	return 0;
}