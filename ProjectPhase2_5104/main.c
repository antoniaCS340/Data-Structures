/
*****************************************************
 * @file   main.c                                    *
 * @author Paterakis Giorgos <geopat@csd.uoc.gr>     *
 *                                                   *
 * @brief Main Function for Data Structures (CS240b) *
 * Project: Winter 2023						         *
 *****************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <time.h>

#include "Movie.h"

#define BUFFER_SIZE 1024  /**< Maximum length of a line in input file */

/* Uncomment the following line to enable debugging prints 
 * or comment to disable it */
#define DEBUG

#ifdef DEBUG
#define DPRINT(...) fprintf(stderr, __VA_ARGS__);
#else  /* DEBUG */
#define DPRINT(...)
#endif /* DEBUG */

int hashtable_size; 	/** The size of the users hashtable (>0) */
int max_users;         /** The maximum number of registrations (users) */
int max_id;            /** The maximum user ID */

// This is a very conservative progress on the hashtable. Our purpose
// is to force many rehashes to check the stability of the code.
int primes_g[170] = 		{  5,   7,  11,  13,  17,  19,  23,  29,  31,  37,
                               41,  43,  47,  53,  59,  61,  67,  71,  73,  79,
                               83,  89,  97, 101, 103, 107, 109, 113, 127, 131,
                              137, 139, 149, 151, 157, 163, 167, 173, 179, 181,
                              191, 193, 197, 199, 211, 223, 227, 229, 233, 239,
                              241, 251, 257, 263, 269, 271, 277, 281, 283, 293,
                              307, 311, 313, 317, 331, 337, 347, 349, 353, 359,
                              367, 373, 379, 383, 389, 397, 401, 409, 419, 421,
                              431, 433, 439, 443, 449, 457, 461, 463, 467, 479,
                              487, 491, 499, 503, 509, 521, 523, 541, 547, 557,
                              563, 569, 571, 577, 587, 593, 599, 601, 607, 613,
                              617, 619, 631, 641, 643, 647, 653, 659, 661, 673,
                              677, 683, 691, 701, 709, 719, 727, 733, 739, 743,
                              751, 757, 761, 769, 773, 787, 797, 809, 811, 821,
                              823, 827, 829, 839, 853, 857, 859, 863, 877, 881,
                              883, 887, 907, 911, 919, 929, 937, 941, 947, 953,
                          	  967, 971, 977, 983, 991, 997, 1009, 1013, 1019, 1021 };

user_t **user_hashtable_p;	/* The users hashtable. This is an array of chains (pinakas katakermatismoy xrhstwn)*/
new_movie_t *new_releases;     /* New releases simply-linked binary tree*/
movieCategory_t *categoryArray[6];  /* The categories array (pinakas kathgoriwn)*/
movie_t *category_Array[6];
const char *categoryNames[] = {
    "Horror",
    "Sci-Fi",
    "Drama",
    "Romance",
    "Documentary",
    "Comedy"
};

void init_global(){

	new_movie_t *new_releases = NULL;
	for(int i = 0; i < 6; i++)
		categoryArray[i] = NULL;


}


int a, b, prime;

