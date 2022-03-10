/*
file node.h chua cac ten ham, thuoc tinh class node

Ho ten: Truong Xuan Dung
MSV   : B18DCCN103
Lop   : E18CN01
Nhom  : 10
STT   : 05
*/
#ifndef NODE_H
#define NODE_H

class node
{
	public:
		float val;
		node *left;
		node *right;
		char l;
		char r;
		bool isleaf;
		char data;
		node(float val);
	protected:
};

#endif
