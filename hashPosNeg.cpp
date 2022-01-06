#include "tree.cpp"
using namespace std;
void generateHashFunc(vector<vector<float>>& base){
	random_device rd;
	default_random_engine generator(rd());
	uniform_real_distribution<float> distribution(-10,10);
	uniform_real_distribution<float> distribution2(-1000,1000);
	for(vector<float>& a:base){
		for(int i=0;i<4;i++){
			if(i==4)a.push_back(distribution2(generator));
			else a.push_back(distribution(generator));
		}
	}
}


void prepareKHashFuncs(vector<vector<vector<float>>>& base,int k){
    for(int i=0;i<base.size();i++){
		base[i].resize(k);
		generateHashFunc(base[i]);
	}
}

int pseudoHash(vector<vector<float>>& base, Point3D* input){
	int res=0;
	float sum=0;
	for(vector<float>& a:base){
		for(int i=0;i<3;i++){
			sum+=a[i]*input->coor[i];
		}
		sum+=a[3];
		res=res<<1;
		if(sum>=0) res++;
		sum=0;
	}
	return res;
}

void buildHash(vector<vector<list<Point3D*>>>& mainHashTable,vector<vector<vector<float>>>& base2,vector<Point3D*>& points,int k){
	prepareKHashFuncs(base2,k);
	for(int i=0;i<points.size();i++){
		for(int j=0;j<mainHashTable.size();j++){
			long long hashIndex=pseudoHash(base2[j],points[i])%(int)1e+5;
			if(hashIndex<0)hashIndex+=(int)1e+5;
			mainHashTable[j][hashIndex].push_back(points[i]);
		}
	}
}

Point3D* findNN(vector<vector<list<Point3D*>>>& mainHashTable,vector<vector<vector<float>>>& base2,Point3D* input){
    Point3D* bestP=nullptr;
	float bestL=0;
	for(int i=0;i<mainHashTable.size();i++){
		long long hashIndex=pseudoHash(base2[i],input)%(int)1e+5;
		if(hashIndex<0)hashIndex+=(int)1e+5;
		int check=0;
		for(Point3D*& a:mainHashTable[i][hashIndex]){
			check++;
			float len=calDis(input,a);
			if(bestP==nullptr || bestL>len){
				bestP=a;
				bestL=len;
			}
			if(check>1e3)break;
		}
	}
    return bestP;
}

Point3D* linear(vector<Point3D*> points,Point3D* input){
	Point3D* res=nullptr;
	float len=0;
	for(Point3D* a:points){
		float temp=calDis(a,input);
		if(res==nullptr || len>temp){
			res=a;
			len=temp;
		}
	}
	return res;
}