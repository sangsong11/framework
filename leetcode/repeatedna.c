
#pragma GCC diagnostic error "-std=c++11"
#include <iostream>
#include <map>
#include <vector>
#include <string>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <set>
#include <algorithm>
using namespace std;

#define MAX_LEN 1024

struct ListNode
{
   int val;
   ListNode *next;
   ListNode(int x): val(x),next(NULL){}
};

bool cmp(const vector <int> &a,const vector<int> &b)
{
  return a[0] < b[0];
};

class Solution
{
public:
   /*leetcode 重复的DNA序列， 已知一段DNA序列，它有A、C 、G、和T四种字符代表的核苷酸组成，在计算机中DNA序列可以用字符串
    表示，现在有一段字符串表示DNA序列，我们需要从该字符串中找出长度为10，并在序列中至少出现2次的子串*/

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

  /*leetcode 322 找零钱 已知一些不同面值的钞票与一个金额，如何用最少个数钞票组成这个金额，失败返回-1，否则返回个数*/
  int coinChange(vector<int>& coins,int amount)// 1:数组钞票的面值 2：金额数
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

    return dp[amount]; //返回最小的张数
  } 

/*leetcode198 房间寻宝  在一条直线上，有n个房屋，每个房屋中有数量不等的财宝，有一个探险家希望从房间中获取财宝
房屋中有报警器，如果同时从相邻的两个房屋中获取财宝就会触发警报，那么在不触发警报的前提下，最多获取多少财宝*/  
  int rob(vector<int>&nums)
  {
      int n = nums.size();
      if(n == 0)
      {
        return 0;
      }
      if(n == 1)
      {
        return nums[0];
      }

      vector<int> value(n,0);
      value[0] = nums[0];
      
      value[1] = max(nums[0],nums[1]);
      for(int i = 2;i < n;i++)
      {
        value[i] = max(value[i-1],value[i-2]+nums[i]);
      }

     return value[n-1];
  }

  /*lettcode 268 消失的数字，位运算，已知数组nums无序的存储了0至n这n+1个数字，这时随机去掉其中的某个数字，
  如何只遍历一次数组，就能求出去掉的那个数字呢*/
  int missingNUmber(vector<int> &num) //返回消失的数字
  {
    int missing = 0;
    for(int i = 0; i < num.size();i++)
    {
      missing = missing ^ num[i] ^ (i+1);
    }
    return missing;
  }

  /*leetcode 452 射击气球  传入参数为数组的坐标，返回为箭的个数*/
  int findMinArrowShots(vector<vector<int>> &points)
  {
    if(points.size() == 0)
     {
       return 0;
     }
     std::sort(points.begin(),points.end(),cmp);//对气球从左到右进行排序
     int arrow_count = 1;
     int interval_right = points[0][1];//存储射击区间的右端点，初始化为1气球的右端点

     for(int i = 1;i< points.size();i++)//遍历各个气球
     {
       if(interval_right >=points[i][0]) //射击区间的右端点大于遍历气球的左端点，可以射击到气球
       {
            if(interval_right > points[i][1])//当前区间右端点大于遍历气球的右端点
            {
              interval_right = points[i][1];//将射击区间的右端点更新为遍历气球的右端点
            }
       }
       else
       {
         arrow_count++; //增加射击次数
         interval_right = points[i][1];//设置新的射击区间右端点为气球的右端点
       } 
     }
     return arrow_count;
  }

  /*leetcode 206 链表的逆置，直接逆置法*/
  ListNode * reverseList(ListNode *head)
  {
    #if 1
       ListNode *reverse = NULL;
       ListNode *next   = NULL;
       while(head)
       {
         next = head->next;
         head->next = reverse;
         reverse = head;
         head = next;
       }
     return reverse;  
   #else          //头插法
      ListNode temp(0);
      ListNode *next = NULL;
      while(head)
      {
        next = head->next;
        head->next = temp.next;
        temp.next = head;
        head = next;
      }
     return temp.next;
   #endif
  }
 /*leetcode 160 链表焦点*/   // set 容器的两个特点： 元素时排好序的，元素时唯一的
 ListNode *getintersectionNode(ListNode *headA,ListNode *headB)
 {
   #if 1
      set<ListNode*> check_set;
      ListNode *result = NULL;
      while(headA)
      {
        check_set.insert(headA);
        headA = headA ->next;
      }
      while(headB)
      {
        if(check_set.find(headB) != check_set.end())
        {
          result = headB;
          break;
        }
        headB = headB ->next;
      }
      return result; 
  #else


  #endif

 }


  void list_print(ListNode *head,const char* name)
  {
    printf("%s:",name);
    if(!head)
    {
      printf("NULL\n");
      return;
    }
    while(head)
    {
      printf("[%d]",head->val);
      head = head->next;
    }
    printf("\n");
  }


  /*leetcode473 火柴拼正方形  剪之优化   */
public:
     bool makesquare(vector<int>&nums)
     {
       if(nums.size() < 4)
       {
         return false;
       }
       int sum = 0;
       for(int i = 0; i < nums.size();i++)
       {
         sum += nums[i];
       }
       if(sum % 4)
       {
         return false;
       }
       sort(nums.rbegin(),nums.rend());
       int bucket[4] = {0};
       return backtrack(0,nums,sum/4,bucket);
     }
