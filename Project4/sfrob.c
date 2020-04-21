#include <stdio.h>
#include <stdlib.h>


//void printError(int x, char** 

char** insertWord(char** container, char* word, int length)
{
  return  realloc(container, (length+1)*sizeof(char*));
}

void printErr()
{
  fprintf(stderr, "Error Allocating Memory!");
  exit(1);
}

void freeMemory(char** ptr, int x)
{
  for(int i = 0; i < x; i++)
    {
      free(ptr[i]);
    }
  free(ptr);
  
}

int frobcmp(const void * input1, const void * input2)
{
  char* ptr1 = *(char**)input1;
  char* ptr2 = *(char**)input2;
  

  while( *ptr1 != ' ' && *ptr2 != ' ')
    {

      if( (*ptr1^42) > (*ptr2^42))
  	return 1;
      else if( (*ptr2^42) > (*ptr1^42))
  	return -1;


      ptr1++;
      ptr2++;
    }
  

  if((*ptr1) == ' ' && (*ptr2) != ' ')
    return -1;
  else if((*ptr1) != ' ' && (*ptr2) == ' ')
    return 1;
  else
    return 0;

}

int main ()
{
  //  printf("helllllllo \n");
  char** wordcontainer = NULL; //2D array, each index holds a word
  int length = 0;

  char* word = (char*)malloc(sizeof(char)); //create current word

  if(word == NULL)
    {
      free(word);
      fprintf(stderr, "Error Allocating Memory");
      exit(1);
    }

  int lengthOfWord = 0;
  word[lengthOfWord] = getchar();
  
  if(ferror(stdin))
    {
      free(word);
      fprintf(stderr, "Input Error!");
      exit(1);
    }
  else
   {
    lengthOfWord = 1;
   }
  
  while(1)
    {
      
      char nextChar = getchar();
      
      if(ferror(stdin))
  	{
	  if(lengthOfWord > 1)
	    free(word);
  	  freeMemory(wordcontainer, length);
  	  fprintf(stderr, "Input Error!");
  	  exit(1);
  	}
      else if(nextChar == ' ')
	{

	  for(; nextChar == ' ';)
	    {
	      nextChar = getchar();
	      if(ferror(stdin))
		{
		  free(word);
		  freeMemory(wordcontainer, length);
		  fprintf(stderr, "Input Error!");
		  exit(1);
		}
	    }
	  
	  if(wordcontainer == NULL)
	    {
	      word = realloc(word, (lengthOfWord+1)*sizeof(char*));
	      if(word == NULL)
		{
		  //free(word);
		  freeMemory(wordcontainer, length);
		  printErr();
		}
	      word[lengthOfWord] = ' ';
	      wordcontainer = malloc(sizeof(char*));
	      if(wordcontainer == NULL)
		{
		  free(word);
		  freeMemory(wordcontainer, length);
		  printErr();
		}
	      wordcontainer[length] = word;
	      word = NULL;
	    }
	  else
	    {
	      
	      word = realloc(word, (lengthOfWord+1)*sizeof(char*));
	      if(word == NULL)
		{
		  freeMemory(wordcontainer, length);
		  printErr();
		}
	      word[lengthOfWord] = ' ';
	      wordcontainer = realloc(wordcontainer, (length+1)*sizeof(char*));
	      if(wordcontainer == NULL)
		{
		  free(word);
		  freeMemory(wordcontainer, length);
		  printErr();
		}
	      wordcontainer[length] = word;
	      word = NULL;

	    }

	  lengthOfWord = 0;

	  length++;

	}
      
      if (nextChar != EOF)
  	{
	  
	  if(lengthOfWord == 0)
	    word = (char*)malloc(sizeof(char)*1);
	  else
	    word = realloc(word, (lengthOfWord + 1)*sizeof(char));
	  word[lengthOfWord] = nextChar;

	  lengthOfWord++;

  	}
      else if(nextChar == EOF)
	{
	  
	  if(lengthOfWord != 0)
	    {
	      
	      word = realloc(word, (lengthOfWord+1)*sizeof(char*));
	      if(word == NULL)
		{
		  freeMemory(wordcontainer, length);
		  printErr();
		}
	      word[lengthOfWord] = ' ';
	      wordcontainer = realloc(wordcontainer, (length+1)*sizeof(char*));
	      if(wordcontainer == NULL)
		{
		  free(word);
		  freeMemory(wordcontainer, length);
		  printErr();
		}
	      wordcontainer[length] = word;
	      

	      lengthOfWord = 0;

	      length++;
	    }

	  break; //this is how i get out of the loop 
	}

    }

    qsort(wordcontainer, length, sizeof(char*), frobcmp);
  
  for(int i = 0; i < length; i++)
    {

      int j = 0;
      while(wordcontainer[i][j] != ' ')
  	{

	  putchar((wordcontainer[i][j]));
  	  j++;
  	}
      putchar(' ');

    }

  freeMemory(wordcontainer, length);

  return 0; //exit successfully!
}
