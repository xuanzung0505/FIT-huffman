/*
file main.cpp de chay chuong trinh

Ho ten: Truong Xuan Dung
MSV   : B18DCCN103
Lop   : E18CN01
Nhom  : 10
STT   : 05
*/
#include <bits/stdc++.h>
#include "node.h"

using namespace std;

vector < pair<char,float*> > data; //data contains pair of (char, float*)
ofstream fo; //output file

//ENCODE DATA
string s; //text
float stat[256]; //statistics of characters in text
map<char,float*> M; //map characters to float
node* Root; //root of the tree
float average_n = 0;
float Entropy = 0;


bool sortByVal(pair<char, float*> &a,
pair<char, float*> &b){
	return (*(a.second) < *(b.second));
}

void data_in(){ //input from keyboard
	cout<<"Enter a text: ";
	getline(cin,s);	
	fo<<"Data entered: "<<s<<endl;
}

void data_file(){ //input from file
	ifstream fi;
	fi.open("input.doc",ios::binary);
	string tmp;
	while(getline(fi,tmp)){
		s+=tmp;
	}
	fi.close();
	cout<<s;
	cout<<endl;
	fo<<"Data entered from file: "<<s<<endl;
}

void data_init(){ //initialize data
	cout<<"______________________________________"<<endl;
	fo<<"________________________________________"<<endl;
	cout<<"INITIALIZING..."<<endl;
	fo<<"INITIALIZING..."<<endl;
	
	//mapping
	for(int i = 0; i < s.size(); i ++){
		stat[s[i]]++;
		M.insert(pair<char,float*>(s[i],&stat[s[i]]));
	}
//	cout<<"done mapping"<<endl;
	//push maps into vector data
	for(map<char,float*>::iterator iter = M.begin(); iter != M.end(); iter++){
		*(iter->second) /= s.size();
		data.push_back(make_pair(iter->first, iter->second));
	}
	sort(data.begin(),data.end(), sortByVal);
	cout<<"DONE!"<<endl;
	fo<<"DONE!"<<endl;
}

void data_out(){
	cout<<"Data size: "<<data.size()<<endl;
	fo<<"Data size: "<<data.size()<<endl;
	for(int i = 0; i < data.size(); i ++){
		cout<<"Data: "<<"\""<<data[i].first<<"\", Probability: "<<*(data[i].second)<<endl;
		fo<<"Data: "<<"\""<<data[i].first<<"\", Probability: "<<*(data[i].second)<<endl;
	}
}

void data_traverse(node* root, string s){ //traverse all nodes in the tree
	string s_here = s;
	if(root->isleaf){
		cout<<root->data<<": "<<s_here<<endl;
		fo<<root->data<<": "<<s_here<<endl;
		average_n += s_here.size()*root->val;
	}
	else{
		if(root->left != NULL){
			data_traverse(root->left, s_here+root->l);
		}
		if(root->right != NULL){
			data_traverse(root->right, s_here+root->r);
		}
	}
}

void data_make_tree(){ //build the tree
	cout<<"______________________________________"<<endl;
	cout<<"MAKING TREE..."<<endl;
	fo<<"________________________________________"<<endl;
	fo<<"MAKING TREE..."<<endl;
	
	node* tmp;
	vector <node*> save;
	
	//make leaf nodes
	for(int i = 0; i < data.size(); i ++){
		tmp = new node(*(data[i].second));
		tmp->isleaf = true;
		tmp->data = data[i].first;
		
//		cout<<tmp->data<<" "<<tmp->val<<" "<<tmp->left<<" "<<tmp->right<<endl;
		save.push_back(tmp);
	}
	cout<<"Initial nodes:"<<save.size()<<endl;
	fo<<"Initial nodes:"<<save.size()<<endl;
	
	//init flag
	bool flag[2*data.size()-1];
	for(int i = 0 ; i < 2*data.size()-1; i ++ ) {
		flag[i] = true;
	}
	
	//make tree
	int pos1,pos2;
	float temp_val;
	node *tmp1, *tmp2;
	
	while(save.size() < 2*data.size()-1){
		
		//find pos1
		pos1 = -1;
		temp_val = 1;
		for(int i = 0; i < save.size(); i ++){
			if(flag[i]){
				if(save[i]->val < temp_val){
					temp_val = save[i]->val;
					pos1 = i;
				}
			}
		}
		flag[pos1] = false;
		
		//find pos2
		pos2 = -1;
		temp_val = 1;
		for(int i = 0; i < save.size(); i ++){
			if(flag[i]){
				if(save[i]->val < temp_val){
					temp_val = save[i]->val;
					pos2 = i;
				}
			}
		}
		flag[pos2] = false;
		
		//push
		tmp = new node(save[pos1]->val + save[pos2]->val);
//		cout<<tmp->val<<endl;
		tmp->left = save[pos1];
		tmp->l = '0';
		tmp->right = save[pos2];
		tmp->r = '1';
		
		//dung den khi save.size() = 2*data.size()-1;
		save.push_back(tmp);
	}
	cout<<"DONE!"<<endl;
	cout<<"Nodes after process: "<<save.size()<<endl;
	fo<<"DONE!"<<endl;
	fo<<"Nodes after process: "<<save.size()<<endl;
	
	cout<<"______________________________________"<<endl;
	cout<<"TRAVERSING NODES..."<<endl;
	fo<<"______________________________________"<<endl;
	fo<<"TRAVERSING NODES..."<<endl;
	
	//root of tree
	Root = new node(0);
	Root->val = tmp->val;
	Root->l = tmp->l;
	Root->left = tmp->left;
	Root->r = tmp->r;
	Root->right = tmp->right;
	
	data_traverse(Root,"");
	cout<<"DONE!"<<endl;
	fo<<"DONE!"<<endl;
}

