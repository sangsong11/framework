
#pragma GCC diagnostic error "-std=c++11"
#include <iostream>
#include <map>
#include <vector>
#include <string>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
using namespace std;

#define MAX_LEN 1024
class Solution
{
public:
    vector<string>findRepeatedDnaSequences(string s)
    {
       //
       map<string,int> dna;
       vector<string> result;
       for(int i = 0; i < s.length();i++)
       {
            string word = s.substr(i,10);
            if(dna.find(word) != dna.end())
            {
   	 	dna[word] += 1;
	    }
            else
            {
               dna[word] = 1;
            }
       }

       map<string,int>:: iterator it;
       for( it = dna.begin(); it != dna.end();it++)
       {
           if(it->second >1)
           {
              result.push_back(it->first);
           }
       }
       return result;
    }

  int coinChange(vector<int>& coins,int amount)
  {
     vector<int>dp(amount+1,-1);
     dp[0] = 0;
     for(int i = 1; i<=amount;i++)
     {
         for(int j=0;j<coins.size();j++)
         {
            if(coins[j]<=i && dp[i-coins[j]] != -1)
            {
                if(dp[i] == -1 || dp[i] > dp[i-coins[j]] +1)
                {
                  dp[i] = dp[i - coins[j]] +1;
                }            
            }
         }
     }

    return dp[amount];
  }  

};



int change_to_array(const char *str,int a[])
{
   int len = strlen(str);
   int i = 0;
   int j = 0;

   for(i = len -1 ; i >= 0; i--)
   {
      a[j++] = str[i]-'0';
   }
   return j;
}


char *addBinary(char *a, char *b)
{
   int n1[MAX_LEN] = {0};
   int n2[MAX_LEN] = {0};

   int nlen1 = change_to_array(a,n1);
   int nlen2 = change_to_array(b,n2);

  int max_len = nlen1 > nlen2 ? nlen1:nlen2;
  int result[MAX_LEN] = {0};
  int result_len = 0;
  int add_one = 0;
  int i;

  for(i = 0;i< max_len;i++)
  {
    int bit = n1[i] + n2[i] + add_one;
    if(bit >= 2)
    {
      add_one = 1;
    }
    else
    {
      add_one = 0;
    }
    result[result_len++] = bit % 2;
  } 
  
  if(add_one == 1)
  {
    result[result_len++] = 1;
  }

  char *result_str = (char*)malloc(MAX_LEN * sizeof(char));
  int strn = 0;
  
  for(i = result_len -1; i>=0;i--)
  {
    result_str[strn++] = result[i]+'0';
  }
 result_str[strn] = '\0';
 return result_str;
}

int main(int argc,char **argv)
{
    string s= "AAAAAGGGTTCCCCCCAAAAAGGGTTC";

    Solution solution;
    vector<string> result = solution.findRepeatedDnaSequences(s);

    for(int i = 0 ; i < result.size(); i++)
    {
         printf("%s\n",result[i].c_str());
    }


    char a[] = "11";
    char b[] = "1";

    char *output = addBinary(a,b);
    printf("%s + %s = %s\n",a,b,output);
    free(output);



    vector<int> coins;
    coins.push_back(1);
    coins.push_back(2);
    coins.push_back(5);
    coins.push_back(7);
    coins.push_back(10);

    printf("dp %d\n",solution.coinChange(coins,14));


    return 0;
}
