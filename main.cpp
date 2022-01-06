#include "hashNN.cpp"

long int size=1e5;

int main(){
	ofstream file("data.csv");
	//Lưu ý: Cần phải khai báo lại tất cả các giá trị (đặc biệt là các bảng Hash cũng như hàm Hash) khi thay đổi tập mẫu.
	//Caution: It's important to redeclared all variables especially the one containing the Hash Table and Hash Functions when changing the dataset.
	//KD-Tree
	TreeNode* root=nullptr;
	/////////

	//PosNeg Hashing
	int L_PosNeg=3;
	int k_PosNeg=50;
	vector<vector<list<Point3D*>>> mainHashTable_PosNeg(L_PosNeg,vector<list<Point3D*>>((int)1e+5));
	vector<vector<vector<float>>> base2(L_PosNeg);
	////////////////

	//LSH Hashing
	int L_LSH=3;
	int k_LSH=4;
	int mau_LSH=5*100;
	int lim=10;
	vector<vector<list<Point3D*>>> mainHashTable_LSH(L_LSH,vector<list<Point3D*>>(size/2));
	vector<vector<int>> outerHash;
	vector<vector<int>> hashFuncs_LSH;
	/////////////

	//Points//
	vector<Point3D*> points;
	for(int i=0;i<size;i++){
		points.push_back(randomGen());
	}
	vector<Point3D*> testPoint;
	for(int i=0;i<1e+4;i++){
		testPoint.push_back(randomGen());
	}

	//Build//
	buildHash(mainHashTable_PosNeg,base2,points,k_PosNeg);
	buildLSH(mainHashTable_LSH,outerHash,hashFuncs_LSH,points,L_LSH,k_LSH,mau_LSH,size);
	root->buildTree(root,points,0,points.size(),0);


	//Searching////
	//KD-Tree//
	for(int i=0;i<100;i++){
		for(int j=0;j<100;j++){
			Point3D* bestP=nullptr;
			float bestL=0;
			root->alt_searchNN(testPoint[i*100+j],root,0,bestP,bestL);
		}
	}
	//Hash cơ bản
	for(int i=0;i<100;i++){
		for(int j=0;j<100;j++){
			Point3D* res=findNN(mainHashTable_PosNeg,base2,testPoint[100*i+j]);
		}
	}
	//Hash LSH
	for(int i=0;i<100;i++){
		for(int j=0;j<100;j++){
			Point3D* res2=findNNLSH(mainHashTable_LSH,outerHash,hashFuncs_LSH,testPoint[100*i+j],L_LSH,k_LSH,mau_LSH,lim,size);
		}
	}

	//Delete//
	for(int i=0;i<1e4;i++){
		delete testPoint[i];
	}
	delete root;
	points.clear();
	mainHashTable_LSH.clear();
	mainHashTable_PosNeg.clear();
	base2.clear();
	outerHash.clear();
	hashFuncs_LSH.clear();
}