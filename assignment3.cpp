#include<iostream>
#include<fstream>
#include<iterator>
#include<cmath>
#include<algorithm>
using namespace std;

template <typename T>
class OrderedMap;                                     //template of an ordered map

class HashNode{                                       //class of a hash node
	private:
		string key;                                   //the word as a key of the node
		int freq;                                     //frequency of the word as the value of the node
	public:
		HashNode(string key, int value){              //constructor
			this->key = key;
			this->freq = value;
		}
		bool isBigger(string other){
			return(this->key.compare(other)>0);       //comparator method to compare the key of this node to key of another
		}
		string get_key(){                             //method to return the key (used out of the class)
			return this->key;
		}
		friend class HashTable;                       //friend the class Hashtable
		friend class OrderedMap<HashNode>;            //friend the class Ordered Map that uses HashNode object
};

//template of orderd map class
template <typename T>
class OrderedMap{
	private:
		int capacity;                                 //capacity of the map
		int size;                                     //size of the map, in other words the number of elements in the ordered map
		T** values;                                   //pointer to array of pointers that stores the elemens of the ordered map
	public:
		OrderedMap();                                 //constructor of class ordered map
		int get_size();                               //method to return the size of the ordered map
		void resize();                                //method to dynamically resize the array storing the elements
		void insert(T* node);                         //method to insert new node to the ordered map
		void sort();                                  //method to sort elements. bubble sort is used
		int find(T* node);                            //method to find node in the ordered map. returns the index of found node in the array of values
		int binarySearch(T* values[], int l, int r, string key);  //binary search method. used as part of the find method
		int search(string key);                       //method to search for a node with given key and return its frequency/value
		friend class HashTable;                       //friend the class hashTable
};

template <typename T>
OrderedMap<T>::OrderedMap(){                          //constructor
	capacity = 1;
	size = 0;                        
}

template <typename T>
int OrderedMap<T>::get_size(){
	return size;                                      //return the current size of the ordered map
}

template <typename T>
void OrderedMap<T>::resize(){                         //dynamically expan the array
	capacity = capacity*2;                            //each time the size of the array is expanded by factor of 2
	T** new_values = new T*[capacity]; 
	for(int i=0; i<size; i++){
		new_values[i] = values[i];
	}
	T** temp = values;
	values = new_values;
}

template <typename T>
int OrderedMap<T>::find(T* node){                     //find the given node and return its index. if not found -1 is returned
	if(this->size==0) return -1;
	int left = 0;
	int right = this->size-1;
	return binarySearch(values, left, right, node->get_key());  //call binary search to find in O(logn) time
}

template <typename T>
int OrderedMap<T>::binarySearch(T* val[], int l, int r, string key){  //binary search
	if(l<0 || r>this->size) return -1;  
	if(r==l){                          
		if(val[l]->key == key) return l;
		else return -1;
	}
	else if(r>l){
		int to_check = l +(r-l)/2;
		if(val[to_check]->key == key) return to_check;
		else if(val[to_check]->isBigger(key)) return binarySearch(val, l, to_check-1, key);  /
		else return binarySearch(val, to_check+1, r, key);
	}
	return -1;
}

template <typename T>
void OrderedMap<T>::insert(T* node){                  //insert the given node to the ordered map
	if(size == 0){                                    //if it is the first element in the ordered map instantiate the array of values
		values = new T*[1];
		size++;
		values[0] = node;
	}else{                           
		int findIndex = find(node);                   //index of the key of the node to be inserted in the array (-1 if not found)
		if(size==capacity){
			resize();}                                //resize the array of values as necessary
		if(findIndex == -1){                          //insert as a new node
			values[size] = node;
			size++;
			sort();                                   //sort the list when a new element is inserted
		}
		else{                                         //if the key is present increment the fruqeuncy of found node
			values[findIndex]->freq += 1; 
		}
		}
}

template <typename T>                                 //bubble sort that sorts the list alphabetically
void OrderedMap<T>::sort(){
	int i,j;
	for(i = 0; i<size-1; i++){
		for(j = 0; j<size-i-1; j++){
			if(values[j]->isBigger(values[j+1]->get_key())){
				T* temp = values[j];
				values[j] = values[j+1];
				values[j+1] = temp;

			}
		}
	}
}

