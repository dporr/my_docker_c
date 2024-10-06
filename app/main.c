#include <stdio.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/mount.h>
#include <sys/stat.h>
#include "util.h"

// Usage: your_docker.sh run <image> <command> <arg1> <arg2> ...
int main(int argc, char *argv[]) {
	// Disable output buffering
	setbuf(stdout, NULL);

	// You can use print statements as follows for debugging, they'll be visible when running tests.
	//printf("Logs from your program will appear here!\n");

	// Uncomment this block to pass the first stage
	//
	char *command = argv[3];
	int status_code = 0;
	int child_pid = fork();
	if (child_pid == -1) {
	    printf("Error forking!");
	    return 1;
	}
	
	if (child_pid == 0) {
		change_mount();		// change_monut();
		// Replace current program with calling program
	    int execv_e = execv(command, &argv[3]); //&argv[3] == <command> <arg1> <arg2> ...
		if(execv_e == -1){
			perror("execv");
			exit(errno);
		}
	} else {
		   // We're in parent
		   wait(&status_code);
		   exit(WEXITSTATUS(status_code));
		   //printf("Child terminated");
	}

	return 0;
}

/* takes a @path and creates it recursively
like mkdir -p <path> in the terminal
*/
int mkdir_recursive(char* path){
	//mutable copy of the original path
    char mut_p[1024];
    snprintf(mut_p, sizeof(mut_p), "%s", path);
	for(int i = 0; i < strlen(mut_p) && mut_p[i] != '\0' ; i++) {
		if(mut_p[i] == '/' && i != 0 ) {
			mut_p[i] = '\0';
			//printf("created %s\n", mut_p);
			if(mkdir(mut_p, S_IRWXU) == -1 && errno != EEXIST) {
				perror("mkdir");
				exit(errno);
			}
			mut_p[i] = '/';
		}
	}
	//printf("created %s\n", mut_p);
	if(mkdir(mut_p, S_IRWXU) == -1 && errno != EEXIST) {
		perror("mkdir");
		exit(errno);
	}
	return 0;
}

int change_mount()
{
	char cwd[1024];
	getcwd(cwd, 1024);
	/*mount /usr/local/bin/ to have acces to docker-explorer*/
	const char* source="/usr/local/bin/";
	char target[2048];
	sprintf(target,"%s%s", cwd, source);
	mkdir_recursive(target);
	int mount_e = mount(source, target,
 		 				"none", MS_BIND, NULL);
	if(mount_e == -1) {
		perror("mount");
		exit(errno);
	}
	//printf("cwd: %s\n", cwd);
	chroot(cwd);
}