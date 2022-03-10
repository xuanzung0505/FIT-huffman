/*
file node.cpp the hien ro cac ham, thuoc tinh cua file node.h

Ho ten: Truong Xuan Dung
MSV   : B18DCCN103
Lop   : E18CN01
Nhom  : 10
STT   : 05
*/

#include "node.h"
#include <bits/stdc++.h>

node::node(float val){
	this->val = val;
	this->left = NULL;
	this->right = NULL;
	this->l = NULL;
	this->r = NULL;
	this->isleaf = false;
	this->data = NULL;
}
