#include <iostream>
#include <random>
#include <math.h>
#include <algorithm>
#include <vector>
#include <list>
#include <chrono>
#include <string>
#include <functional>
#include <climits>
#include <fstream>
using namespace std;

class Point3D{
public:
	double* coor;
	Point3D(double x,double y,double z):coor(new double[3]{x,y,z}){}
	void print();
	~Point3D(){
		delete[] coor;
	}
};

class TreeNode{
public:
	Point3D* data;
	TreeNode* left;
	TreeNode* right;
	TreeNode(Point3D* data):data(data),left(nullptr),right(nullptr){}
	void insert(TreeNode*& root,Point3D* input,int level);
	Point3D* searchNN(Point3D* input,TreeNode* root,int level);
	void alt_searchNN(Point3D* input,TreeNode* root,int level,Point3D*& bestP,float& bestL);
	void buildTree(TreeNode*& root,vector<Point3D*>& base,int start,int end,int level);
	~TreeNode();
};
