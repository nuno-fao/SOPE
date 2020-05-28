#include "funcs.h"
#include "registers.h"

void sigint_handler(int sig) {

	writeRecvSignalEvent(sig);
    int target = getppid();
     char  c;
	writeSendSignalEvent(target, SIGSTOP);
	
     signal(sig, SIG_IGN);
	 kill(target, SIGSTOP);
	 

     printf("\nSIGINT activado\n"
            "Deseja mesmo encerrar?\n"
			"Escreva s ou S para encerrar, qualquer outra tecla para continuar.\n");
     c = getchar();
     if (c == 'S' || c == 's'){
		signal(sig, SIG_DFL);
		  writeSendSignalEvent(target, SIGCONT);
		  kill(target, SIGCONT);
          writeSendSignalEvent(target, SIGTERM);
		  kill(target, SIGTERM);
		  fflush(stdout);
		  exit(0);
          }
          else {
			writeSendSignalEvent(target, SIGCONT);  
            kill(target, SIGCONT);
			fflush(stdout);
          }

		  signal(sig, SIG_DFL);

}

bool readFlags(char **argv, int argc, struct FLAGS* flags){
	char *link1 = "-l\0";
	char *link2 = "--count-links\0";
	char *all1 = "-a\0";
	char *all2 = "--all\0";
	char *bytes1 = "-b\0";
	char *bytes2 = "--bytes\0";
	char *block1 = "-B\0";
	char *block2 = "--block-size=";
	char *dereference1 = "-L\0";
	char *dereference2 = "--dereference\0";
	char *separate1 = "-S\0";
	char *separate2 = "--separate-dirs\0";
	char *max_depth = "--max-depth=";

	flags->all = flags->bytes = flags->dereference = flags->link = flags->separate=false;	//default flag state
	flags->blockSize = 1024;		//default block size
	flags->maxDepth = 2147483647;	//maximum value for an int (just a big value so it will never be reached for the default value)
	flags->dir = "";

	for(char **arg=argv+1;*arg!=0;arg++){
		char *thisarg=*arg;

		//will check each flag and if it has been already used
		if((strncmp(all1,thisarg,3)==0||strncmp(all2,thisarg,6)==0)&&flags->all==false){
			flags->all=true;
			continue;
		}
		else if((strncmp(link1,thisarg,3)==0 || strncmp(link2,thisarg,14)==0)&&flags->link==false){
			flags->link=true;
			continue;
		}
		else if((strncmp(bytes1,thisarg,3)==0||strncmp(bytes2,thisarg,8)==0)&&flags->bytes==false){
			flags->bytes=true;
			continue;
		}
		else if((strncmp(block1,thisarg,3)==0||strncmp(block2,thisarg,13)==0)&&flags->blockSize==1024){
			if(strncmp(block1,thisarg,3)==0){
				arg++;
				flags->blockSize=atoi(*arg);
			}
			else{
				thisarg+=13;
				flags->blockSize=atoi(thisarg);
			}
			continue;
		}
		else if((strncmp(dereference1,thisarg,3)==0||strncmp(dereference2,thisarg,14)==0)&&flags->dereference==false){
			flags->dereference=true;
			continue;
		}
		else if((strncmp(separate1,thisarg,3)==0||strncmp(separate2,thisarg,16)==0)&&flags->separate==false){
			flags->separate=true;
			continue;
		}
		else if((strncmp(max_depth,thisarg,12)==0)&&flags->maxDepth==2147483647){
			thisarg+=12;
			flags->maxDepth=atoi(thisarg);
			continue;
		}
		else if(strlen(flags->dir)==0){
			flags->dir=thisarg;
			continue;
		}
		return true;	//returns true if any input is wrong
	}

	if(flags->link==false) return true; //links is a must

	return false; //read flags with no errors
}

long int du(struct FLAGS* flags,char* path,int depth){
	struct dirent* newFile;
	struct stat statBuffer;
	long int dirSize=0;

    DIR* source_dir = opendir(flags->dir);

    if (source_dir == NULL) return -1;

    chdir(flags->dir);

    while ((newFile = readdir(source_dir)) != NULL){

        if(flags->dereference ){
        	stat(newFile->d_name, &statBuffer);
        }
        else {
        	lstat(newFile->d_name, &statBuffer);
        }

    	char fullPath[ 4096 ]="";	//max path length for linux (not sure, I just googled)
    	strcat(fullPath,path);
    	strcat(fullPath,"/");
    	strcat(fullPath,newFile->d_name);


        if(S_ISREG(statBuffer.st_mode)){ 

			long int fileSize=0;

			getSizeFlagged(&fileSize,flags,statBuffer);

			if(flags->all && depth<flags->maxDepth){
				printItem(fullPath,fileSize);
			}
			dirSize+=fileSize;

        }

		else if(S_ISLNK(statBuffer.st_mode)&&(flags->dereference==false)){ 

			long int fileSize=0;

			getSizeFlagged(&fileSize,flags,statBuffer);

			if(flags->all && depth<flags->maxDepth){
				printItem(fullPath,fileSize);
			}
			dirSize+=fileSize;

        }
		
		else if(S_ISDIR(statBuffer.st_mode)){
			
			if(strcmp(newFile->d_name,".")==0||strcmp(newFile->d_name,"..")==0){
				continue;
			}

			int pid, pd[2];

			flags->dir=newFile->d_name;

			pipe(pd);

			pid=fork();
			if(pid==0){
				long int subDirSize=0;

				subDirSize += du(flags,fullPath,depth+1);

				close(pd[0]);//close reading end
				writeSendPipeEvent(subDirSize);
				write(pd[1],&subDirSize,sizeof(subDirSize));	//writes sub directory size to pipe so father can read
				close(pd[1]);//close writing end

				writeExitEvent(0);
				exit(0);
			}
			else if(pid>0){
				waitpid(pid,NULL,0);

				long int subDirSize=0;

				close(pd[1]);//close writing end
				writeRecvPipeEvent(subDirSize);
				read(pd[0],&subDirSize,sizeof(subDirSize));	//read full sub directory size from child process
				close(pd[0]);//close reading end

				getSizeFlagged(&subDirSize,flags,statBuffer);

				if(depth<flags->maxDepth){
					printItem(fullPath,subDirSize);
				}

				if(!flags->separate){	//add sub directory size to main directory if separate flag not active
					dirSize+=subDirSize;
				}
			}
			else{
				perror("Error forking");
				return -1;
			}	
		}
    }
    if(depth==0){	//print main directory size
    	getSizeFlagged(&dirSize,flags,statBuffer);
    	printItem(path,dirSize);
    }
    chdir("..");	//since we changed directory in the beggining we must go back
    closedir(source_dir);
    return dirSize;
}

void printItem(char* path, long int size){
	printf("%-ld\t%s\n",size,path);
	writeEntryEvent(size, path);
	fflush(stdout);
}

void getSizeFlagged(long int *size,struct FLAGS* flags,struct stat statBuffer){
	if(flags->bytes){
		(*size)+=statBuffer.st_size;
	}
	else{
		(*size)+=(statBuffer.st_blocks*512)/flags->blockSize;
	}
}