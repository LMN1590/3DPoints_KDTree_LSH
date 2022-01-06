#include "main.h"


float calDis(Point3D* a,Point3D* b){
	float total=0;
	for(int i=0;i<3;i++){
		float temp=a->coor[i]-b->coor[i];
		total+=temp*temp;
	}
	return sqrt(total);
}
Point3D* randomGen(){
	random_device rd;
	default_random_engine generator(rd());
	uniform_int_distribution<int> distribution(0,100000);
	float x=(float)distribution(generator)/1000;
	float y=(float)distribution(generator)/1000;
	float z=(float)distribution(generator)/1000;
	return new Point3D(x,y,z);
}
bool compx(Point3D* a,Point3D* b){
	return a->coor[0]<b->coor[0];
}
bool compy(Point3D* a,Point3D* b){
	return a->coor[1]<b->coor[1];
}
bool compz(Point3D* a,Point3D* b){
	return a->coor[2]<b->coor[2];
}
///////////////////////////////////////////////////////////
//Point Functions//////////////////////////////////////////
void Point3D::print(){
	cout<<this->coor[0]<<" "<<this->coor[1]<<" "<<this->coor[2]<<" "<<endl;
}
///////////////////////////////////////////////////////////
//Tree Functions//////////////////////////////////////////
TreeNode::~TreeNode(){
	if(this->left!=nullptr) delete this->left;
	if(this->right!=nullptr) delete this->right;
	delete this->data;
}
void TreeNode::insert(TreeNode*& root,Point3D* input,int level){
	if(root==nullptr){
		root=new TreeNode(input);
		return;
	}
	level=level%3;
	if(root->data->coor[level]>input->coor[level]){
		insert(root->left,input,level+1);
	}
	else{
		insert(root->right,input,level+1);
	}
}
void TreeNode::alt_searchNN(Point3D* input,TreeNode* root,int level,Point3D*& bestP,float& bestL){
	if(root==nullptr)return;
	float cur=calDis(input,root->data);
	if(root->left==nullptr && root->right==nullptr){
		if(bestP==nullptr || cur<bestL){
			bestP=root->data;
			bestL=cur;
		}
		return;
	}
	level=level%3;
	if(root->data->coor[level]>input->coor[level]){
		if(root->left!=nullptr)alt_searchNN(input,root->left,level+1,bestP,bestL);
		else alt_searchNN(input,root->right,level+1,bestP,bestL);

		if(bestP==nullptr || cur<bestL){
			bestL=cur;
			bestP=root->data;
		}

		if(bestL>abs(root->data->coor[level]-input->coor[level])){
			if(root->left!=nullptr && root->right!=nullptr){
				alt_searchNN(input,root->right,level+1,bestP,bestL);
			}
		}
		
	}
	else{
		if(root->right!=nullptr)alt_searchNN(input,root->right,level+1,bestP,bestL);
		else alt_searchNN(input,root->left,level+1,bestP,bestL);

		if(bestP==nullptr || cur<bestL){
			bestL=cur;
			bestP=root->data;
		}

		if(bestL>abs(root->data->coor[level]-input->coor[level])){
			if(root->left!=nullptr && root->right!=nullptr){
				alt_searchNN(input,root->left,level+1,bestP,bestL);
			}
		}
	}
	
}
Point3D* TreeNode::searchNN(Point3D* input,TreeNode* root,int level){
	if(root==nullptr)return nullptr;
	float cur=calDis(input,root->data);
	if(root->left==nullptr && root->right==nullptr) return root->data;
	level=level%3;
	Point3D* best=nullptr;
	if(root->data->coor[level]>input->coor[level]){
		if(root->left!=nullptr) {
			Point3D* leftPoint=searchNN(input,root->left,level+1);
			float leftDis=calDis(input,leftPoint);
			if(leftDis<abs(root->data->coor[level]-input->coor[level])) best=leftPoint;
			else{
				if(root->right==nullptr)best=leftPoint;
				else{
					Point3D* rightPoint=searchNN(input,root->right,level+1);
					float rightDis=calDis(input,rightPoint);
					if(leftDis<rightDis) best=leftPoint;
					else best=rightPoint;
				}
			}
		}
		else{
			Point3D* rightPoint=searchNN(input,root->right,level+1);
			best=rightPoint;
		}
	}
	else{
		if(root->right!=nullptr) {
			Point3D* rightPoint=searchNN(input,root->right,level+1);
			float rightDis=calDis(input,rightPoint);
			if(rightDis<abs(root->data->coor[level]-input->coor[level])) best=rightPoint;
			else{
				if(root->left==nullptr)best=rightPoint;
				else{
					Point3D* leftPoint=searchNN(input,root->left,level+1);
					float leftDis=calDis(input,leftPoint);
					if(leftDis<rightDis) best=leftPoint;
					else best=rightPoint;
				}
			}
		}
		else{
			Point3D* leftPoint=searchNN(input,root->left,level+1);
			best=leftPoint;
		}
	}
	if(cur<calDis(best,input)) return root->data;
	else return best;
}
void TreeNode::buildTree(TreeNode*& root,vector<Point3D*>& base,int start, int end,int level){
	if(!(start<end)) return;
	level=level%3;
	switch(level){
		case 0:
		sort(base.begin()+start,base.begin()+end,compx);
		break;
		case 1:
		sort(base.begin()+start,base.begin()+end,compy);
		break;
		case 2:
		sort(base.begin()+start,base.begin()+end,compz);
		break;
	}
	int median=(start+end)/2;
	this->insert(root,base[median],0);
	this->buildTree(root,base,start,median,level+1);
	this->buildTree(root,base,median+1,end,level+1);
}
///////////////////////////////////////////////////////////