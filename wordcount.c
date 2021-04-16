/*
Erik Morua, cssc3333, Red ID: 816670615
Nathan Azoulay, cssc3302 Red ID: 821871239
Program 2 Poor Person's Progress Bar: Wordcount.c
IN: A .txt file through the command line.
Work: .txt file will be read. Words will be counted, while in a seperate thread, a function, progress_monitor, will track the progress of wordcount.
OUT: A live progress bar will be printed until wordcount is finished processing. The printed progress bar will be printed along with the number of words in the file.

Pair Programming Equitable Participation & Honesty Affidavit
We the undersigned promise that we have in good faith attempted to follow the principles of pair programming. Although we were free to discuss ides with others, 
the implementation is our own. We have shared a common workspace and taken turns at the keyboard for the majority of the work that 
we are submitting. Furthermore, any non programming portions of the assignment were done independently. We recognize that should this not be the case,
we will be subject to penalties as outlined in the course syllabus.

Pair Programmer 1  Nathan Azoulay, Red ID: 821871239, 2/23/2021
Pair Programmer 2  Erik Morua, Red ID: 816670615, 2/23/2021
*/
#include "wordcount.h"


//Declaring struct to use in wordcount
typedef struct {
	long* CurrentStatus;
	long InitialValue;
	long TerminationValue;
} PROGRESS_STATUS;

int main(int argc, char** argv) {

	long words;

	if (argc == 2) {
		if (argv[1] != NULL) {
			wordcount(argv[1]);
		}
	}
	else if (argc > 2) {
		printf("Could not open file \n");
		exit(1);
	}
	else {
		printf("Could not find file specified.\n");
		exit(1);
	}
}

//progress_monitor accepts a void* struct that holds information filled in wordcount().
//Display: progress bar that is shows using '-' and '+' every 10 updates. 
//Every mark represents 2%. Loops in coordination with the progress of wordcount() which will read the words in a file.
void* progress_monitor(void* data) {
	PROGRESS_STATUS* prog = (PROGRESS_STATUS*)data;
	//printf("Testing... %ld \n",prog->TerminationValue);
	*prog->CurrentStatus = 0;
	long int curr_progress;
	long perc;
	//curr_progress = (long)((int)((*(prog->CurrentStatus)) * 100 / (int)prog->TerminationValue));
	int count = 0;

	while (((*prog->CurrentStatus) < prog->TerminationValue)) //loops until wordcount finishes reading the file.
	{	


		// calculates the current percentage
		curr_progress = (long)((float)((*(prog->CurrentStatus)) * 100 / (float)prog->TerminationValue));

		if ((curr_progress == count + 1) && count != 50) {
			if (curr_progress % 10 == 0) { //every 10 marks, print a +
				printf("+");
				fflush(stdout);
				count++;
			}
			else { // else we print a -
				printf("-");
				fflush(stdout);
				count++;
			}
		}
	}
	
	printf("\n");
	fflush(stdout);
	pthread_exit(0);
}

//Wordcount takes in a file to read. It will read and keep track of the number of words in the file passed in.
//Main thread here. Starts a second thread to run progress_monitor while the file is being read. Initializes a struct and alocates memory. 
//Updates the struct as it runs. When it finishes, the thread is cut off and the number of words counted is returned.
long int wordcount(const char* fname) {
	FILE* path;
	long currBytes = 0;
	long int total = 0;
	long int numWords = 0;
	int x;
	int check;

	//opening file input from terminal
	//printf("%s\n",fname);
	path = fopen(fname, "r");
	if (path == NULL) {
		printf("Could not open file %s", fname);
		return 0;
	}

	// get number of bytes in file
	fseek(path, 0L, SEEK_END);
	total = ftell(path);
	rewind(path);

	//allocating and initializing our structure.
	PROGRESS_STATUS* progress = malloc(sizeof(PROGRESS_STATUS));
	progress->CurrentStatus = 0;
	progress->InitialValue = 0;
	progress->TerminationValue = (long)total;

	//Creating Thread that will run progress monitor.
	pthread_t pthread;
	pthread_create(&pthread, NULL, progress_monitor, (void*)&progress);

	//looping until the end of the file is reached.
	while ((x = fgetc(path)) != EOF) {
		currBytes = ftell(path);
		progress->CurrentStatus = (long)currBytes; //updates the current location at each loop
		//printf("Testing... %ld \n", progress.CurrentStatus);
		if (isspace(x)) { //if there is a space, a word has completed, count the word. 
			while (isspace(x)) //checking for double blank spaces so the function doesn't count a blank as a word.
				x = fgetc(path);
				currBytes = ftell(path);
				progress->CurrentStatus = (long)currBytes; //update current status
			//printf("Testing... %ld \n", progress.CurrentStatus);
			numWords++; //keeping track of words
		}
		if(fname == EOF){
			system("pause");
		}
	}
	fclose(path); // done with file, close.
	pthread_join(pthread, NULL); //pauses the thread until wordcount is done counting.
	printf("There are %ld words in %s \n", numWords, fname);
	fflush(stdout);
	//printf("There are %ld bytes in %s \n", currBytes, fname);
	free(progress);//release memory allocated for progress.
	return numWords;
}