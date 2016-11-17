#include <stdio.h>
#include <iostream>
#include <algorithm>
#include <stdlib.h>
#include <queue>
#include <vector>
#include <fstream>
#include <bitset>

using namespace std;

#define MARKER -1

struct tree_node{
    char c;
    tree_node *left;
    tree_node *right;

    tree_node(char x) : c(x), left(NULL), right(NULL) {}
};

vector<bool> encoding[128];
tree_node* tree_node_ptr;

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

void print_tree(tree_node* root){
	if(root == NULL) return;
	if(root->left == NULL && root->right == NULL) printf("%c ",root->c);
	else printf("O ");
	print_tree(root->left);
	print_tree(root->right);
}

void p(tree_node* root,vector<int> vec)
{
	if(root->left == NULL && root->right == NULL)
	{
		printf("%c - ",root->c);
		for(int i=0;i<vec.size();i++)
		{
			cout<<vec[i];
		}
		cout<<endl;
		return;
	}

	vec.push_back(0);
	p(root->left,vec);
	vec.pop_back();
	vec.push_back(1);
	p(root->right,vec);
	vec.pop_back();
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
	// reading the file
	
	string inp;
	cout<<"\nEnter the name of the file to be decompressed: ";
	cin>>inp;

	ifstream ifs;
	ofstream ofs;

	string temp = inp;
	inp.resize(inp.size()-8);
	inp = inp + "_tree.txt";

	int count,remain;
	tree_node* root = NULL;
	FILE* fp = fopen(inp.c_str(), "r");
	fscanf(fp, "%d %d\n", &count, &remain);
	deSerialize(root, fp);
	fclose(fp);

	string output,binary;
	ifs.open(temp.c_str(),ifstream::in | ifstream::binary);
	int te = ifs.get();
	int count1 = 0;

	while(ifs.good())
	{
		binary = bitset<8>(te).to_string(); //to binary
		if(count1 == count)
		{
			binary = binary.substr(8-remain,remain);
		}
		
		output = output.append(binary);
		// cout<<binary<<"\n";
		te = ifs.get();
		count1++;
	}
	ifs.close();

	// cout<<output.size()<<endl;
	//cout<<output<<endl;

	temp.resize(temp.size()-8);
	temp = temp + "_final.txt";

	ofs.open(temp.c_str(),ofstream::out | ofstream::binary);

	int i=0;
	int prev = 0;
	tree_node* root1 = root;

	while(i <= output.size())
	{
		if(root1->left == NULL && root1->right == NULL)
		{
			ofs << root1->c;

			root1 = root;
			// cout<<output.substr(prev,i-prev)<<endl;
			prev=i;
			if(i == output.size())	break;
			else	continue;
		}

		if(output[i] == '0')
		{
			root1 = root1->left;
		}
		else if(output[i] == '1')
		{
			root1 = root1->right;
		}
		i++;
	}
	ofs.close();

	return 0;
}