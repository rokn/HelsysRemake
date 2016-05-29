//--------------------------------------------
// NAME: Yassen Alexiev
// CLASS: XIa
// NUMBER: 29
// PROBLEM: #4
// FILE NAME: ls
// FILE PURPOSE:
// Целта на задачате е да се реализира стандартната UNIX комадна ls с добавена функционалност.
//Командата ls се използва за извеждане на съдържанието на директории. От командния
//ред програмата получава списък от имена на файлове и/или директории, а на стандартния
//изход извежда информация за тях. Ако подаденото име, е съществуващ файл, се извежда
//името му и неговия тип, а ако името е на директория - имената и типовете на всички
//файлове, съдържащи се в директорията.
// ...
//---------------------------------------------


#include <stdio.h> 
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <unistd.h>
#include <string.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>

void directory_process(char* dir);

//--------------------------------------------
// STRUCT: flags_t
// По-лесно използване на флаговете
// PARAMETERS:
// l - за flag_l
// a - за flag_a
// R - за flag_R
//----------------------------------------------
struct flags_t {
	int l;
	int a;
	int R;
};

struct flags_t flags;

//--------------------------------------------
// FUNCTION: is_file
// Проверка дали подадения път е файл
// PARAMETERS:
// path - подавам string и проверявам дали той е 
// файл, използвайки stat
//----------------------------------------------

int is_file(const char *path)
{
    struct stat path_stat;
    stat(path, &path_stat);
    return S_ISREG(path_stat.st_mode);
}

//--------------------------------------------
// FUNCTION: file_info
// Принта информация за файловете и директориите 
// в настоящата директория
// PARAMETERS:
// Подавам buffer и спрямо него извличам информацията 
// за всеки един файл и директория  
//----------------------------------------------

void file_info(struct stat* buffer){
        printf( (buffer->st_mode & S_IRUSR) ? "r" : "-");
	    printf( (buffer->st_mode & S_IWUSR) ? "w" : "-");
		printf( (buffer->st_mode & S_IXUSR) ? "x" : "-");
		printf( (buffer->st_mode & S_IRGRP) ? "r" : "-");
		printf( (buffer->st_mode & S_IWGRP) ? "w" : "-");
		printf( (buffer->st_mode & S_IXGRP) ? "x" : "-");
		printf( (buffer->st_mode & S_IROTH) ? "r" : "-");
		printf( (buffer->st_mode & S_IWOTH) ? "w" : "-");
		printf( (buffer->st_mode & S_IXOTH) ? "x" : "-");

      printf(" %ld", (long)buffer->st_nlink);

      struct passwd *own = getpwuid(buffer->st_uid); 
      printf(" %s", own->pw_name); //passwd - взимам паролата, за да извлека името на user-a с own->pw_namе
      struct group *gr = getgrgid(buffer->st_gid); 
      printf(" %s", gr->gr_name); // group - взимам id-то на групата и след това принтя името й с gr->gr_name

      printf(" %lld", (long long)buffer->st_size); //принтя размера на директорията или файла в байтове с buffer->st_size

      char date[14]; // заделям памет за string в който да се изпринти датата на създаване или последна модишикация
      strftime(date, 14, " %m %d %H:%M", localtime(&(buffer->st_mtime))); // подавам правилния формат на датата
      printf("%s", date); // принта я
}


//--------------------------------------------
// FUNCTION: file_process
// Обработване на даден файлл 
// PARAMETERS:
// char* filename - името на файла
// char* directory - директорията на файла
//----------------------------------------------
void file_process (char* filename, char* directory) {
  
  char* path = malloc(sizeof(char) * (strlen(directory) + strlen(filename) + 2));
  // char* path - заделяме достатъчно памет за името на пътя
  path[0] = '\0';
  strcat(path, directory); // добавяме в path - директорията
  strcat(path, "/"); // добавяме в path - /
  strcat(path, filename); // добавяме в path - името на файла

  if (flags.a == 0 && filename[0] == '.') {
    return;
  }

  struct stat buffer;
  
  if (stat(path, &buffer) == -1) {
    perror(path); // връща обработената грешка 
	return; 
  }

  switch (buffer.st_mode & S_IFMT) {
    case S_IFBLK:  printf("b");            
      break;
    case S_IFCHR:  printf("c");        
      break;
    case S_IFDIR:  printf("d");               
      break;
    case S_IFIFO:  printf("p");               
      break;
    case S_IFLNK:  printf("l");                 
      break;
    case S_IFREG:  printf("-");            
      break;
    case S_IFSOCK: printf("s");                  
      break;
    default:
      printf("??????\n");
      break;
    }

    if (flags.l == 1) {
    	file_info(&buffer);
	}

  printf(" %s\n",filename); // принтя името на файла

  if (flags.R != 0) {
    if ((buffer.st_mode & S_IFMT) == S_IFDIR) {
      if(!(filename[0] == '.' || filename[1] == '.')) {
        directory_process(path);
      }
    }
  }
  free (path); // освобождавам паметта заделена за path 

}

//--------------------------------------------
// FUNCTION: directory_process
// Обработка на файловете и директориите в настоящата директория
// PARAMETERS:
// char* directory - подавам името на директорията 
//----------------------------------------------
void directory_process(char* directory) {
  DIR *dir;
   if ((dir = opendir(directory)) == NULL){ // ако няма директория с такова име
    perror(directory); // връща обработената грешка
    return;
   }
    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL) {
      file_process(entry->d_name, directory); // вика функцията file_process с името на настоящия файл/директоря и директорията в която се намира
    }
  closedir(dir); // затваря директорията
}

//--------------------------------------------
// FUNCTION: main
// Стартиране на програмата
// PARAMETERS:
// int argc - броя на аргументите подавани на програмата
// char* argv[] - масив с подадените елементи на програмата
//----------------------------------------------
int main(int argc, char *argv[]) { 
 int opt = 0; // зануляване на opt - опция 

 flags.a = 0; // зануляване на флага "а"
 flags.l = 0; // зануляване на флага "l"
 flags.R = 0; // зануляване на флага "R"
 
 while((opt = getopt(argc,argv, "laR")) != -1) { // въртин цикъла докато опция няма
  
  switch ((char)opt) {
   case 'a' : {
    flags.a = 1; //set-вам флага "а" на 1 
    break; 
   }
   case 'l' : {
    flags.l = 1; //set-вам флага "l" на 1
    break;
   }
   case 'R' : {
    flags.R = 1; //set-вам флага "R" на 1
    break;
   }
  }
 }
 if (optind >= argc) {
  directory_process("."); // викам функцияна directory_process с аргумент настоящата директория
 }

 int i = 0;
  for (i = optind; i < argc; i++) {
    if (is_file(argv[i])) {
      file_process(argv[i], ""); // извикване на функцията file_process с агрументи настоящото argv и директорията 
    }
    else {
      directory_process(argv[i]); // извикване на функцията directory_process с настоящото argv
    }
 }

 return 0;
}