int main(int argc, char** argv)
{
	
	FILE *fin = NULL;
	char buff[BUFFER_SIZE], event;

	/* Check command buff arguments */
	if ( argc != 2 ) {
		fprintf(stderr, "Usage: %s <input_file> \n", argv[0]);
		return EXIT_FAILURE;
	}

	/* Open input file */
	if (( fin = fopen(argv[1], "r") ) == NULL ) {
		fprintf(stderr, "\n Could not open file: %s\n", argv[1]);
		perror("Opening test file\n");
		return EXIT_FAILURE;
	}

	/* Read input file buff-by-buff and handle the events */
	while ( fgets(buff, BUFFER_SIZE, fin) ) {

		DPRINT("Event: %s \n", buff);

		switch(buff[0]) {
		
		init_global();
		
		/* Comment */
		case '#':
			break;
		/* max_users */
		case '0': {
			sscanf(buff, "%c %u", &event, &max_users);
			DPRINT("max users: %u\n", max_users);
			
			printf("max users:%d\n",max_users);
			int i = 0;
			while(max_users >= primes_g[i]){
				i++;
			}
			hashtable_size = primes_g[i];
			printf("size of array user:%d\n",hashtable_size);

			user_hashtable_p = (user_t **)malloc(hashtable_size * sizeof(user_t *));

			if(user_hashtable_p == NULL){
				printf("Dynamic memory allocation failed\n");
				return -1;
			}

			for (int i = 0; i < hashtable_size; ++i) {
				user_hashtable_p[i] = NULL;
			}
			
			break;
		}
		/* max_id */
		case '1': {
			sscanf(buff, "%c %u", &event, &max_id);
			DPRINT("max id: %u\n", max_id);
			
			int i = 0;
			while(max_id < primes_g[i]){
				i++;
			}
			prime = primes_g[i];
			srand(time(NULL));
			a = (rand() % prime) + 1; //tuxaios arithmos [1,p-1]
			//srand(time(NULL));
			b = rand() % prime; //tuxaios arithmos [0,p-1]
			
			break;
		}		
		/* Event R : R <userID> - Register user. */
		case 'R':
		{
			int userID;
			sscanf(buff, "%c %d", &event, &userID);
			DPRINT("%c %d\n", event, userID);
			if ( register_user(userID) ) {
				DPRINT("%c succeeded\n\n", event);
			} else {
				fprintf(stderr, "%c failed\n", event);
			}

			break;
		}
		/* Event U : U <userID> - Unregister user. */
		case 'U':
		{
			int userID;
			sscanf(buff, "%c %d", &event, &userID);
			DPRINT("%c %d\n", event, userID);

			if ( unregister_user(userID) ) {
				DPRINT("%c %d succeeded\n\n", event, userID);
			} else {
				fprintf(stderr, "%c %d failed\n", event, userID);
			}

			break;
		}
		/* Event A : A <movieID> <category> <year> - Add new movie. */
		case 'A':
		{
			int movieID,category, year;
			sscanf(buff, "%c %d %d %d", &event, &movieID, &category, &year);
			DPRINT("%c %d %d %d\n", event, movieID, category, year);

			if ( add_new_movie(movieID, category, year) ) {
				DPRINT("%c %d %d %d succeeded\n\n", event, movieID, category, year);
			} else {
				fprintf(stderr, "%c %d %d %d failed\n", event, movieID, category, year);
			}

			break;
		}
		/* Event D : D  - Distribute movies. */
		case 'D':
		{
			sscanf(buff, "%c", &event);
			DPRINT("%c\n", event);

			if ( distribute_movies() ) {
				DPRINT("%c succeeded\n\n", event);
			} else {
				fprintf(stderr, "%c failed\n", event);
			}

			break;
		}
		/* Event W : W <userID ><category><movieID><score> - Watch movie */
		case 'W':
		{
			int userID, movieID,category,score;

			sscanf(buff, "%c %d %d %d %d", &event,&userID,&category, &movieID, &score);
			DPRINT("%c %d %d %d %d\n", event,userID, category, movieID, score);

			if ( watch_movie(userID,category, movieID, score) ) {
				DPRINT("%c %d %d %d %d succeeded\n\n", event,userID, category, movieID, score);
			} else {
				fprintf(stderr, "%c %d %d %d %d failed\n", event, userID,category, movieID, score);
			}

			break;
		}
		/* Event –	F  <userID ><category1><category2><score> Filter movies */
		case 'F':
		{
			int userID, score;
			sscanf(buff, "%c %d %d\n", &event, &userID,&score);
			DPRINT("%c %d %d\n", event, userID,score);

			if (filter_movies(userID,score) ) {
				DPRINT("%c %d %d succeeded\n\n", event, userID,score);
			} else {
				fprintf(stderr, "%c %d %d failed\n", event, userID,score);
			}

			break;
		}
		/* Event Q : Q <userID> - User statistics */
		case 'Q':
		{
			int userID;
			sscanf(buff, "%c %d\n", &event, &userID);
			DPRINT("%c %d\n", event, userID);

			if ( user_stats(userID) ) {
				DPRINT("%c %d succeeded\n\n", event, userID);
			} else {
				fprintf(stderr, "%c %d failed\n", event, userID);
			}

			break;
		}
		/* Event I : I <movieID> <category> - Search movie */
		case 'I':
		{
			int movieID,category;
			sscanf(buff, "%c %d %d\n", &event, &movieID, &category);
			DPRINT("%c %d %d\n", event, movieID, category);

			if ( search_movie(movieID, category) ) {
				DPRINT("%c %d %d succeeded\n\n", event, movieID, category);
			} else {
				fprintf(stderr, "%c %d %d failed\n", event, movieID, category);
			}

			break;
		}
		/* Event M : M  - Print movies */
		case 'M':
		{
			sscanf(buff, "%c", &event);
			DPRINT("%c\n", event);

			if ( print_movies() ) {
				DPRINT("%c succeeded\n\n", event);
			} else {
				fprintf(stderr, "%c failed\n", event);
			}

			break;
		}
		/* Event P : P  - Print users */
		case 'P':
		{
			sscanf(buff, "%c", &event);
			DPRINT("%c\n", event);

			if ( print_users() ) {
				DPRINT("%c succeeded\n\n", event);
			} else {
				fprintf(stderr, "%c failed\n", event);
			}

			break;
		}
		/* Empty line */
		case '\n':
			break;

		/* Ignore everything else */
		default:
			DPRINT("Ignoring buff: %s \n", buff);
			break;
		}
	}

	for (int i = 0; i < hashtable_size; ++i) {
        user_t* currentUser = user_hashtable_p[i];
        while (currentUser != NULL) {
            user_t* temp = currentUser;
            currentUser = currentUser->next;
            free(temp);
        }
    }
    free(user_hashtable_p);

	return (EXIT_SUCCESS);
}

