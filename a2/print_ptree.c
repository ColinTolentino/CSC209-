#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "ptree.h"
#include <errno.h>
#include <ctype.h>
#include <string.h>
//Frees the whole trees
void free_Nodes(struct TreeNode *root){
    	if(root == NULL){
        	return;
    	}
    	if(root->name != NULL){
        	free(root->name);
        	root->name = NULL;
    	}
    	if(root->child_procs != NULL){
        	free_Nodes(root->child_procs);
    	}
    	if(root->next_sibling!= NULL){
        	free_Nodes(root->next_sibling);
    	}
}
//Checks if inputs are numeric values.
int check_numeric_input(char *input){
    for(int i = 0;i<strlen(input);i++){
        if(!isdigit(input[i])){
            return 1;
        }
    }
    return 0;
}

int main(int argc, char **argv) {
    char *pid_check;
    char *depth_check;
	int max_depth = 0;
	int pid = 0;
	int c;
	struct TreeNode *root = NULL;

	// TODO: Update error checking and add support for the optional -d flag

    if(argc != 2 && argc != 4){
    	printf("Usage:\n\tptree [-d N] PID\n");
    	return 1;
    }
    //loops to find the arguments
    while((c = getopt(argc,argv,"d:")) != -1){
    	switch(c){
    		case 'd':
                depth_check = optarg;
                if(strcmp(argv[1], "-d") == 0){
                    pid_check = argv[3];
                }
                else{
                    //pid is at position 1.
                    pid_check = argv[1];
                }
                //Check if inputs are numeric.
                if(check_numeric_input(pid_check) == 1 || check_numeric_input(depth_check) == 1){
                    //perror("input non numeric");
                    return 1;
                }
                max_depth = strtol(optarg,NULL,10);
                if(generate_ptree(&root, strtol(pid_check, NULL, 10)) != 1){
                    print_ptree(root, max_depth);
                }else{
                    print_ptree(root, max_depth);
                    free_Nodes(root);
                    //perror("error in generate_ptree");
                    return 2;
                }
                

    			break;
    		case '?':
    			printf("Usage:\n\tptree [-d N] PID\n");
    			return 1;
    	}
    }
    //Checks if the inputs are numbers
    pid_check = argv[1];
    if(check_numeric_input(pid_check) == 1){
        //perror("non numeric number in pid");
        return 1;
    }
    pid = strtol(argv[1],NULL,10);
    
    if(generate_ptree(&root,pid) != 1){
        print_ptree(root,0);
    }else{
        print_ptree(root,0);
        //perror("error in generate_ptree");
        free_Nodes(root);
        return 2;
    }
    // NOTE: This only works if no -d option is provided and does not
    // error check the provided argument or generate_ptree. Fix this!
    //Free all the allocated memory
    free_Nodes(root);
    return 0;
}

