#include<iostream>
#include<fstream>
#include <string>
#include <map>
#include<queue>
#include <vector>
#include<cstring>
#include<bits/stdc++.h>
#include<conio.h>
#include "MD5.h"
#include "MD5.cpp"

using namespace std;
int constructFileHash(string name,int freq=1)
{
    string f1="I:\\Studies\\ADS package\\friends\\"+name+".txt";
    string f2="I:\\Studies\\ADS package\\hashes\\"+name+"_hash.txt";
    char fileread[f1.size()+1],filewrite[f2.size()+1];
    strcpy(fileread,f1.c_str());
    strcpy(filewrite,f2.c_str());
    ifstream file1(fileread, ios::in);
    ofstream file2(filewrite,ios::out);
    int frequency=freq;
    int fileHash[128]={0};
    std::string word;
    while(file1>>word)
    {
        int hash[128];
        MD5(word).hexdigest(hash);
        for (int i=0; i< 128; i++)
        {
            hash[i] = (hash[i] > 0) ? hash[i]*frequency : -1;
            fileHash[i] += hash[i];
        }

    }

    for (int i=0; i< 128; i++)
    {
        file2 << (fileHash[i] > 0) ;
    }

    file1.close();
    file2.close();
}
class Graph
{
    int numVertices;
    map<string, vector<string> > adjLists;

public:
    void addEdge(string src, string dest);
    vector<string> getFriends(string);
    vector<string> getTwoLevel(string);
    void writeToFile(string);
    void getFriendSuggestion(string);
    vector<string> minHamming(string name,int size);
    void updateGraph();
};
void Graph::updateGraph()
{
    ifstream fin("Friends.txt",ios::in);
    ifstream fperson("Persons.txt",ios::in);
    string person,frnd;
    while(fin>>person>>frnd)
    {
        addEdge(person,frnd);
    }
    writeToFile(person);
    fin.close();
    fperson.close();
}
void Graph::addEdge(string src, string dest)
{
    adjLists[src].push_back(dest);
    adjLists[dest].push_back(src);

}
vector<string> Graph::getFriends(string person)
{
    return adjLists[person];
}
vector<string> Graph::getTwoLevel(string person)
{
    vector<string> result= getFriends(person);
    vector<string>::iterator beg = result.begin();
    vector<string>::iterator end = result.end();

    for(;beg != end; beg++)
    {
        vector<string> temp = getFriends(*beg);
        vector<string>::iterator beg2 = temp.begin();
        vector<string>::iterator end2 = temp.end();
        for(; beg2!=end2 ; beg2++)
        {
            if(*beg2 != person )
                {
                    result.push_back(*beg2);
                }
        }
    }
    std::sort(result.begin(), result.end());
    result.erase(unique(result.begin(), result.end()), result.end());
    return result;
}


void Graph::writeToFile(string name)
{
    string fname="I:\\Studies\\ADS package\\friends\\"+name+".txt";
    char filename[fname.size()+1];
    strcpy(filename,fname.c_str());
    fstream file(filename,ios::out);
    vector<string> friends = getTwoLevel(name);
    vector<string>::iterator beg = friends.begin();
    vector<string>::iterator end = friends.end();

    for(;beg != end; beg++)
    {

        file << *beg << endl;
    }
    file.close();
    constructFileHash(name);
}
struct node{
	string name;
	string hashValue;
	int hamDist;
};

bool compareHamDist(node N1,node N2)
{
	return(N1.hamDist < N2.hamDist);
}

int hammingDistance(string bitstr1,string bitstr2)
{
	int hamDist=0;
	for(int i=0;i<128;i++)
	{
		if(bitstr1[i]!=bitstr2[i])
		{
			hamDist++;
		}
	}
	return hamDist;
}