void print_chain(int index){

	printf("Chain %d of Users:\n",index);
	user_t *curr = user_hashtable_p[index];
	while(curr != NULL){
		printf("   %d\n",curr->userID);
		curr = curr->next;
	}
	printf("DONE ");
}

unsigned int hash_fuction(int K){

	return ((a*K+b) % prime) % hashtable_size;
}

int register_user(int userID){

	//dhmiourgia neou komvou xrhsth
	user_t *newUser = (user_t*)malloc(sizeof(user_t));
	if(newUser == NULL){
		printf("Dynamic memory allocation failed\n");
		return 0;
	}

	newUser->userID = userID;
	newUser->history = NULL;
	newUser->next = NULL;

	//evresh katallhlhs alusidas gia thn eisagwgh sto pinaka katakermatismou
	int index = hash_fuction(userID);

	if(user_hashtable_p[index] == NULL){
		user_hashtable_p[index] = newUser;
	} else {
		user_t *lastUser = user_hashtable_p[index];
		while(lastUser->next != NULL){
			lastUser = lastUser->next;
		}
		lastUser->next = newUser;
	}

	print_chain(index);
	return 1;
}

void delete_tree_history(userMovie_t *p){
	
	if (p == NULL){
		return;
	}
	delete_tree_history(p->lc);
	delete_tree_history(p->rc);

	free(p);
}
int unregister_user(int userID){

	int index = hash_fuction(userID);
	user_t *curr = user_hashtable_p[index];
	user_t *prev = NULL;

	while (curr->next != NULL && curr->userID != userID){
		prev = curr;
		curr = curr->next;
	}

	if (curr == NULL) {
		printf("User doesn't exist!\n");
        return 0;  // failure
	}

	delete_tree_history(curr->history);

	// afairesi komvou apo ti lista
    if (prev != NULL) {
        prev->next = curr->next;
    } else {
        user_hashtable_p[index] = curr->next;
    }

	free(curr);
	print_chain(index);
	return 1;
}

