# Trie-Rhymer
Rhymer function implemented with Trie in C++

There is three files included listed below:

- compile.sh : shell script containing command used to compile in ubuntu
- main.cpp    : Main code used to run and compile the codes
- trie.cpp    : Data Structure definition
- macros.h    : Variables that can be toggled 0 and 1 to change different running versions
- dictionary.txt : example dictionary word list separated by newlines
- query.txt      : example query word list separated by newlines

Run ./compile.sh and run ./main to execute code
# Software description
----------------------------------------------------
C++0x/ C++11

compiled with g++, searched online and it seems mac computers have a version of c++ so I assume it would work on Mac.

Set random mode, syllable mode, and debug mode in macro.h

# Methodology
----------------------------------------------------
In this implementation, words that rhyme is defined by most number of characters that matched, it doesn't take into account strings that have different characters but sounds the same when pronounced. Initially did some research on good data structures with string matching, found a few data structure which was not used for the final implementation based on a few reasoning listed below:

- Aho–Corasick algorithm : The complexity of the algorithm is linear in the length of the strings plus the length of the searched text plus the number of output matches
- Bloom-filter : Memory efficient does is very scalable, though it will have chances of getting false positive so rejected
- Hash table : Fast but Trie will be faster considering the nature of the problem
- HAT-Trie : Fast efficient scalable but too complicated to implement
- Radix-Trie : a compressed version of a Trie, thus more efficient and scalable, wanted to implement after finishing Trie but did not spent time doing that

In the end I picked Trie, considering the nature of the data structure which is initially used for autocomplete with prefix, I implement postfix rhymer with trie's by reversing the sequence of the dictionary and query strings before inserting into or searching the Trie. Comparison of the query word need not be compared with all the dictionary words character by character rather we can do it rather quickly with Trie's, as when the last character is not present in the first node of the reversed trie, a None rhyme output will be return, which is O(1), while if there is an initially postfix match on the reversed trie, and there is two or more words that has that postfix, it will only be done once compared to two or more times.

I implemented two ways to match the query to the Trie, first is going through concatenated string characters from 1 to max number of words starting from the end of the query word stopping to further iterate when failed to find a match as the best number of character match has already been found. Second is splitting the query word by its syllable,syllable is the way humans find rhyming words so it make sense to use it here, I define syllable by splitting by one character after a vowel a,e,i,o,u but in reality syllables are much more complicated than that, we need to add additional conditional statements to accustom to cases like 'range' is one syllable 'saa' is one syllable 'rle' is one sylable and so on. But for the sake of speed and concept, I implement a vowel version. For the syllable version it also stops when it fails to match the concatenated syllable characters to the Trie. 

To demonstrate the concatenated string characters with the word "Orange"

Method 1 - 

           first query  : 'e'

           second query : 'ge'
           
           .......
           
           last query   : 'Orange'
           
Method 2  

          First query   : 'ge'

          second query  : 'range'
          
          last query    : 'Orange'
          
Both methods will return the most number of characters match and does have at least one matched word in the trie

Trie structure search time complexity is linear O(n), where n is  maximum string length, its penalty is on storage requirements since all the characters will take up one node increasing the size of the tree, radix trie will reduce this. Definitely a better syllable extractor needs to be done, possibly using a deep learning method as my method of syllable implementation may accidentally skip characters and return the wrong best matched rhyme
