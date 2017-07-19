//mysort -r -n (lines) -filename
//reads n lines from file and displays them in sorted order

//int main(int numargs, char *args[])
	//use man 3 getopt. to parse args
	//getopt()

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define BUFF 1024

static char **lines;
static int numLines = 0;
static int maxLines = 2;

void freeArray(char **array, int size)
{
    for (int i = 0; i < size; i++)
	{
		free(array[i]);
		array[i] = NULL;
	}
	free(array);
}

//courtesy of linux man page website
static int cmpstr(const void *p1, const void *p2)
{
	return strcmp(*(char * const *) p1, *(char * const *) p2);
}

int readFile(char *filename)
{
	FILE *file;
	if ((file = fopen(filename, "r")) != NULL)
	{
		int i = 0;
		while (fgets(lines[i], BUFF, file) != NULL)  //places a full line up to and including newline char into str
		{	
			i++;
			if (i == maxLines) 
			{
				int newMax = (maxLines * 2);
				char **newPtr = malloc(newMax * sizeof(char*));
				if (newPtr == NULL)
				{
					freeArray(lines, i);
					return 1;
				}
				for (int j = 0; j < newMax + 1; j++)
				{
					if ((newPtr[j] = malloc(BUFF)) == NULL) return 1;
				}
				for (int j = 0; j < i; j++)
				{
					newPtr[j] = lines[j];
				}
				maxLines = newMax;
				lines = newPtr;		
			}	
		}
		qsort(lines, i, sizeof(char*), cmpstr);
		numLines = i;
		fclose(file);
		return 0;
	}
	else return 1;
}

int main (int argc, char **argv)
{
  int reverseFlag = 0;
  int readFlag = 0;
  int numFlag = 0;
  int numOpts = 0;
  char *nValue = NULL;
  char *name = NULL;
  long ret;
  int c;
  int i;

  opterr = 0;
  while((c = getopt (argc, argv, "rn:")) != -1)
  {
    switch (c)
      {
      case 'r':
        reverseFlag = 1;
		numOpts++;
        break;
		
      case 'n':
        nValue = optarg;
		char *ptr;
		ret = strtol(nValue, &ptr, 10);
		if (ret < 0)
		{
			fprintf(stderr, "Invalid argument for -n option.\n");
			exit(1);
		}
		numFlag = 1;
		numOpts++;
		if (*ptr == 'r')
		{
			reverseFlag = 1;
			numOpts++;
		}
        break;
		
      case '?':
        if (optopt == 'n')
          fprintf(stderr, "Option -%c requires an argument.\n", optopt);
        else if (isprint(optopt)) fprintf(stderr, "Unknown option -%c.\n", optopt);
		exit(1);
		
      default:
		fprintf(stderr, "Unknown error.\n");
        exit(1);
      }
  } 
 
 switch (numOpts)
 {
	case 0:
		if ((argc == 2) || (argc == 1))
		{
			name = argv[optind];
			readFlag = 1;
		}
		else 
		{
			fprintf(stderr, "Improper arguments.\n");
			exit(1);
		}
		break;
	case 1:
		if (argc == 3)
		{
			name = argv[optind];
			readFlag = 1;
		}
		else 
		{
			fprintf(stderr, "Improper arguments.\n");
			exit(1);
		}
		break;
	case 2:
		if ((argc < 5) && (argc > 1))
		{
			if (argc == 2) break;
			name = argv[optind];
			readFlag = 1;
		}
		else 
		{
			fprintf(stderr, "Improper arguments.\n");
			exit(1);
		}
		break;
	default:
		fprintf(stderr, "Unknown error.\n");
        exit(1);
 }
 
 if (argc > 4)
 {
	 fprintf(stderr, "Too many arguments.\n");
     exit(1);
 }
 
  //dynamically allocate lineCount rows of size (char *) each
  if ((lines = malloc(maxLines * sizeof(char*))) == NULL) exit(1);
  //dynamically allocate each row of line array with 1024 bytes of space
  for (i = 0; i < maxLines; i++)
  {
	if ((lines[i] = malloc(BUFF)) == NULL) exit(1); 
  }
  
  if (readFlag) 
  { 
	if (readFile(name) != 0) //fills **lines with sorted info
	{
		fprintf(stderr, "Unable to open file.\n");
		exit(1);
	}
  }
  else 
  {
	  i = 0;
	  while(fgets(lines[i], BUFF, stdin)) 
	  {
		i++;
		if (i == maxLines) 
		{
			int newMax = (maxLines * 2);
			char **newPtr = malloc(newMax * sizeof(char*));
			if (newPtr == NULL)
			{
				freeArray(lines, i);
				exit(1);
			}
			for (int j = 0; j < newMax + 1; j++)
			{
				if ((newPtr[j] = malloc(BUFF)) == NULL) exit(1);
			}
			for (int j = 0; j < i; j++)
			{
				newPtr[j] = lines[j];
			}
			maxLines = newMax;
			lines = newPtr;		
		}	
	  }
	  numLines = i;
  }
  
  
  
  if (reverseFlag) 
  {
	  if (numFlag == 1)
	  {
		  if (ret > numLines)
		  {
			  fprintf(stderr, "Invalid number of lines requested.\n");
			  freeArray(lines, numLines);
			  exit(1);
		  }
		  for (int num = numLines; num > (numLines - (ret + 1)); num--)
		  {
			printf("%s", lines[num]);
		  }
		  freeArray(lines, numLines);
		  exit(0);
	  }
	  else
	  {
		  for (int num = numLines + 1; num > -1; num--)
		  {
			printf("%s", lines[num]);
		  }
		  freeArray(lines, numLines);
		  exit(0);
	  }
  }
  else
  {
	if (numFlag == 1)
	{
		if (ret > numLines)
		{
			fprintf(stderr, "Invalid number of lines requested.\n");
			freeArray(lines, numLines);
			exit(1);
		}
		for (int num = 0; num < ret; num++)
		{
			printf("%s", lines[num]);
		}
		freeArray(lines, numLines);
		exit(0);
	}
	else
	{
		for (int num = 0; num < numLines; num++)
		{
			printf("%s", lines[num]);
		}
	}
	freeArray(lines, numLines);
    exit(0);
  }
}
		  