void printNewMovies(new_movie_t *p){

	if(p == NULL)
		return;
	printNewMovies(p->lc);
	printf("%d, ",p->movieID);
	printNewMovies(p->rc);

}

int add_new_movie(int movieID, int category, int year){

	new_movie_t *P = new_releases;
	new_movie_t *prev = NULL;


	//anazitish an uparxei tainia me to idio key, alliws thn katallili thesi wste na paramenei binary search tree
	while(P != NULL){
		if(P->movieID == movieID){
			printf("There is already that movie!\n");
			return 0;
		}
		prev = P;
		if(movieID < P->movieID){
			P = P->lc;
		} else {
			P = P->rc;
		}
	}

	// dhmiourgia neou komvou
	new_movie_t *newMovie = (new_movie_t *)malloc(sizeof(new_movie_t));
	if( newMovie == NULL){
		printf("Memory allocation failed\n");
		return 0;
	}
	newMovie->movieID = movieID;
	newMovie->category = category;
	newMovie->year = year;
	newMovie->sumScore = newMovie->watchedCounter = 0;
	newMovie->lc = newMovie->rc = NULL;

	//sindesi tou komvou sth swsth thesi
	if (prev == NULL){
		new_releases = newMovie;
	} else if (movieID < prev->movieID){
		prev->lc = newMovie;
	} else {
		prev->rc = newMovie;
	}

	//ektypwsh event
	printf("New releases Tree:\n\tnew releases:");
	printNewMovies(new_releases);
	printf("\nDONE ");
	return 1;
}

int counter;
int countPerCat(new_movie_t *p, int category){// metraei tis tainies kathe kathgorias

	if(p == NULL)
		return counter;
	countPerCat(p->lc, category);
	countPerCat(p->rc, category);

	if (p->category == category)
		counter++;

	return counter;
}

int num ; // counter gia kathe eidos kathgoriwn
int array[100][2];
int ind_ex;
void tree_to_array(new_movie_t *p, int cat){
	if (p == NULL)
		return;
	tree_to_array(p->lc, cat);
	if(p->category == cat){
		array[ind_ex][1] = p->movieID;
		array[ind_ex][2] = p->year;
		ind_ex++;
	}
	tree_to_array(p->rc, cat);
	if(ind_ex == num)
		exit; //otan vrei oles tis tainies ths idias kathgorias

}

movie_t *BSTinsert(int start, int end){
	if (start > end) 
        return NULL;


    int mid = (start + end) / 2;
	movie_t *newMovie = (movie_t *)malloc(sizeof(movie_t));
	if( newMovie == NULL){
		printf("Memory allocation failed\n");
		return NULL;
	}
	newMovie->movieID = array[mid][1];
	newMovie->year = array[mid][2];
	newMovie->sumScore = newMovie->watchedCounter = 0;
	newMovie->lc = newMovie->rc = NULL;

    newMovie->lc = BSTinsert(start, mid - 1);
    newMovie->rc = BSTinsert(mid + 1, end);

    return newMovie;
}

void deleteTree(new_movie_t *R){

	if (R == NULL){
		return;
	}
	deleteTree(R->lc);
	deleteTree(R->rc);

	free(R);
}

int distribute_movies(){

	for(int i = 0; i < 6; i++){
		counter = 0;
		num = countPerCat(new_releases, i); //global deixnei tis tainies pou yparxoun se kathe kathgoria
		ind_ex = 0;
		tree_to_array(new_releases, i); //antigrafi twn komvwn ths i kathgorias se voithitiko pinaka
		category_Array[i] = BSTinsert(0, num-1); //eisagvgh tvn komvwn tou pinaka se taxinomhmeno dendro

	}
	print_movies();

	//apeleftherwsi mnhmhs
	deleteTree(new_releases);
	free(new_releases);
	return 1;
	
	//distribute_movies_bonus();
}

