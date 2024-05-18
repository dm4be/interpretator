#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h>

void copy_file(const char *scr, const char *dst);
void move_file(const char *scr, const char *dst);
void delete_file(const char *filename);
void list_files(const char *filename);
void remove_newline(char *str);
int main(){
	char command[256];
	while (1){
		printf("Enter command: ");
		if (fgets(command, sizeof(command), stdin) == NULL){
			perror("fgets");
			exit(EXIT_FAILURE);
		}
		command[strcspn(command, "\n")] ==0;
		if (strncmp(command, "copy ",5) == 0){
			char *scr = strtok(command +5, " ");
			char *dst = strtok(NULL, " ");
			if (scr && dst){
				copy_file(scr, dst);
			} else{
				fprintf(stderr, "Usage: copy <source> <destination>\n");
			}
		}else if (strncmp(command, "move ", 5)==0){
			char *scr = strtok(command + 5, " ");
			char *dst = strtok(NULL, " ");
			if (scr && dst){
				move_file(scr, dst);
			} else {
				fprintf(stderr, "Usage: move <source> <destination>");
			}
		} else if (strncmp(command, "delete ", 7)==0){
			char *filename = command +7;
			delete_file(filename);
		} else if (strncmp(command, "list ", 5)==0){
			char *filename = command +5;
			list_files(filename);
		} else if (strncmp(command, "exit", 4) == 0){
			break;
		} else if (strncmp(command, "info", 4) == 0) {
			printf("We have 5 command to work with files:\n");
			printf("copy: Can be used for copying content from <source> to <destination> (usage: copy <source> <destination>)\n");
			printf("move: Can be used for move file to other directories or rename file (usage: move <source> <destination>)\n");
			printf("delete: Can be used to delete file (usage: delete <filename>)\n");
			printf("list: Can be used to watch content in file (usage: list <filename>)\n");
			printf("exit: Can be used to exit from proframm.\n");

		} else {
			fprintf(stderr, "Unknown command\n");
		}
	
	}
	return 0;

}
void remove_newline(char *str){
	char *newline = strchr(str,'\n');
	if (newline) {
		*newline = '\0';
	}
}

void copy_file(const char *scr, const char *dst) {
	FILE *source, *destination;
	char buffer[1024];
	size_t bytes;

	source = fopen(scr, "rb");
	if (!source){
		perror("fopen");
		return;
	}
	
	remove_newline(dst);

	destination = fopen (dst, "wb");
	if (!destination){
		perror("fopen");
		fclose(source);
		return;
	}
	while ((bytes = fread(buffer,1,sizeof(buffer), source)) > 0) {
		fwrite(buffer, 1, bytes, destination);
	}
	fclose(source);
	fclose(destination);
}

void move_file(const char *scr, const char *dst){
	remove_newline(dst);
	remove_newline(scr);
	if (rename(scr, dst) != 0) {
		perror("rename");
	}
}

void delete_file(const char *filename){
	remove_newline(filename);
	if (remove(filename) != 0) {
		perror("remove");
	}
}
void list_files(const char *filename) {
	FILE *file;
	char buffer[1024];
	remove_newline(filename);

	file = fopen(filename, "r");
	if (!file) {
		perror("fopen");
		return;
	}

	while (fgets(buffer,sizeof(buffer), file) != NULL) {
		printf("%s", buffer);
	}
	fclose(file);

}

