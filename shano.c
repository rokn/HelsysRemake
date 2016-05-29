#include <stdio.h>
#include <time.h>
#include <pwd.h>
#include <grp.h>
#include <sys/types.h>
#include <dirent.h>
#include <unistd.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <string.h>

int flag_l = 0;
int flag_a = 0;
int flag_R = 0;

void directory_go_through(char* dirname);

int is_regular_file(const char *path)
{
    struct stat path_stat;
	stat(path, &path_stat);
	return S_ISREG(path_stat.st_mode);
}

void file_printing(char* dir, char* filename)
{
	char* filepath = malloc(sizeof(char) * (strlen(dir) + strlen(filename)+ 2));
	filepath[0] = '\0';

	strcat(filepath, dir);
	strcat(filepath, "/");
	strcat(filepath, filename);
	
	if(flag_a == 0 && filename[0] == '.')
	{
		return;
	}
	struct stat buff;
	if((stat(filepath, &buff) == -1))
	{
		perror(filepath);
		return;
	}

	
	switch (buff.st_mode & S_IFMT){
	case S_IFBLK: 
		printf("b");            
		break;
	case S_IFCHR:  
		printf("c");        
		break;
	case S_IFDIR:  
		printf("d");               
		break;
	case S_IFIFO:  
		printf("p");               
		break;	
	case S_IFLNK:  
		printf("l");                 
		break;
	case S_IFREG:  
		printf("-");            
		break;
	case S_IFSOCK: 
		printf("s");                  
		break;
	default:
		printf("????");                
		break;
	}
	
	if(flag_l == 1)
	{
		if(buff.st_mode & S_IRUSR)
		{
			printf("r");
		}
		else
		{
			printf("-");
		}
		if(buff.st_mode & S_IWUSR)
		{
			printf("w");
		}
		else
		{
			printf("-");
		}
		if(buff.st_mode & S_IXUSR)
		{
			printf("x");
		}
		else
		{
			printf("-");
		}
		if(buff.st_mode & S_IRGRP)
		{
			printf("r");
		}
		else
		{
			printf("-");
		}
		if(buff.st_mode & S_IWGRP)
		{
			printf("w");
		}
		else
		{
			printf("-");
		}
		if(buff.st_mode & S_IXGRP)
		{
			printf("x");
		}
		else
		{
			printf("-");
		}
		if(buff.st_mode & S_IROTH)
		{
			printf("r");
		}
		else
		{
			printf("-");
		}
		if(buff.st_mode & S_IWOTH)
		{
			printf("w");
		}
		else
		{
			printf("-");
		}
		if(buff.st_mode & S_IXOTH)
		{
			printf("x");
		}
		else
		{
			printf("-");
		}

		printf(" %ld",(long)buff.st_nlink);

		struct passwd *owner = getpwuid(buff.st_uid); // show user name
		printf(" %s", owner->pw_name);

		struct group  *group = getgrgid(buff.st_gid);
		printf(" %s", group->gr_name);

		printf(" %lld", (long long)buff.st_size);

		char date[12];
		strftime(date, 12, "%m %d %H:%M", localtime(&(buff.st_mtime)));
		printf(" %s", date);

	}

	printf(" %s\n", filename);

	if(flag_R != 0)
	{
		if((buff.st_mode & S_IFMT) == S_IFDIR)
		{
			if(!(filename[0] == '.' || filename[1] == '.'))
			{
				directory_go_through(filepath);
			}
		}

	}

}

void directory_go_through(char* dirname)
{
	DIR *dir;

	if((dir = opendir(dirname)) == NULL)
	{
		perror(dirname);
		return;
	}

	struct dirent *entry;
	while ((entry = readdir(dir)) != NULL) 
	{
		file_printing(dirname, entry->d_name);
	}
	closedir(dir);
}


int main(int argc, char * const argv[])
{
	int option = 1;

	while(option != -1)
	{
		option = getopt(argc, argv, "laR");
		if(option != -1)
		{
			switch((char)option)
			{
				case 'a':
				{
					flag_a = 1;
					break;
				}
				case 'l':
				{
					flag_l = 1;
					break;
				}
				case 'R':
				{
					flag_R = 1;
					break;
				}
			}
		}
	}

	if(optind >= argc)
	{
		directory_go_through(".");
	}
	int i = 0;

	for(i = optind; i < argc; i++)
	{

		if(is_regular_file(argv[i]))
		{
			file_printing("",argv[i]);				
		}
		else
		{
			directory_go_through(argv[i]);
		}

	}

	return 0;
}