void printW(userMovie_t *p){
	if(p == NULL)
		return;
	printW(p->lc);
	if(p->lc == NULL && p->rc == NULL)
		printf("\t%d,%d\n",p->movieID, p->score);
	printW(p->rc);
}
int watch_movie(int userID, int category, int movieID, int score) {

	//anazhthsh tainias me uid sthn dotheisa kathgoria
	movie_t *curr = category_Array[category];
	while (curr != NULL && movieID != curr->movieID){
		if (movieID < curr->movieID){
			curr = curr->lc;
		} else {
			curr = curr->rc;
		}
	}
	if(curr != NULL){
		curr->watchedCounter++;
		curr->sumScore += score;
	} else {
		printf("This movie doesn't exist!\n");
		return 0;
	}
	
	//anazhthsh xrhsth apo ton pinaka katakermatismou
	int index = hash_fuction(userID);
	user_t *currUser = user_hashtable_p[index];

	while (currUser->next != NULL && currUser->userID != userID){
		currUser = currUser->next;
	}

	if (currUser == NULL) {
		printf("User doesn't exist!\n");
		return 0;  // failure
	}
	
	//anazhthsh gia thn katallhlh thesi sto dentro istorikou tou xrhsth gia thn nea tainia
	userMovie_t *userHistory = currUser->history ;
	userMovie_t *prev = NULL;

	while(userHistory != NULL){
		if(userHistory->movieID == movieID){
			printf("There is already that movie!\n");
			return 0;
		}
		prev = userHistory;
		if(movieID < userHistory->movieID){
			userHistory = userHistory->lc;
		} else {
			userHistory = userHistory->rc;
		}
	}
	
	//dimiourgia komvou ths neas tainias
	userMovie_t *watchedMovie = (userMovie_t *)malloc(sizeof(userMovie_t));
	if( watchedMovie == NULL){
		printf("Memory allocation failed\n");
		return 0;
	}

	watchedMovie->category = category;
	watchedMovie->movieID = movieID;
	watchedMovie->score = score;
	watchedMovie->lc = watchedMovie->rc = watchedMovie->parent = NULL;

	//dimiourgia voithitikou komvou 
	userMovie_t *p = (userMovie_t *)malloc(sizeof(userMovie_t));
	if(p == NULL){
		printf("Memory allocation failed\n");
		return 0;
	}

	
	if (prev == NULL){
		//an einai o prwtos pou eisagete einai riza
		currUser->history = watchedMovie;
		free(p);
	}else {
		p->category = prev->category;
		p->movieID = prev->movieID;
		p->score = prev->score;
		p->parent = prev;
		p->lc = p->rc = NULL;

		if(movieID < prev->movieID){
			//an to key einai mikrotero tou gonikou, tote prev kai dexi upodendro exoun to key prev
			prev->rc = p;
			prev->lc = watchedMovie;
			watchedMovie->parent = prev;
			
		} else {
			//alliws exoun to key ths neas tainias
			prev->rc = watchedMovie;
			prev->lc = p;
			watchedMovie->parent = prev;

			prev->category = category;
			prev->movieID = movieID;
			prev->score = score;
		}
	}
	printW(currUser->history);
	printf("\nDONE ");

	return 1;
}

int numMovies = 0;
void countMovies(movie_t *p, int score){

	if(p == NULL)
		return;
	countMovies(p->lc, score);
	
	if((float)(p->sumScore/p->watchedCounter) > score){
		numMovies++;
	}
	countMovies(p->rc, score);
}

int numArray = 0;
void insertToArray(movie_t **Farray, movie_t *R, int score){

	if (R == NULL)
		return;
	insertToArray(Farray, R->lc, score);
	if ((float)(R->sumScore/R->watchedCounter) > score){
		Farray[numArray] = R;
		numArray++;
	}
	insertToArray(Farray, R->rc, score);
}