vector<string> Graph:: minHamming(string name,int size)
{
	node *minHam= new node[size];
	node hashNode,tempMax,tempNode;
	char fileName[50];
	int i;
	bool flag;
	vector<node> hashArray;
	ifstream nameFile, hashFile;
	string orgHash,fName,hash,tempName;

	nameFile.open("Persons.txt",ios::in);

	fName ="I:\\Studies\\ADS package\\hashes\\"+name+"_hash.txt";
	strcpy(fileName,fName.c_str());
    hashFile.open(fileName,ios::in);
    hashFile>>orgHash;
	hashFile.close();

	vector<string> friendsOfPerson = getFriends(name);


	while(nameFile>>tempName)
	{

		if(tempName!=name)
		{
		    vector<string>::iterator begstr = friendsOfPerson.begin();
            vector<string>::iterator endstr = friendsOfPerson.end();
            for(;begstr!=endstr;begstr++)
            {
                flag=true;
                if(tempName==*begstr)
                {
                    flag=false;
                    break;
                }
            }
            if(flag)
            {
			fName ="I:\\Studies\\ADS package\\hashes\\"+tempName+"_hash.txt";
			strcpy(fileName,fName.c_str());
			hashFile.open(fileName,ios::in);
			hashFile>>hash;
			hashFile.close();

			hashNode.name = tempName;
			hashNode.hashValue = hash;
			hashNode.hamDist = hammingDistance(orgHash,hashNode.hashValue);
			hashArray.push_back(hashNode);
            }
		}

	}

	vector<node>::iterator beg = hashArray.begin();
	vector<node>::iterator end = hashArray.end();

	sort(beg,end,compareHamDist);

	beg = hashArray.begin();

	for(i=0;i<size;i++)
	{
		minHam[i]=*beg;
		beg++;
	}
	vector<string> friends;
	for(int i=0 ; i< size; i++)
    {
        friends.push_back(minHam[i].name);
    }
	return friends;
}
void Graph::getFriendSuggestion(string name)
{
    vector<string> friends = minHamming(name,5);
    vector<string>::iterator beg = friends.begin();
    vector<string>::iterator end = friends.end();
    for(;beg!=end;beg++)
    {
        cout<<*beg<<endl;
    }

}
void addPerson(string name,vector<string> friends)
{
    ofstream f("Persons.txt",ios::out|ios::in);
    f.seekp(0,ios::end);
    f<<name<<endl;
    f.close();
    ofstream fwrite("Friends.txt",ios::out|ios::in);
    fwrite.seekp(0,ios::end);
    vector<string>::iterator beg= friends.begin();
    vector<string>::iterator end=friends.end();
    for(; beg!=end ; beg++)
    {
        fwrite<<endl<<name<<" "<<*beg;
    }
    fwrite.close();

}
int main()
{
    Graph g;
    ifstream fin("Friends.txt",ios::in);
    ifstream fperson("Persons.txt",ios::in);
    string person,frnd;
    while(fin>>person>>frnd)
    {
        g.addEdge(person,frnd);
    }
    while(fperson>>person)
    {
        g.writeToFile(person);
    }

    fin.close();
    fperson.close();
    int ch,n;
    string name,friends;
    vector<string> persons;
    vector<string>::iterator beg = persons.begin();
    vector<string>::iterator end = persons.end();
    ifstream f("Persons.txt",ios::in);
    while(1)
    {
        system("cls");
        cout<<"\t\t\t\tLOCALITY SENSITIVE HASHING APPLICATION IN GRAPH \n \t\t\t\t\t\tFRIEND SUGGESTION\n\n";
        cout<<"1.Get People List \n2.Add Person\n3.Get Friends of Person \n4.Get Friend suggestion\n5.Exit\n ";
        cin>>ch;
        cout<<endl;
        switch(ch)
        {
            case 1:
                    while(f>>person)
                    {
                        cout<<person<<endl;
                    }
                    f.close();
                    break;
            case 2: cout<<"\nEnter a name:";
                    cin>>name;
                    cout<<"\nEnter no. of friends:";
                    cin>>n;
                    for(int i=0;i<n;i++)
                        {
                            cin>>friends;
                            persons.push_back(friends);
                        }
                    addPerson(name,persons);
                    g.updateGraph();
                    break;
            case 3: cout<<"\nEnter a Name:";
                    cin>>name;
                    persons = g.getFriends(name);
                    beg=persons.begin();
                    end=persons.end();
                    for(; beg != end ; beg++)
                    {
                        cout<<*beg<<endl;
                    }
                    break;
            case 4: cout<<"\nEnter a name:";
                    cin>>name;
                    g.getFriendSuggestion(name);
                    break;
            case 5:exit(0);

        }
        getch();
    }
}