void data_out_2(){ //print compression result and Entropy of source
	cout<<"______________________________________"<<endl;
	cout<<"COMPRESSION RESULT:"<<endl;
	fo<<"______________________________________"<<endl;
	fo<<"COMPRESSION RESULT:"<<endl;
	
	for(int i = 0 ; i < data.size(); i ++){
		Entropy += -(*(data[i].second) * log2(*(data[i].second)));
	}
	cout<<"H(X) = "<<Entropy<<"; average n = "<<average_n<<endl;
	fo<<"H(X) = "<<Entropy<<"; average n = "<<average_n<<endl;
}


//DECODE DATA
string bits; //bits

void bits_in(){ //get bits
	cout<<"Enter a stream of bits: ";
	cin>>bits;
	fo<<"Bits entered: "<<bits<<endl;
}

void bits_decode(){ //decode bits stream
	node* tmp_node = Root;
	
	string tmp = "";
	string res = "";
	bool valid = true;
	
	for(int i = 0; i < bits.size(); i++){
//		cout<<bits[i]<<endl;
		tmp+= bits[i];
		
		if(bits[i]=='0'){
			if(tmp_node->left != NULL){
				tmp_node = tmp_node->left; //next node
				if (tmp_node->isleaf) {
					res+= tmp_node->data;	
					tmp_node = Root;
					tmp = "";
				}
				continue;
			}
		}
		else{
			if(tmp_node->right != NULL){
				tmp_node = tmp_node->right;
				if (tmp_node->isleaf) {
					res+= tmp_node->data;	
					tmp_node = Root;
					tmp = "";
				}
				continue;
			}
		}
	}
	
	if (tmp == ""){
		cout<<"Bits are valid, result: \""<<res<<"\""<<endl;
		fo<<"Bits are valid, result: \""<<res<<"\""<<endl;
	}
	else{
		cout<<"Bits are not valid, result: \""<<res<<"\""<<", bits left: \""<<tmp<<"\""<<endl;
		fo<<"Bits are not valid, result: \""<<res<<"\""<<", bits left: \""<<tmp<<"\""<<endl;
	}
}

int main(int argc, char** argv) {
	fo.open("ketquamahoa.txt");
	cout<<"***HUFFMAN ALGORITHM EXAMPLE***"<<endl;
	fo<<"***HUFFMAN ALGORITHM EXAMPLE***"<<endl;
	
	//ENCODE DATA
	cout<<"______________________________________________________________"<<endl;
	fo<<"______________________________________________________________"<<endl;
	cout<<"1) ENCODE DATA"<<endl;
	fo<<"1) ENCODE DATA"<<endl;
	data_in(); //nhap xau ki tu tu ban phim
//	data_file(); //nhap tu file input.doc
	data_init();	
	data_out();
	data_make_tree();
	data_out_2(); //final result
	
	//DECODE BITS
	cout<<"______________________________________________________________"<<endl;
	cout<<"2) DECODE BITS"<<endl;
	fo<<"______________________________________________________________"<<endl;
	fo<<"2) DECODE BITS"<<endl;
	bits_in();
	bits_decode();
	
	fo.close();
	return 0;
}