void Heapify(movie_t **A, int i){

	int m = numMovies-1;
	int lc = 2*m-numMovies;
	int	rc = lc-1;
	int p;

	while(( lc >= i && (float)A[lc]->sumScore/A[lc]->watchedCounter < (float)A[m]->sumScore/A[m]->watchedCounter) || (rc >= i && (float)A[rc]->sumScore/(float)A[rc]->watchedCounter < A[m]->sumScore/A[m]->watchedCounter)){

		lc = 2*m-numMovies;
		rc = lc-1;
		if (rc >= i){
			if ((float)A[lc]->sumScore/A[lc]->watchedCounter < (float) A[rc]->sumScore/A[rc]->watchedCounter){
				p = lc;
			} else {
				p = rc;
			}
		} else {
			p = i;
		}

		//swap
		movie_t *temp = A[m];
		A[m] = A[p];
		A[p] = temp;
		m = p;

	}

}

int filter_movies(int userID, int score){

	
	for(int i = 0; i < 6; i++){
		countMovies(category_Array[i],score);
	}

	movie_t **filterArray = (movie_t **)malloc(numMovies * sizeof(movie_t*));

	for(int i = 0; i < 6; i++){
		insertToArray(filterArray, category_Array[i], score);
	}

	for(int i = 1; i < numMovies; i++){
		Heapify(filterArray, i);
	}

	movie_t *temp;
	for(int i = 0; i < numMovies-1; i++){
		//swap 
		temp = filterArray[i];
		filterArray[i] = filterArray[numMovies-1];
		filterArray[numMovies-1] = temp;

		Heapify(filterArray, i);
	}

	float avg;
	for(int i = 0; i < numMovies; i++){
		avg = (float)filterArray[i]->sumScore/filterArray[i]->watchedCounter;
		printf("\t%d, %0.2f  ",filterArray[i]->movieID, avg);
	}
	printf("\nDONE ");

	for(int i = 0; i < numMovies; i++){
		free(filterArray[i]);
	}
	free(filterArray);

	return 1;
}

userMovie_t *FindNextLeaf(userMovie_t *leaf){
	userMovie_t *parent1 = leaf->parent;
	userMovie_t *next;//pithano epomeno fyllo 

	if(parent1->lc == leaf && parent1->rc != NULL){//an to prohgoumeno fyllo einai aristero paidi tou patera tou kai exei dexi paidi
		next = parent1->rc; 
	} else {// alliws kateuthinome anapoda sto dendro mexri na synanthsw ton prwto komvo pou xtipisa apo dexia
		userMovie_t *Nparent = parent1->parent;

		while (parent1 == Nparent->rc){
			parent1 = Nparent;
			Nparent = parent1->parent;
		}

		next = Nparent->rc;
		 
	}

	while(1){
		if(next->lc == NULL && next->rc == NULL){ // an o next den exei paidia einai fyllo
			return next;
		} else if (next->lc != NULL){//an exei aristero paidi phgainv sto aristerotero paidi tou kai to epistrefw

			while(next->lc != NULL){
				next = next->lc;
			}
			return next;
		}else {//alliws phgainw sto dexi paidi tou kai epanalamvanw
			next = next->rc;
		}
	}
}
int user_stats(int userID){

	int index = hash_fuction(userID);
	user_t *currUser = user_hashtable_p[index];

	while (currUser->next != NULL && currUser->userID != userID){
		currUser = currUser->next;
	}

	if (currUser == NULL) {
		printf("User doesn't exist!\n");
		return 0;  // failure
	}

	
	if (currUser->history != NULL){
		int count = 0;
		int ScoreSum = 0;
		float avg;
		userMovie_t *nextLeaf = currUser->history; 
		userMovie_t *lastLeaf = currUser->history;

		while (nextLeaf->lc != NULL){
			nextLeaf = nextLeaf->lc;
		}
		while (lastLeaf->rc != NULL){
			lastLeaf = lastLeaf->rc;
		}

		ScoreSum += nextLeaf->score;
		count++;
		while(nextLeaf != lastLeaf ){
			nextLeaf = FindNextLeaf(nextLeaf);
			ScoreSum += nextLeaf->score;
			count++;
		}
		avg = (float)ScoreSum/count;

		printf("Q  %d %0.2f\nDONE\n", currUser->userID, avg);
	} else {
		printf("User hasn't watched any movie yet!\n");
	}
	
	
	return 1;
}

