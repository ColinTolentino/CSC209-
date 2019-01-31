#include <stdio.h>
#include <stdlib.h>
#include <string.h>


// TODO: Implement a helper named check_permissions that matches the prototype below.
int check_permissions(char *permissions, char *file_permissions){
    int check; 
    if(file_permissions[0] == 'd'){ //Checks if it is a directory
        return 0;
    }
    for(check = 0;check < 9; check++){
        if(permissions[check] != '-'){
            //Start at file_permissions at position 1 since we ignore directories
            if(permissions[check] != file_permissions[check+1]){
                return 0;
            }
        }
    }
    return 1;
}


int main(int argc, char** argv) {
    char permission_requirements[10]; // 9 elements in the permissions
	char file_permissions[11]; // 10 possible elements in the permissions
    int size_of_file;
    int size_limit;
    int permissions_count = 0;     

    if (!(argc == 2 || argc == 3)) {
        fprintf(stderr, "USAGE:\n\tcount_large size [permissions]\n");
        return 1;
    }
    // TODO: Process command line arguments.
    scanf("%*s %*d"); //Skips the first line
    size_limit = strtol(argv[1],NULL,10);
    if(argc == 2){
        while(scanf("%*s %*s %*s %*s %d %*d %*s %*s %*s",&size_of_file) != EOF){
            if(size_of_file > size_limit){
                permissions_count++;
            }
        }
    }
    // TODO Call check permissions and then print the returned value.
    if(argc == 3){ //If permissions are called in.
        permissions_count = 0; //Reset permission_count since we are now accounting for permissions.
        strcpy(permission_requirements,argv[2]);
        while(scanf("%s %*s %*s %*s %d %*d %*s %*s %*s",file_permissions,&size_of_file) != EOF){
            if(check_permissions(permission_requirements,file_permissions) == 1 && size_of_file > size_limit){
                permissions_count++;
            }
        }
    }
    printf("%d\n", permissions_count);
    return 0;
}
