/* Author(s): PES1201700872, Aishwarya M A Ramanath
			  PES1201700301, Anagha Ananth
			  PES1201700218, Sharanya Venkat
Contents:     This file contains the code implementing a tree structure for the contents of a given directory path*/

#include<stdio.h>
#include<unistd.h>
#include<dirent.h>
#include<stdlib.h>
#include<string.h>

/* Structure to represent tree
   Attribute(s):
   		name(char):         name of pwd
   		isdir(boolean): 	returns 1 if it's a directory
   		next_dir(pointer):  pointer to the next directory
   		next_file(pointer): pointer to the next file
   		level(int):         level of node in the tree
*/
typedef struct t_node
{
    char* name;
    int isdir;		
    struct t_node *next_dir;	
    struct t_node *next_file;
    int level;
}node;

// Declaration of functions
node* create_node(char* n);
node* create_tree(char *root_name);
void print_tree(node *start);
void summary(int dir,int file);

/*Global variables
Since the display function is called recursively, the variables will keep re-initializing to 0
if not declared gloabally which is counter-productive, since we need to maintain a count per level 
in the tree.
*/

int count; 
int max;
int dir_in_path;
int files_in_path;


/* This a function to create a node of the structure declared above
   and initialize it's corresponding attributes
   Returns: created node
*/
node* create_node(char* n)
{
	node* temp=(node*)malloc(sizeof(node));
	temp->name=n;
	temp->next_dir=NULL;
	temp->next_file=NULL;
    return temp;
}

/*Function to create the directory tree, which is called recursively.
  Opens the directory specified, checks iteratively if contents are files 
  or directories : if directory found, recursively calls the function to 
  parse through the directory files.
  Argument(s): 
  	root_name(char): contains name of specified directory
  Returns: Root of created directory tree
*/
node* create_tree(char *root_name)
{
	//Opens the directory
	DIR *dir=opendir(root_name);
	

	//dr is a pointer of type dirent which is invoked from <dirent.h>
	//dr contains attributes such as inode, directory name, record length, directory type etc
	struct dirent *dr={NULL};
	node *root=NULL,*temp=NULL,*temp1=NULL;
	char *name=(char *)calloc(2000,sizeof(char));

	//Checks to see if directory exists
	if(dir==NULL)
	{
		printf("\nFailed to open ..!!");
		printf(" : %s",root_name);
		return NULL;
	}

	//While loop runs until all contents of the directory have been explored.
	while((dr=readdir(dir))!= NULL)
	{ 		
			//Every directory has two directories called "." and "..", which stand
			//for present working directory and root directory which cannot be parsed further
			if(strcmp((dr->d_name),".")!=0 && strcmp((dr->d_name),"..")!=0) 
				temp=create_node(dr->d_name);
			else
			{
				temp=NULL;
				continue;
			}
			
			if(temp1!=NULL)
			{
				temp1->next_file=temp;
			
			}

			else
			{
				root=temp;
			}

			//Checking if it's a directory
			if((dr->d_type)==DT_DIR) 
			{
				temp->isdir=1;
				strcpy(name,root_name);
				(temp->next_dir)=create_tree((strcat((strcat(name,"/")),dr->d_name)));
				
			}

			else
			{
				(temp)->isdir=0;
				(temp)->next_dir=NULL;
				
			}

			temp1=temp;

			
	}

return (root);
}

/*Display Function parses through the entire tree and prints it's contents. 
	Argument(s): root of the tree 
*/
void print_tree(node *start)
{		
		char *s="|    ",*s1="|----";
		int i;
		node *temp=start;
		if(start==NULL)
			return;

	
		temp->level=count;
		if(count > max)
			max=count;
		printf("\n");
		for(i=0;i<(count-1);i++)
			printf("%s",s);
		if(count>0)
			printf("%s",s1);
		
		printf("level:%d  %s",temp->level,temp->name);
		
		//Checks if it's a directory
		if(temp->isdir==1)
		{	
			dir_in_path+=1;
			printf("\tDIRECTORY");

			count++;
			print_tree(temp->next_dir);
			count--;
		}

		files_in_path+=1;
		print_tree(temp->next_file);

}

/* Function that summarizes number of directories, files and displays 
depth of directory tree constructed.
Argument(s): 
	dir(int): number of directories
	file(int): total number of files that exist in pwd
*/
void summary(int dir,int file)
{
	printf("\n**********SUMMARY**********\n");
	printf("Number of directories in the given path : %d\n",dir);
	printf("Number of Files in the given path : %d\n",file);
	printf("Depth of tree: %d\n", max);

}

/* Main function: implements all the above declared functions 
*/
int main(int args, char** argv)
{
	node *start=create_node(".");

	if(args>2)
	{
		printf("\n input usage : ./a.out [PATH]");
		exit(0);
	}
	
	printf("**********Listing all contents of : %s**********\n",argv[1]);
	start->isdir=1;
	(start->next_dir)=create_tree(((args==2)? argv[1]:getcwd(0,0)));
	print_tree(start);
	printf("\n");
	summary(dir_in_path,files_in_path-dir_in_path);

return 0;
}