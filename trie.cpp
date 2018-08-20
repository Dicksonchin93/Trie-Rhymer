#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <algorithm>
#include "macros.h"
using namespace std;



class Trie
{
	private:
	struct node
	{
		int prefix_count;
		bool isEnd;
		struct node *child[26];
	}*head;

	
	public:
	vector<string> best_rhyme;
	bool stop_trigger; //stop program from iterating to each character when the previous trial has already fail to find a match
	Trie()
	{
		head = new node();
		head->isEnd = false;
		head->prefix_count = 0;			//additional variable to easily obtain number of word branches in this node
		stop_trigger = false;
	}

	void reverseStr(string& str);   		//reverse string characters
	void toLower(string& str);      		//lower case all string characters
	void toUpperFirst(string& str); 		//upper case first character in string
	void insert(string word);       		//add new string to Trie data structure(reversed string)
	bool search(string word);			//search whether full word exist
	int words_with_prefix(string prefix);		//Get number of words that is present with this prefix
	void printMatches(node *tmp, vector<char> word, vector<string>& all_string);//Print words as output
	void Rhymer(string postfix, bool reset);			//Find rhyming words and print with printMatches

};

void Trie::reverseStr(string& str)
{
    	int n = str.length();
    	for(int i =0; i<n/2;i++)
    	{
        	swap(str[i], str[n-i-1]);
    	}
}

void Trie::toLower(string& str)
{
    	locale loc;
    	for(string::size_type i =0; i<str.length(); i++)
    	{
        	str[i] =tolower(str[i],loc);
    	}

}


void Trie::toUpperFirst(string& str)
{
    
    	str[0] =toupper(str[0]);
    


}
void Trie::insert(string word)
{
        if(word.empty())
            	return;
        toLower(word);
        reverseStr(word);
	node *current = head;
	current->prefix_count++;

	for(int i = 0 ; i < word.length(); ++i)
	{
                
		int letter = (int)word[i] - (int)'a';	//extract first character of word

		if(current->child[letter] == NULL)
		{
			current->child[letter] = new node();
		} 		
		current->child[letter]->prefix_count++;
		current = current->child[letter];
	}
	current->isEnd = true;
}

bool Trie::search(string word)
{
        //reverseStr(word);
	node *current = head;
	for(int i = 0 ; i < word.length(); ++i)
	{
		int letter = (int)word[i] - (int)'a';
		if(current->child[letter] == NULL)
			return false;		//not found
		current = current->child[letter];
	}
	return current->isEnd;
}

int Trie::words_with_prefix(string prefix)
{
	node *current = head;
	for(int i = 0; i < prefix.length() ; ++i)
	{
		int letter = (int)prefix[i] - (int)'a';
		if(current->child[letter] == NULL)
			return 0;
		else
			current = current->child[letter];
	}
	return current->prefix_count;
}

void Trie::printMatches(node *tmp, vector<char> word, vector<string>& all_string)
{
    	if(tmp==NULL)
    		return;

    	if(tmp->isEnd)
    	{
       		vector<char>::iterator it = word.begin();
        	string tmp_ = "";
        	while(it != word.end())
        	{
            	tmp_.append(string(1,*it));
            	++it;
        	}
        	reverseStr(tmp_);
        	toUpperFirst(tmp_);
#if DEBUG==1
        	cout << tmp_ << "\n";
#endif
	

		all_string.push_back(tmp_);
		
		
		
    	}
	
    	for(int i=0;i<26;i++)
    	{
        	if(tmp->child[i] != NULL)
        	{
            		word.push_back((char)i+'a');
           		printMatches(tmp->child[i], word, all_string);
            		word.pop_back();
        	}

    	}


  
}

void Trie::Rhymer(string postfix, bool reset)
{
    	toLower(postfix);
    	vector<char> word;
#if DEBUG==1
    	cout << __FUNCTION__ << " called with postfix: "<< postfix << endl;
#endif
	if(reset)
	{
		best_rhyme.clear();
		stop_trigger = false;
	}
    	if(postfix.length() == 0)
    	{
        	cout << " empty postfix" << endl;
        	return;
    	}
    	reverseStr(postfix);
    	node *current = head;
    	for(int i=0; i < postfix.length();i++)
    	{
        	int letter = (int)postfix[i] - (int)'a';
        	if(current-> child[letter] == NULL)
        	{
			stop_trigger = true;
            		return;
        	}
        	word.push_back(postfix[i]);
        	current = current->child[letter];
    	}
#if DEBUG==1
    	cout<< "Matches : ";
#endif
        vector<string> all_string;
    	printMatches(current, word, all_string);
	

	if(!all_string.empty())
	{	
		best_rhyme.clear();
		vector<string>::iterator it = all_string.begin();
        	while(it != all_string.end())
        	{
			best_rhyme.push_back(*it);
#if DEBUG==1
            		cout<< *it << endl;
#endif
			++it;
        	}
	}
       	
	
    

}