template <typename T>
int OrderedMap<T>::search(string key){                //search for the iterator (index in this case) of the given key, return -1 if not found
	T to_find = T(key, 0);
	int index = find(&to_find);
	if(index == -1) return index;
	else return values[index]->freq;                  //return the frequency of the given key in case it is in the ordered map
}


class HashTable{                                      //class of the HashTable
	private: 
		OrderedMap<HashNode>* buckets;                //Array storing the buckets
		int size;                                     //the total number of words in the Hashtable, including copies
		int capacity;                                 //the capacity of the HashTable = size of the array storing buckets
		string hashChoice;                            //the hash function used in the system
	public:
 		HashTable(int capacity, string choice);       //constructor
		~HashTable();                                 //destructor
		unsigned long hashCode(const string key);     //helper picking the right hash function
		unsigned long hashCode1(const string key);    //Summation Hash Code
		unsigned long hashCode2(const string key);    //Polynomial Hash Code
		unsigned long hashCode3(const string key);    //Cyclic Shift Hash Code
		unsigned long hashCode4(const string key);    //Summatoin Hash Code with MAD compression
		void insert(const string key);                //insert the key into the hash table
		int search(const string key);                 //search for the key in the hash table and return its frequence
		int count_words();                            //return the total number of the words
		int count_collisions();                       //return the number of collisions
		int count_unique_words();                     //return the numbef of unque words
		//void highestFreq();                           //method to output the word with highest frequency
		void find_freq(string word);                  //output the frequency of the given word
		friend class OrderedMap<HashNode>;            //friend OrderedMap
};

HashTable::HashTable(int capacity, string choice){    //constructor
	buckets = new OrderedMap<HashNode>[capacity]; 
	for (int i = 0; i<capacity; i++){buckets[i] = OrderedMap<HashNode>();} //instantiate an OrderedMap for each bucket
	this->capacity = capacity;
	this->size = 0;
	hashChoice = choice;                              //hash function
}

HashTable::~HashTable(){                              //destructor
	delete[] buckets;
}

unsigned long HashTable::hashCode(const string key){  //helper to navigate the hash code
	if(hashChoice == "1"){
		return (hashCode1(key));
	}else if(hashChoice == "2"){
		return (hashCode2(key));
	}else if(hashChoice == "3"){
		return (hashCode3(key));
	}else return hashCode4(key);
}

unsigned long HashTable::hashCode1(const string key)  //Summation Hash Code
{
	long hash = 0;
	for(int i=0; i<key.size(); i++){
		hash += int(key[i]);
	}
	return hash%capacity;
}

unsigned long HashTable::hashCode2(const string key)  //Polynomial Hash Code
{
	long hash = 0;
	int a = 33;
	int b = 1e6 + 7;
	long long expa = 1;
	for(int i=0; i<key.size(); i++){
		hash = (hash + (key[i]+25)*expa)%b;
		expa = (expa*a)%b;
	}
	return hash%capacity;
}

unsigned long HashTable::hashCode3(const string key)  //Cycle Shift Hash Code
{
	char k[key.size()];
	for(int i = 0; i<key.size(); i++){
		k[i] = key[i];
	}
	unsigned int hash = 0;
	for(int i=0; i<key.size(); i++){
		hash = (hash<<7) | (hash>>25);
		hash += k[i];
	}
	return hash%capacity;
}

unsigned long HashTable::hashCode4(const string key){ //Summation Hash Code using MAD compression method
	long hash = 0;
	for(int i=0; i<key.size(); i++){
		hash += key[i]*20+194506;
	}
	return (hash%109481)%capacity;
}

void HashTable::insert(const string key){             //insert an entry into the hash table
	int hashcode = hashCode(key);                     //generate the hash code (bucket)
	HashNode* to_insert = new HashNode(key, 1);       //create a HashNode with frequency 1
	buckets[hashcode].insert(to_insert);              //insert
	this->size++;

}

int HashTable::search(const string key){              //search for given key and return its frequency (-1 if not found)
	int hashcode = hashCode(key);
	if(buckets[hashcode].search(key) == -1) return -1;
	else return buckets[hashcode].search(key);
}

int HashTable::count_words(){                         //return the total number of words in the hash table, including repetetive ones
	return this->size;
}

int HashTable::count_collisions(){                    //count the number of collisions
	int total = 0;
	for(int i=0; i<this->capacity; i++){              //total collisions = sum of collisions in each bucket
		if(buckets[i].get_size()!=0) total += buckets[i].get_size()-1;  //collisions in each bucket = the size of the bucket -1
	}
	return total;
}

