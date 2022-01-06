#include "hashPosNeg.cpp"
#define HASH_MOD 4294967291
using namespace std;

void generateHashFunctions2(vector<vector<int>>& hashFuncs, int k){
    random_device rd;
	default_random_engine generator(rd());
	normal_distribution<float> distribution(0,100);
    hashFuncs.resize(k,vector<int>(0));
    for(vector<int>& a:hashFuncs){
        for(int i=0;i<3;i++){
            a.push_back(distribution(generator));
		}
    }
}

void generateHashTable(vector<vector<int>>& outerHash,int L,int k){
    srand(time(NULL));
    outerHash.resize(L,vector<int>(0));
    for(int i=0;i<L;i++){
        for(int j=0;j<k;j++){
            outerHash[i].push_back(j);
        }
        for(int j=0;j<k;j++){
            int temp=rand()%k;
            swap(outerHash[i][temp],outerHash[i][j]);
        }
    }
}

void buildHash(vector<vector<list<Point3D*>>>& mainHashTable_LSH,vector<vector<int>>& outerHash, vector<vector<int>>& hashFuncs, Point3D* points,int L,int k,int mau_LSH,long int size,vector<float>& t){
    for(int i=0;i<L;i++){
        string hashStr="";
        float sum=0;
        for(int j=0;j<k;j++){
            for(int plane=0;plane<3;plane++){
                sum=sum+hashFuncs[outerHash[i][j]][plane]*(points->coor[plane]);
            }
            sum=sum+t[j];
            sum=floor(sum/(float)mau_LSH);
            hashStr.append(to_string(sum));
            sum=0;
        }
        hash<string> hash;
        long long unsigned int pos=(hash(hashStr)%HASH_MOD)%(size/2);
        mainHashTable_LSH[i][pos].push_back(points);
    }
}

void buildLSH(vector<vector<list<Point3D*>>>& mainHashTable_LSH,vector<vector<int>>& outerHash, vector<vector<int>>& hashFuncs, vector<Point3D*>& points,int L,int k,int mau_LSH,long int size){
    random_device rd;
	default_random_engine generator(rd());
    uniform_real_distribution<float> distribution(0,100);
    vector<float> t;
    for(int i=0;i<k;i++){
        t.push_back(distribution(generator));
    }
    generateHashFunctions2(hashFuncs,k);
    generateHashTable(outerHash,L,k);
    for(Point3D*& a:points){
        buildHash(mainHashTable_LSH,outerHash,hashFuncs,a,L,k,mau_LSH,size,t);
    }
    
}

Point3D* findNNLSH(vector<vector<list<Point3D*>>>& mainHashTable_LSH,vector<vector<int>>& outerHash, vector<vector<int>>& hashFuncs, Point3D* input,int L,int k,int mau_LSH,int lim,long int size){
    Point3D* bestP=nullptr;
	float bestL=0;
    
    for(int i=0;i<L;i++){
        string hashStr="";
        for(int j=0;j<k;j++){
            float sum=0;
            for(int plane=0;plane<3;plane++){
                sum=sum+hashFuncs[outerHash[i][j]][plane]*(input->coor[plane]);
            }
            sum=sum+hashFuncs[outerHash[i][j]][3];
            sum=floor(sum/(float)mau_LSH);
            hashStr.append(to_string(sum));
        }
        hash<string> hash;
        long long unsigned int pos=hash(hashStr)%HASH_MOD;
        pos=pos%(size/2);
        int check=0;
        for(Point3D*& hashPoint:mainHashTable_LSH[i][pos]){
            //check++;
            float len=calDis(hashPoint,input);
			if(bestP==nullptr || bestL>len){
				bestP=hashPoint;
				bestL=len;
			}
            if(check>lim)break;
        }
    }
    return bestP;
}