private:
// i 代表正在放置的火柴的小标，nums存储火柴的长度
// edge 为计算好的正方形边长，bucket存储4个桶的高度
 bool backtrack(int i,vector<int>&nums,int edge,int bucket[])
 {
    if(i >= nums.size())
    {
      return true;
    }
    for(int j = 0;j<4;j++)
    {
      if(bucket[j]+nums[i] > edge) //这根火柴无法放在第j个桶中
      {
        continue;
      }
      bucket[i] += nums[i]; //num[i]放入桶j中
      if(backtrack(i+1,nums,edge,bucket)) //递归放置下一个火柴
      {
         return true; //下一根火柴放置成功则直接返回true
      }
      bucket[j] -= nums[i];  //将nums[i]从桶j中拿出
    }
    return false;
 }
 
   /* leetcode N皇后问题 回溯法*/
   // x,y表示放置皇后的坐标，二维数组attack表示棋盘是否可放置皇后
   void put_queen(int x,int y,vector<vector<int>> &attack)
   {
     //方向数组，方便后面对8个方向进行标记
     static const int dx[] = {-1,1,0,0,-1,-1,1,1};
     static const int dy[] = {0,0,-1,1,-1,1,-1,1};

     attack[x][y] = 1; 
     //通过二层循环，将皇后可能攻击的位置进行标记
     for(int i = 1;i<attack.size();i++) //从皇后位置向1到n-1个距离延伸
     {
       for(int j = 0;j < 8;j++) //遍历8个方向
       {
         int nx = x + i * dx[j];  //生成的新位置的行
         int ny = y + i * dy[j];  //生成的新位置的列
         if(nx >= 0 && nx < attack.size() && ny >= 0 && ny < attack.size()) //在棋盘内
         {
           attack[nx][ny] = 1;
         }
       }
     }
   }
  // K表示当前处理的行
  // n 表示N皇后问题
  // queen存储皇后的位置
  //attack 标记皇后攻击的位置 
  //solve 存储N皇后的全部解法
   void qbacktrack(int k,int n,vector<string>&queen,vector<vector<int>> &attack,
        vector<vector<string>> &solve)
    {
      if(k == n)   //找到一组解
      {
        solve.push_back(queen);
        return ;
      }
      
      //遍历0至n-1列，在循环中，回溯试探皇后可知放置的位置
      for(int i = 0;i < n;i++)
      {
        if(attack[k][i] == 0) //判断当前k行i列是否可以放置皇后
        {
          vector<vector<int>> tmp = attack; //备份attack数组
          queen[k][i] = 'Q';
          put_queen(k,i,attack);
          qbacktrack(k+1,n,queen,attack,solve);
          attack = tmp;
          queen[k][i] = '.';
        }
      }
    }

public:
    vector<vector<string>> solveQueens(int n)
    {
       vector<vector<string>> solve; //存储最后结果
       vector<vector<int>>attack;  // 标记皇后的攻击位置
       vector<string> queen;  //保存皇后位置

       for(int i = 0; i < n; i++)
       {
         attack.push_back((std::vector<int>()));
         for(int j = 0;j < n;j++)
         {
           attack[i].push_back(0);
         }
         queen.push_back("");
         queen[i].append(n,'.');
       }
       qbacktrack(0,n,queen,attack,solve);
       return solve;
    }
   

};


/*leeetcode 字符串表示二进制大整数加法*/
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

   

   vector<int>nums;
   nums.push_back(1);
   nums.push_back(3);
   nums.push_back(4);
   nums.push_back(3);
   nums.push_back(0);
   nums.push_back(2);
   nums.push_back(7);

   printf("%d\n",solution.rob(nums));
  

  vector<int> missnums;
  missnums.push_back(1);
  missnums.push_back(0);
  missnums.push_back(2);
  missnums.push_back(5);
  missnums.push_back(4);
  printf("%d\n",solution.missingNUmber(missnums));

  
  vector<vector<int>> points;
  vector<int> p0,p1,p2,p3;
  p0.push_back(10);
  p0.push_back(16);
  points.push_back(p0);

  p1.push_back(2);
  p1.push_back(8);
  points.push_back(p1);

  p2.push_back(1);
  p2.push_back(6);
  points.push_back(p2);

  p3.push_back(7);
  p3.push_back(12);
  points.push_back(p3);

  printf("%d\n",solution.findMinArrowShots(points));


  ListNode la(1);
  ListNode lb(2); 
  ListNode lc(3);
  ListNode ld(4);
  ListNode le(5);

  la.next = &lb;
  lb.next = &lc;
  lc.next = &ld;
  ld.next = &le;
  ListNode *head = &la;
  solution.list_print(head,"old_list");
  head = solution.reverseList(&la);

  solution.list_print(head,"reverse_list");


 vector<int> numsquare;
 numsquare.push_back(1);
 numsquare.push_back(2);
 numsquare.push_back(3);
 numsquare.push_back(4);
 numsquare.push_back(5);
 numsquare.push_back(5);
 printf("%d\n",solution.makesquare(numsquare));


 vector<vector<string>> qresult;
 qresult = solution.solveQueens(8);
 printf("4皇后共有%ld多种解法:\n",qresult.size());
 for(int j = 0; j < qresult.size();j++)
 {
   printf("解法%d:\n",j+1);
   for(int k = 0;k < qresult[j].size();k++)
   {
     printf("%s\n",qresult[j][k].c_str());
   }
   printf("\n");
 }
return 0;



}