int HashTable::count_unique_words(){                  //count the number of unique words
	int total = 0;
	for(int i=0; i<this->capacity; i++){              //total number of unique words = the number of HashNodes in the hash table
		total += buckets[i].get_size();
	}
	return total;
}

void HashTable::find_freq(string word){               //output frequency of the given word
	transform(word.begin(), word.end(), word.begin(), ::tolower); //transform to lowercase
	int foundFrequency = search(word);                //search for the frequency
	if(foundFrequency==-1) cout<<"\""<<word<<"\" not found";
	else cout<<"frequency of \""<<word<<"\" is: "<<foundFrequency<<endl;
}

void separate_string(string k, HashTable* wizard){    //function to separate the input string into individual words
	string word = "";
	int pos;
	for(int i=0; i<k.size(); i++){
		if(k[i] == '\n' || k[i] == ' '){              //use newline and space as separators
			if(word.size()!=0) {                      //ignore if the word is empty (ignore symbols or whitespace characters)
				transform(word.begin(), word.end(), word.begin(), ::tolower);
				wizard->insert(word);                 //insert the extracted word into the hash table
				word = "";                            //reset the word
			}
		}else if (isalpha(k[i]))  word += k[i];       //add character to the word only if it is a letter
	}
	if(word.size()!=0) {                              //add the last word (if it is a word)
		transform(word.begin(), word.end(), word.begin(), ::tolower);
		wizard->insert(word);
	}
}

void help(){                                          //print available commands
	cout<<endl
		<<"           LIST OF AVAILABLE COMMANDS"<<endl
		<<endl
		<<" help                         print available commands"<<endl
		<<" freq <word>                  print frequence of the given word"<<endl
		<<" exit                         exit the program"<<endl
		<<">";
}

void switch_hashFun(){                                //function to switch the hash function
	cout<<"	LIST OF AVAILABLE HASH FUNCTIONS: "<<endl
		<<"1. Summation Hash Code"<<endl
		<<"2. Polynomial Hash Code"<<endl
		<<"3. Cycle Shift Hash Code"<<endl
		<<"4. MAD Hash Code (default)"<<endl
		<<"(Optional. Press Enter to skip)"<<endl;
	getline(cin, hashChoice);                         //hash function choice is taken as user input
	while(hashChoice!="1" && hashChoice != "2" && hashChoice != "3" && hashChoice != "4" && hashChoice.length() != 0){
		cout<<"invalid input. please try again"<<endl;//error checking of the input
		getline(cin, hashChoice);
	}
}

int main(){
	string fullpath, hashChoice;
	ifstream sourcefile;

	cout<<"    WELCOME TO WORD COUNT WIZARD"<<endl    //welcome message
		<<"enter the full path of input file: ";
	getline(cin, fullpath);                           //get the file
	sourcefile.open(fullpath);                        //open the file
	while(!sourcefile){                               //error checking for file name
		cout<<"File was not found, please try again: ";
		cin>>fullpath;
		sourcefile.open(fullpath);
	}

	switch_hashFun();                                 //switch hash function
	
	string words = "";                                //read the file into a string combining lines
	while(!sourcefile.eof()){
		string line;
		getline(sourcefile, line);					
		words += ' '+line;
	}
	
	HashTable wizard = HashTable(101501, hashChoice); //the capacity is a prime number bigger than 100000
	separate_string(words, &wizard);                  //separate the string into words and insert them into the hash table wizard
	sourcefile.close();

	cout<<"--------------------------------------------"<<endl //pring statistics
		<<wizard.count_words()<<" words in total"<<endl
		<<wizard.count_unique_words()<<" unique words"<<endl
		<<wizard.count_collisions()<<" collisions"<<endl;

	bool exit = false;                                //interact with user
	string user, command, argument;
	while(!exit){
		help();                                       //print available commands
		getline(cin, user);
		command = user.substr(0, user.find(" "));     //separate the user input into command and argument (when there is an argument, otherwise argument is empty and not used)
		argument = user.substr(user.find(" ")+1);

			 if(command == "help")      help();
		else if(command == "freq")      wizard.find_freq(argument);
		else if(command == "exit")      exit = true;
		else cout<<"Command not available"<<endl;     //error checking for commands
	}

	return 0;
};