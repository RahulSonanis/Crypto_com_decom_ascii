#include <stdio.h>
#include <iostream>
#include <algorithm>
#include <stdlib.h>
#include <queue>
#include <vector>
#include <bitset>
#include <fstream>

using namespace std;

#define MARKER -1

struct tree_node{
    char c;
    tree_node *left;
    tree_node *right;

    tree_node(char x) : c(x), left(NULL), right(NULL) {}
};


struct heap_node{
	tree_node* root;
	int weight;

	bool operator<(const struct heap_node& rhs) const
    {
        return weight > rhs.weight;
    }
};

vector<bool> encoding[128];
int weights[128];
tree_node* tree_node_ptr;
struct heap_node temp_heap_node,min1,min2;

void print_tree(tree_node* root){
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

string get_encoding(char c)
{
	string ans;
	vector<bool>::iterator it;
	for(it=encoding[c].begin() ; it != encoding[c].end(); it++)
	{
		if((*it) == false) ans.push_back('0');
		else ans.push_back('1');
	}
	return ans;
}

void get_encoding(tree_node* root, vector<bool> bitstream){
	if(root == NULL) return;
	if(root->left == NULL && root->right == NULL){
		encoding[root->c] = bitstream;
		// printf("%c - ",root->c);
		// print_encoding(bitstream);
		// printf("\n");
	}
	bitstream.push_back(false);
	get_encoding(root->left,bitstream);
	bitstream.pop_back();
	bitstream.push_back(true);
	get_encoding(root->right,bitstream);
}

priority_queue<heap_node> heap;

void build_tree()
{

	for(int i=0;i<128;i++)
	{
		if(weights[i] > 0) 
		{
			tree_node_ptr = new tree_node(i);

			temp_heap_node.root = tree_node_ptr;
			temp_heap_node.weight = weights[i];
			heap.push(temp_heap_node);
		}
	}

	while(heap.size() > 1)
	{
		min1 = heap.top();
		heap.pop();
		min2 = heap.top();
		heap.pop();
		tree_node_ptr = new tree_node(128);
		tree_node_ptr->left = min1.root;
		tree_node_ptr->right = min2.root;
		temp_heap_node.root = tree_node_ptr;
		temp_heap_node.weight = min1.weight + min2.weight;
		heap.push(temp_heap_node);
	}

	tree_node_ptr = heap.top().root;
	vector<bool> bitstream;
	get_encoding(tree_node_ptr,bitstream);
}

void serialize(tree_node *root, FILE *fp)
{
    if (root == NULL)
    {
        fprintf(fp, "%d ", MARKER);
        return;
    }

    fprintf(fp, "%d ", root->c);
    serialize(root->left, fp);
    serialize(root->right, fp);
}


void deSerialize(tree_node *&root, FILE *fp)
{
    int val;
    if (!fscanf(fp, "%d ", &val) || val == MARKER)
       return;
 
    root = new tree_node((char)val);
    deSerialize(root->left, fp);
    deSerialize(root->right, fp);
}

int main()
{
	for(int i=0;i<128;i++) weights[i] = 0;

	// reading the file
	
	string inp;
	cout<<"Enter the name of the file to be compressed: ";
	cin>>inp;	

	ifstream ifs;
	ofstream ofs;

	ifs.open(inp.c_str(),ifstream::in | ifstream::binary);
	char te = ifs.get();
	while(ifs.good())
	{
		weights[te]++;
		te = ifs.get();
	}
	ifs.close();

	// building the huffman encoding scheme tree
	build_tree();

	// making compressed file with name filename_com.txt
	string temp = inp;
	inp.resize(inp.size()-4);
	inp = inp + "_com.txt";

	ifs.open(temp.c_str(),ifstream::in | ifstream::binary);
	ofs.open(inp.c_str(),ofstream::out | ofstream::binary);

	string anss;
	te = ifs.get();
	int count = 0;
	int ss = 0;

	while(ifs.good())
	{
		//cout<<get_encoding(te)<<endl;
		anss += get_encoding(te);
		if(anss.size() >= 8)
		{
			// cout<<anss.substr(0,7);
			bitset<8> foo (anss.substr(0,8));
			//cout<<(char)foo.to_ulong()<<endl;
			ss += 8;
			ofs << (char)foo.to_ulong();
			anss.erase(0,8);
			count++;
		}

		te = ifs.get();
	}

	int remain = 0;
	if(anss.size() <= 8)
	{
		remain += anss.size();
		ss += anss.size();
		// cout<<anss<<endl;
		bitset<8> foo (anss);
		ofs << (char)foo.to_ulong();
	}
	ifs.close();
	ofs.close();
	// compressed file made
	// cout<<ss<<endl;

	// serializing the huffman encoding tree with name filename_tree.txt
	temp.resize(temp.size()-4);
	inp = temp + "_tree.txt";

	FILE *fp = fopen(inp.c_str(), "w");
	if (fp == NULL)
	{
	    puts("Could not open file");
	    return 0;
	}
	fprintf(fp, "%d %d\n", count, remain);
	// print_tree(tree_node_ptr);
	serialize(tree_node_ptr, fp);
	fclose(fp);

	return 0;
}



 
