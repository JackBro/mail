#include <stdio.h>
#include <curl/curl.h>

#include <string>

//#include "loader.h"
#include "getter.h"

#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
/*struct thr_cntxt
{
	Loader *loader;
};

void *loader_fun(void *arg)
{
	thr_cntxt *cntxt = (thr_cntxt *)arg;
	cntxt->loader->load("http://habrahabr.ru/post/155201/");
	printf("~~~~~~~~~~~~~~ end ~~~~~~~~~~~~~~~~~~~\n");
	return (void *)0;
}

void *reader_fun(void *arg)
{
	int fd;
	char buf[255];
	fd = open("mypipe", O_RDONLY);
	if (fd == -1) {
		fprintf(stderr, "Could not open pipe\n");
		return (void *)1;
	}

	ssize_t bytes;
	do {
		printf(">>>> pre read\n");
		bytes = read(fd, &buf, sizeof(buf)-1);
		printf(">>> get %d bytes\n", bytes);
		if (bytes == -1) {
			fprintf(stderr, "Could not read from pipe\n");
			close(fd);
			return (void *)1;
		}
		buf[bytes] = 0;
		printf((const char *)buf);

	} while (bytes);

	return (void*)0;
}*/

int main()
{
	CURL *curl;

	curl = curl_easy_init();
	if (curl) {
		printf("Hello curl!\n");
		curl_easy_cleanup(curl);
	}
	printf("Hello!\n");

/////////////////////////////////////////////////

	Getter getter("mypipe", 3); 
	printf("^^^^^^^^^^^^^ GET ^^^^^^^^^^^^^^^^^^\n");
	getter.get("http://london.sonoma.edu/writings/BeforeAdam/chapter3.html");

	printf("^^^^^^^^^^^^^ GET ^^^^^^^^^^^^^^^^^^\n");
	getter.get("http://london.sonoma.edu/writings/BeforeAdam/chapter1.html");

	printf("^^^^^^^^^^^^^ GET ^^^^^^^^^^^^^^^^^^\n");
	getter.get("http://london.sonoma.edu/writings/BeforeAdam/chapter2.html");
//	getter.get("http://chehov.niv.ru/chehov/text/arhierej.htm");
	//getter.get("http://ilibrary.ru/text/29/p.1/index.html");

	//getter.get("http://www.really-learn-english.com/english-short-stories-for-complete-beginners.html");

	//getter.get("http://www.klassika.ru/read.html?proza/chehov/6.txt&page=10");
	//getter.get("http://lib.ru/LITRA/CHEHOW/d.txt");

//////////////////////////////////////////////////
	/*int err;
	pthread_t tid1, tid2;

	Loader loader("mypipe");

	thr_cntxt cnxt;
	cnxt.loader = &loader;
	err = pthread_create(&tid1, NULL, loader_fun, &cnxt);
	if (err) {
		fprintf(stderr, "Could not create thread\n");
		return 1;
	}

	err = pthread_create(&tid2, NULL, reader_fun, NULL);
	if (err) {
		fprintf(stderr, "Could not create thread 2\n");
		return 1;
	}


	void *thr_ret;
	err = pthread_join(tid1, &thr_ret);
	if (err) {
		fprintf(stderr, "Could not join thread\n");
	}

	err = pthread_join(tid2, &thr_ret);
	if (err) {
		fprintf(stderr, "Could not join thread 2\n");
	}*/
//////////////////////////////////////////////////

	/*Loader loader("mypipe");
	loader.load("http://habrahabr.ru/post/155201/");

	int fd;
	char buf[255];
	fd = open("mypipe", O_RDONLY);
	if (fd == -1) {
		fprintf(stderr, "Could not open pipe\n");
		return 1;
	}

	ssize_t bytes;
	do {
		printf(">>>> pre read\n");
		bytes = read(fd, &buf, sizeof(buf)-1);
		printf(">>> get %d bytes\n", bytes);
		if (bytes == -1) {
			fprintf(stderr, "Could not read from pipe\n");
			close(fd);
			return 1;
		}
		buf[bytes] = 0;
		//printf((const char *)buf);

	} while (bytes);

	close(fd);*/
	return 0;
}
