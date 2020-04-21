#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>

void memoryerror()
{
  fprintf(stderr, "Error Allocating Memory\n");
}

void readerr()
{
  fprintf(stderr, "Error Reading Data\n");
}

int frobcmpf(const void * input1, const void * input2)
{
  char* ptr1 = *(char**)input1;
  char* ptr2 = *(char**)input2;
  while( *ptr1 != ' ' && *ptr2 != ' ')
    {
      if(toupper((unsigned char)(*ptr1^42)) > toupper((unsigned char)(*ptr2^42)))
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

int main(int argc, char** argv)
{
  if(argc > 2)
    {
      fprintf(stderr,"Invalid number of Arguments!\n");
      exit(1);
    }

  const char* flag = argv[1];
  int use_fFlag = 0;
  if(argc == 2 )
    {
      if(strcmp(flag, "-f") == 0)
	use_fFlag = 1;
      else
        {
	  fprintf(stderr, "Invalid Argument\n");
	  exit(1);
        }
    }
  struct stat buffer;
  int err = fstat(0, &buffer);

  if(err < 0)
    {
      fprintf(stderr, "Buffer Error\n");
      exit(1);
    }
  char** container;
  int numWords = 0;

  if(S_ISREG(buffer.st_mode))
    {

      int filesize = buffer.st_size + 2;
      char* filedata = (char*)malloc(sizeof(char) * (filesize)); //so i can add EOF and Space

      if(filedata == NULL)
        {
	  memoryerror();
	  exit(1);
        }

      filedata[buffer.st_size] = ' ';
      filedata[buffer.st_size + 1] = EOF;

      int status = read(0, filedata, filesize); //should have the file contents in filedata

      if(status < 0) //error
        {
	  readerr();
	  free(filedata);
	  exit(1);
        }

      //count number of words

      int newWord = 1;
      int* wordLengths = malloc(sizeof(int)*1);
      int wls = 0;
      int wordLength = 0;

      for(int i = 0; i < filesize; )
        {
	  if(filedata[i] == EOF)
	    {
	      //printf("(%d) %d", i, filesize);
	      break;
	    }

	  if(filedata[i] != ' ')
            {
	      wordLength++;
            }
	  if(filedata[i] != ' ' && newWord == 1)
            {
	      numWords++;
	      newWord = 0;
            }
	  else if(filedata[i] == ' ')
            {
	      if(wordLength != 0)
                {
		  wordLengths[wls] = wordLength+1;
		  wls++;
		  wordLengths = realloc(wordLengths, sizeof(int)*(wls+1));
		  if(wordLengths == NULL)
                    {
		      memoryerror();
		      free(filedata);
		      exit(1);
                    }

                }
	      wordLength = 0;
	      newWord = 1;
            }
	  char idk;
	  i++;
	  if(i == filesize - 1)
	    {
	      //printf("123asdasda");
	      idk = filedata[i];
	      err = fstat(0, &buffer);

	      if(err < 0)
                {
		  fprintf(stderr, "Buffer Err\n");
		  exit(1);
                }

	    }


	  if(i == (filesize-1) && buffer.st_size != (filesize - 2))
            {
	      err = fstat(0, &buffer);
	      if(err < 0)
                {
		  fprintf(stderr, "Buffer Err\n");
		  exit(1);
                }
	      filesize = buffer.st_size + 2;
	      //allocate the memory
	      filedata = realloc(filedata, sizeof(char) * filesize);
	      if(filedata == NULL)
                {
		  memoryerror();
		  free(wordLengths);
		  exit(1);
                }

	      int begin = lseek(0, 0, SEEK_SET);
	      int stat = read(0, filedata, filesize);
	      if(stat < 0)
                {
		  readerr();
		  free(wordLengths);
		  free(filedata);
		  exit(1);
                }

	      filedata[buffer.st_size] = ' ';
	      filedata[buffer.st_size + 1] = EOF;
      newWord = 1;
      wordLengths = (int*)realloc(wordLengths, sizeof(int)*1);
      wls = 0;
      wordLength = 0;
      numWords = 0;

	      i = 0;
            }
        }
      //now I know how many pointers to allocate!
      container = (char**)malloc(sizeof(char*) * numWords);
      //
      if(container == NULL)
        {
	  memoryerror();
	  //free(container);
	  free(filedata);
	  exit(1);
        }

      int wordsAdded = 0;
      //printf("%d", filesize);
      for(int i = 0; i < filesize; i++)
        {
	  if(wordsAdded == numWords)
	    break;

	  char* curword = malloc(sizeof(char)*wordLengths[wordsAdded]); //account for the space!
	  if(curword == NULL)
            {
	      for(int j = 0; j < numWords; j++)
                {
		  free(container[j]);
                }
	      free(container);
	      free(filedata);
	      memoryerror();
	      exit(1);
            }
	  //printf("%d ", wordLengths[wordsAdded]);
	  if(filedata[i] != ' ')
	    for(int j = 0; j < wordLengths[wordsAdded]; i++)
	      {
		curword[j] = filedata[i];
		//		putchar(curword[j]);
		j++;
	      }
	  i--;
	  //printf("%d ", wordsAdded);
	  //	  for(int k = 0; curword[k] != ' '; k++)
	  // {
	  //  printf("%c", curword[k]);
	  // }
	  //printf(" ");
	  container[wordsAdded] = curword;
	  wordsAdded++;
        }
      free(filedata);
      free(wordLengths);
    }
  else
    {
      int size = 0;
      char* word;
      int cont = 0;
      while(1)
        {
	  char c[1];
	  int status = read(0, c, 1);

	  if(status < 0)
            {
	      readerr();
	      exit(1);
            }

	  if(c[0] != ' ' && status > 0)
            {
	      cont = 0;
	      if(size != 0)
                {
		  word = (char*)realloc(word, sizeof(char)*(size + 1));
                }
	      else
                {
		  word = (char*)malloc(sizeof(char)*(size + 1));
                }

	      if(word == NULL)
                {
		  for(int i = 0; i < numWords; i++)
                    {
		      free(container[i]);
                    }
		  free(container);
		  memoryerror();
		  exit(1);
                }
	      word[size] = c[0];
	      //putchar(word[size]);
	      size++;
            }
	  else if(c[0] == ' ' || status == 0)
            {
	      if(cont == 1)
                {
		  continue;
                }
	      if(size > 0)
                {
		  if(numWords > 0)
                    {
		      container = (char**)realloc(container, sizeof(char*)*(numWords+1));
                    }
		  else
                    {
		      container = (char**)malloc(sizeof(char*)*(numWords+1));
                    }

		  if(container == NULL)
                    {
		      memoryerror();
		      free(word);
		      exit(1);
                    }

		  word = (char*)realloc(word, sizeof(char)*(size+1));
		  if(word == NULL)
                    {
		      memoryerror();
		      for(int i = 0; i < numWords; i++)
                        {
			  free(container[i]);
                        }
		      free(container);
		      exit(1);
                    }
		  word[size] = ' ';
		  container[numWords] = word;
		  size = 0;

		  numWords++;

		  if(status == 0)
                    {
		      break;
                    }
		  else
                    {
		      cont = 1;
                    }
                }
            }
        }
    }

  if(use_fFlag == 0)
  qsort(container, numWords, sizeof(char*), frobcmp);
  else
  qsort(container, numWords, sizeof(char*), frobcmpf);

  for(int i = 0; i < numWords; i++)
    {
      for(int j = 0; container[i][j] != ' '; j++)
	{
	  int out[1];
	  out[0] = container[i][j];
	  //	  int wr = 5;
	  int wr = write(1, &container[i][j], 1);
	  //printf("%c", (int) container[i][j]);
	  if(wr < 0)
	    {
	      fprintf(stderr, "Output Error\n");
	      for(; i < numWords; i++)
		{
		  free(container[i]);
		}
	      free(container);
	      exit(1);
	    }
	}
      int out[1];
      out[0] = ' ';
      //printf(" ");
      int wr = write(1, &out[0], 1); //(' ');
      free(container[i]);
    }

  free(container);
}
