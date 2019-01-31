#include <stdio.h>
// Add your system includes here.
#include <stdlib.h>
//lstat includes
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
//
#include <string.h>

#include "ptree.h"

// Defining the constants described in ptree.h
const unsigned int MAX_PATH_LENGTH = 1024;

// If TEST is defined (see the Makefile), will look in the tests 
// directory for PIDs, instead of /proc.
#ifdef TEST
    const char *PROC_ROOT = "tests";
#else
    const char *PROC_ROOT = "/proc";
#endif


/*
 * Creates a PTree rooted at the process pid. The root of the tree is
 * placed in root. The function returns 0 if the tree was created 
 * successfully and 1 if the tree could not be created or if at least
 * one PID was encountered that could not be found or was not an 
 * executing process.
 */

int generate_ptree(struct TreeNode **root, pid_t pid) {
    // Here's a trick to generate the name of a file to open. Note 
    // that it uses the PROC_ROOT variable
    FILE *fp;
    struct stat fileStat;
    struct stat fileStat2;
    struct stat fileStat3;
    char buffer[MAX_PATH_LENGTH];
    int child_pid;
    char procfile[MAX_PATH_LENGTH + 1];
    char cmdline_file[MAX_PATH_LENGTH + 1];
    char children_file[MAX_PATH_LENGTH + 1];
    int error;
    static int error_check = 0; // for checking if one of the children fails.

    if (sprintf(procfile, "%s/%d/exe", PROC_ROOT, pid) < 0) {
        fprintf(stderr, "File %d: sprintf failed to produce a filename\n",pid);
        return 1;
    }
    //Creates a filename for cmdline
    if (sprintf(cmdline_file, "%s/%d/cmdline", PROC_ROOT, pid) < 0) {
        fprintf(stderr, "sprintf failed to produce a filename\n");
        return 1;
    }
    // Your implementation goes here.
    //We start implementing the node.
    *root = malloc(sizeof(struct TreeNode));
    if(root == NULL){
        //malloc failed to run
        return 1;
    }
    struct TreeNode *current_node = *root;
    //File exe does not exist.
    if(lstat(procfile,&fileStat) < 0 || lstat(cmdline_file,&fileStat2) < 0){
        //fprintf(stderr, "File %d: exe or cmdline file does not exist\n", pid);
        free(*root);
        *root = NULL;
        return 1;
    }
    //File exe and cmdline exists.
    else{
        //Checks if file is regular
        if(S_ISREG(fileStat.st_mode) != 0 && S_ISREG(fileStat2.st_mode) != 0){
            current_node->pid = pid;
            current_node->child_procs = NULL;
            current_node->next_sibling = NULL;
        }
        else{
            //File is not a regular.
            free(*root);
            *root = NULL;
            //fprintf(stderr, "File %d: exe or cmd file is not regular\n", pid);
            return 1;
        }
    }
    //Now we open and check the cmdline
    fp = fopen(cmdline_file,"r");
    if(fp != NULL){
        //Stores the first element in cmdline into buffer.
        if(fscanf(fp,"%s",buffer) != 1){
            current_node->name = NULL;
            //fprintf(stderr, "File %d: has a empty cmdline file");
            return 1;
        }

        //Gets the name in the case there is a path.
        char *last = strrchr(buffer,'/');
        if(last != NULL){
            if((current_node->name = malloc(strlen(last+1)+1)) == NULL){
                return 1; //malloc failed.
            }
            strcpy(current_node->name,last+1);
        }else{
            if((current_node->name = malloc(strlen(buffer)+1)) == NULL){
                return 1; //malloc failed.
            }
            strcpy(current_node->name,buffer);
        }
        error = fclose(fp);
        if(error!=0){
            //Failed to close the file.
            //fprintf(stderr, "File %d: Failed to close children file\n",pid);
            return 1;
        }
    }
    else{//File failed to open
        //perror("Failed to open cmdline file");
        return 1;
    }
    //Now we check if there are children.
    if (sprintf(children_file, "%s/%d/task/%d/children", PROC_ROOT, pid, pid) < 0) {
        //fprintf(stderr, "File %d: sprintf failed to produce a filename\n",pid);
        return 1;
    }else{//File children_file does not exist.
        if(lstat(children_file,&fileStat3) < 0){
            //fprintf(stderr, "File %d: Children file does not exist",pid);
            return 1;
        }else{//There is a regular children file
            if(S_ISREG(fileStat3.st_mode) == 0){
                //perror("Children file is not regular");
                return 1;
            }
            //Now we open the children.
            fp = fopen(children_file,"r");
            if(fp != NULL){
                //Go through each child and link.
                struct TreeNode *child_node = NULL;
                if(fscanf(fp,"%d",&child_pid) == 1){
                    //Recursively call to build child node before linking.
                    if(generate_ptree(&child_node,child_pid) ==1){
                        error_check = 1;
                    }
                    //Before we link we make sure child_node exists or else we skip
                    if(child_node != NULL){
                        current_node->child_procs = child_node;
                        //links the current node to its child node.
                        current_node = current_node->child_procs;
                        //Changes the pointer to the child node and loops.
                    }
                }
                //Sets the next sibling to NULL before checking if there are siblings.
                while(fscanf(fp,"%d",&child_pid) == 1){
                    struct TreeNode *sibling_node = NULL;
                    //Recursively call to build sibling node before linking.
                    if(generate_ptree(&sibling_node,child_pid) ==1){
                        error_check = 1;
                    }
                    //We make sure sibling node exists before we link or else we skip it.
                    if(sibling_node!= NULL){
                        //links the current node to its sibling node
                        current_node->next_sibling = sibling_node;
                        //Changes the pointer to the sibling node and loops.
                        current_node = current_node->next_sibling;
                    }     
                }
            }else{//File failed to open children file.
                //fprintf(stderr, "File %d: Failed to open children file",pid);
                return 1;
            }
            //Either loop finished or 
            error = fclose(fp);
            if(error!=0){
                //fprintf(stderr, "File %d: Failed to close children file\n",pid);
                return 1;
            }    
        }
    }
    return error_check;
}
/*
 * Prints the TreeNodes encountered on a preorder traversal of an PTree
 * to a specified maximum depth. If the maximum depth is 0, then the 
 * entire tree is printed.
 */
void print_ptree(struct TreeNode *root, int max_depth) {
    // Here's a trick for remembering what depth (in the tree) you're at
    // and printing 2 * that many spaces at the beginning of the line.
    static int depth = 0;
    if(root == NULL){
        return;
    }

    //Print the whole tree since max depth is 0
    if(max_depth == 0 || depth<max_depth){ 
        printf("%*s", depth * 2, "");
        //printing the process id and name
        if(root->name != NULL){
            printf("%d: %s\n", root->pid, root->name);
        }else{
            printf("%d\n", root->pid);
        }

        //printing our siblings.
        if(root->child_procs != NULL){
            depth++;
            print_ptree(root->child_procs,max_depth);
            depth--;
        }
        if(root->next_sibling != NULL){
            print_ptree(root->next_sibling,max_depth);
            }
    }
}