int search_movie(int movieID, int category){ 

	movie_t *R = category_Array[category];
	while(R != NULL && movieID != R->movieID){
		if(movieID < R->movieID){
			R = R->lc;
		} else {
			R = R->rc;
		}
	}

	if (R != NULL){
		printf("I %d %s %d\n", movieID, categoryNames[category], R->year);
		printf("DONE ");
		return 1;
	} else {
		printf("There isn't that movie!\n");
		return 0;
	}
	
}
void printMoviesPerCat(movie_t *p){
	if(p == NULL)
		return;
	printMoviesPerCat(p->lc);
	printf("%d ",p->movieID);
	printMoviesPerCat(p->rc);

}
int  print_movies(){

	printf("Movie Category Array:\n");
	for (int i = 0; i < 6; i++) {
        printf("\t%s: ", categoryNames[i]);
		printMoviesPerCat(category_Array[i]);
		printf("\n");
	}
	printf("DONE ");
	return 1;
}

int  print_users(){

	for(int i = 0; i < hashtable_size; i++){
		
		user_t *currUser = user_hashtable_p[i];
		if (currUser != NULL){
			printf("Chain %d of Users:\n", i);
		}
		while (currUser != NULL){
			
			printf("\t%d\n", currUser->userID);
			printf("\t History Tree:\n");

			if(currUser->history != NULL){
				printW(currUser->history);
				printf("\n");
			} else {
				printf("User didn't watch any movie\n");
			}

			currUser = currUser->next;
		}

	}

	printf("\nDONE ");

	return 1;
}

/*bonus dhmiourgia AVL dendrwn event D

movie_t *LeftRotate(movie_t *y){
	movie_t *x = y->rc;
	movie_t *p = x->lc;

	x->lc = y;
	y->rc = p;

	return x;
}

movie_t *RightRotate(movie_t *x){
	movie_t *y = x->lc;
	movie_t *p = y->rc;

	y->rc = x;
	x->lc = p;

	return y;
}

int Height(movie_t *p){

	int lh, rh;
	if (p == NULL){
		return 0;
	}

	lh = Height(p->lc);
	rh = Height(p->rc);

	return (lh > rh) ? lh + 1 : rh + 1;


}

int nodeBalance(movie_t *p){
	if (p == NULL){
		return 0;
	}

	return Height(p->lc)-Height(p->rc);
}

movie_t *AVLinsert(movie_t *R, movie_t *p){

	if (R == NULL){
		return p;
	}

	if (p->movieID < R->movieID){
		R->lc = AVLinsert(R->lc, p);
	} else if (p->movieID > R->movieID){
		R->rc = AVLinsert(R->rc, p);
	} else {
		return R;
	}

	int balance = nodeBalance(R);

	if (balance > 1 && p->movieID < R->lc->movieID) {
        return RightRotate(R);
    }
    if (balance < -1 && p->movieID > R->rc->movieID) {
        return LeftRotate(R);
    }
    if (balance > 1 && p->movieID > R->lc->movieID) {
        R->lc = LeftRotate(R->lc);
        return RightRotate(R);
    }
    if (balance < -1 && p->movieID < R->rc->movieID) {
        R->rc = RightRotate(R->rc);
        return LeftRotate(R);
    }

    return R;
}

void postOrder(new_movie_t *R){

	if (R == NULL){
		return;
	}
	postOrder(R->lc);
	postOrder(R->rc);
	movie_t *newNode = (movie_t *)malloc(sizeof(movie_t));
	newNode->movieID = R->movieID;
	newNode->year = R->year;
	newNode->sumScore = newNode->watchedCounter = 0;
	newNode->lc = newNode->rc = NULL;
	AVLinsert(category_Array[R->category], newNode);

	free(R);
	
}

int distribute_movies_bonus(){

	//matadiatetagmenh gia prospelasi tou dendrou twn newn kykloforiwn
	postOrder(new_releases);
	
	print_movies();
	return 1;
}*/