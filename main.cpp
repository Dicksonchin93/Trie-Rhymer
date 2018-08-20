#include <iostream>
#include <string>
#include <fstream>
#include <ctime>
#include <unistd.h>
#include <ios>
#include <iostream>

#include "trie.cpp"
#include "macros.h"
using namespace std;


struct product {
  int weight;
  double price;
} ;


//////////////////////////////////////////////////////////////////////////////
//
// process_mem_usage(double &, double &) - takes two doubles by reference,
// attempts to read the system-dependent data for a process' virtual memory
// size and resident set size, and return the results in KB.
//
// On failure, returns 0.0, 0.0

void process_mem_usage(double& vm_usage, double& resident_set)
{
   using std::ios_base;
   using std::ifstream;
   using std::string;

   vm_usage     = 0.0;
   resident_set = 0.0;

   // 'file' stat seems to give the most reliable results
   //
   ifstream stat_stream("/proc/self/stat",ios_base::in);

   // dummy vars for leading entries in stat that we don't care about
   //
   string pid, comm, state, ppid, pgrp, session, tty_nr;
   string tpgid, flags, minflt, cminflt, majflt, cmajflt;
   string utime, stime, cutime, cstime, priority, nice;
   string O, itrealvalue, starttime;

   // the two fields we want
   //
   unsigned long vsize;
   long rss;

   stat_stream >> pid >> comm >> state >> ppid >> pgrp >> session >> tty_nr
               >> tpgid >> flags >> minflt >> cminflt >> majflt >> cmajflt
               >> utime >> stime >> cutime >> cstime >> priority >> nice
               >> O >> itrealvalue >> starttime >> vsize >> rss; // don't care about the rest

   stat_stream.close();

   long page_size_kb = sysconf(_SC_PAGE_SIZE) / 1024; // in case x86-64 is configured to use 2MB pages
   vm_usage     = vsize / 1024.0;
   resident_set = rss * page_size_kb;
}


// Find syllables cutting point in word and return sequentially concatenated syllables strings starting from the end of the word
void syllable(string word, vector<string>& first)
{
    	int i = 0;
    	int z = 0;
	int tmp_i=0;
    	for(i=word.length()-1;i>-1; i--){
        	char w = word[i];
		
        	if(w=='a'||w=='e'||w=='i'||w=='o'||w=='u')
        	{   
            		string tmp_first;
            		tmp_first = word.substr(i-1, word.length());
            
#if DEBUG==1
            		cout << tmp_first << ' ' << endl;
#endif
	    		first.push_back(tmp_first);
			tmp_i = i;
        	}
       
    	}
	if(tmp_i!=word.length()-1)
	{
		first.push_back(word);
#if DEBUG==1
		cout << word << ' ' << endl;
#endif
	}
    
}


int main()
{       
	Trie* head = new Trie();
	ifstream dicTionary("dictionary.txt");
        if(!dicTionary){
		cout<<"Can't Open File"<<endl;
          	return -1;
	}
	string word;

	cout << "initiating dictionary" << endl;
   	while(dicTionary >> word)
	{
		//cout << word << endl;
		head->insert(word);
	}
	
	ifstream query("query.txt");
        if(!query){
		cout<<"Can't Open File"<<endl;
          	return -1;
	}
        string query_word;
//SEARCH THROUGH EACH CHARACTER IN WORD
    clock_t begin = clock();
#if SYL==0
   	while(query >> query_word)
	{
		bool reset = true;
		cout <<"Input ("<< query_word << ") :";
        	for(int i=query_word.length()-2;i>-1; i--)
		{
          		string first = query_word.substr(i, query_word.length()-i);
        		head->Rhymer(first, reset);
                        reset = false;
			
			if(head->stop_trigger==true)
			{
	#if DEBUG==1
				cout<< "Found Best Rhyme Breaking Further Character Loop"<<endl;
	#endif
				break;
			}
		}
		cout << "Matches ";
                string tmp_matches;
      
		if(!head->best_rhyme.empty())
		{
			
#if RAND==0

			vector<string>::iterator it = head->best_rhyme.begin();
        		string tmp_ = "";
        		while(it != head->best_rhyme.end())
        		{

            			tmp_matches.append(*it);
				tmp_matches.append(" ");
				++it;
        		}
			cout << tmp_matches ;
		
#elif RAND==1
        srand(time(NULL));
        int r = rand() % head->best_rhyme.size();
        //printf('size of rhymed %d', head->best_rhyme.size());
        //cout<< head->best_rhyme.size()<<" "<<r;
        cout<<head->best_rhyme[r];
#endif
        }
		else
		{
			cout<< "None";
		}
		cout << endl;
       	
		
	
	}
//SEARCH BY SYLLABLE
#else
	while(query >> query_word)
	{
		bool reset = true;
		cout <<"Input ("<< query_word << ") :";

		
		vector<string> first;
		syllable(query_word, first);
		vector<string>::iterator it = first.begin();
        	string tmp_ = "";
        	while(it != first.end())
        	{

            		head->Rhymer(*it, reset);
                	reset = false;
			++it;
			if(head->stop_trigger==true)
			{
	#if DEBUG==1
				cout<< "Found Best Rhyme Breaking Further Syllable Loop"<<endl;
	#endif
				break;
			}
        	}
	
        

		cout << "Matches ";
                string tmp_matches;
		if(!head->best_rhyme.empty())
		{
#if RAND==0
			
			vector<string>::iterator it = head->best_rhyme.begin();
        		string tmp_ = "";
        		while(it != head->best_rhyme.end())
        		{

            			tmp_matches.append(*it);
				tmp_matches.append(" ");
				++it;
        		}
			cout<<tmp_matches;
#elif RAND==1
        srand(time(NULL));
        int r = rand() % head->best_rhyme.size();
        //printf('size of rhymed %d', head->best_rhyme.size());
        //cout<< head->best_rhyme.size()<<" "<<r;
        cout<<head->best_rhyme[r];
#endif
        }
        
         
  
		else
		{
			cout<< "None";
		}
		cout << endl;
       	
		
	
	}
#endif
                
        clock_t end = clock();
        double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
        double vm, rss;
        process_mem_usage(vm, rss);
        cout << "-------------------------------------"<<endl;
        cout << " VM: " << vm << "; RSS(Resident Set Size): " << rss << endl;
        //cout << " total memory usage" << getrusage() <<endl;
        cout<< " total elapsed time for query task " << elapsed_secs<< endl;
      	return 0;
        
       
}

