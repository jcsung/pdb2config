//pdb2config.cpp
//By Jeffrey Sung
//Last updated: 
//Converts PDB to DL_POLY CONFIG
//Important note: Has no way to handle periodic boundary condictions
//Simply assumes levcfg = imcon = 0
//Compiles with g++
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <iomanip>
#include <math.h>

using namespace std;

struct atom{
	int serial;
	int resSeq;
	double x, y, z;
	string before;
	string after;
	string name;
	
	friend ostream &operator<<(ostream &os, const atom &at);
	friend bool operator==(atom &a1, atom &a2);
};

struct residue{
	vector <atom> atoms;
	double theta; //angle
	char axis; // possible values: 'x', 'y', or 'z'
};

double str2double(const string &str);
int str2int(const string & str);
atom readatom(const string & str);
string trim(const string & str);
vector <atom> split(const string & str);


double str2double(const string &str){
	stringstream ss(str);
	double d;
	if (!(ss>>d)) d=0;
	return d;
}
int str2int(const string &str){
	stringstream ss(str);
	int i;
	if (!(ss>>i)) i=0;
	return i;
}
atom readatom(const string &str){
	atom at;
	at.before=str.substr(0,30);
	at.after=str.substr(54,26);
	at.name=str.substr(12,4);
	at.serial=str2int(str.substr(6,5));
	at.resSeq=str2int(str.substr(22,4));
	at.x=str2double(str.substr(30,8));
	at.y=str2double(str.substr(38,8));
	at.z=str2double(str.substr(46,8));
	return at;
}

string trim(const string & strorig){
	return strorig;
}

ostream &operator<<(ostream &os, const atom &at){
/*
	os<<at.before;
	os<<setiosflags(ios_base::right|ios_base::fixed);
	os<<setw(8)<<setprecision(3)<<at.x;
	os<<setw(8)<<setprecision(3)<<at.y;
	os<<setw(8)<<setprecision(3)<<at.z;
	os<<at.after;
*/
	os<<setiosflags(ios_base::right|ios_base::fixed);
	os<<at.name<<setw(16)<<at.serial<<endl;
	os<<setw(16)<<setprecision(9)<<at.x<<"    ";
	os<<setw(16)<<setprecision(9)<<at.y<<"    ";
	os<<setw(16)<<setprecision(9)<<at.z;


	return os;
}

bool operator==(atom &a1, atom &a2){
	return a1.serial==a2.serial;
}

vector <atom> split(const string & str){
	vector <atom> ret(1);
	atom at;
	stringstream ss(str);
	string s2;
	int i=0;
	for (; ss>>s2;){
		at.serial=str2int(s2);
		ret[i++]=at;
		if (i==ret.size()) ret.resize(2*i);
	}
	ret.resize(i);
	return ret;
}

int main(int argc,char *argv[]){
	if (argc < 3){
		cout<<"Syntax: "<<argv[0]<<" pdb_in config_out"<<endl;
	}
	else{
		int which=0, i=1, j=0;
		atom curat;
		residue curesid;
		string line, s2;
		double cur_x, cur_y, cur_z;

		ifstream infile(argv[1]);
		ofstream outfile(argv[2]);

		stringstream ss("");

		outfile<<argv[1]<<endl;
			
		while (getline(infile,line)){
			if (line.find("ATOM")==string::npos&&line.find("HETATM")==string::npos){
			//	ss<<line<<endl;
				continue;
			}
			curat=readatom(line);
			ss<<curat<<endl;	
			i++;
		}
		outfile<<setiosflags(ios_base::right|ios_base::fixed);
		outfile<<setw(10)<<0<<setw(10)<<0<<setw(10)<<i-1<<setw(15)<<setprecision(6)<<0.0<<endl;
		outfile<<ss.str();
		infile.close();
		outfile.close();
	
	}
	return 0;
}
