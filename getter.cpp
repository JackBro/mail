#include "getter.h"
#include "loader.h"

#include <pthread.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>


volatile int g_load_done = 0;
pthread_mutex_t g_load_mutex = PTHREAD_MUTEX_INITIALIZER;
//pthread_cond_t g_load_cond = PTHREAD_COND_INITIALIZER;

Getter::Getter(const char *pipe_name, const char *url) : m_pipe_name(pipe_name), m_url(url) {}

struct loader_cnxt
{
	Loader *loader;
	const char *url;
};

struct reader_cnxt
{
	const char *pipe_name;
};

void thr_exit()
{
	pthread_mutex_lock(&g_load_mutex);
	printf("g_load_done = 1\n");
	g_load_done = 1;
	//pthread_cond_signal(&g_load_cond);
	pthread_mutex_unlock(&g_load_mutex);

}

void *loader_fun(void *arg)
{
	loader_cnxt *cnxt = (loader_cnxt *)arg;
	cnxt->loader->load(cnxt->url);
	// signal that loading is over
	thr_exit();	
}

bool is_thr_done()
{
	bool res;
	pthread_mutex_lock(&g_load_mutex);
	//while (g_load_done == 0)
	//	pthread_cond_wait(&g_load_cond, &g_load_mutex);
	res = g_load_done;
	printf("is_thr_done: %d\n", res);
	pthread_mutex_unlock(&g_load_mutex);
	return res;
}

void *reader_fun(void *arg)
{
	reader_cnxt *cnxt = (reader_cnxt *)arg;

	int fd;
	char buf[255];

	fd = open(cnxt->pipe_name, O_RDONLY);
	if (fd == -1) {
		fprintf(stderr, "Could not open pipe %s error: %s\n", cnxt->pipe_name, strerror(errno));
		close(fd);
		return (void *)1;
	}

	int flags = fcntl(fd, F_GETFL, 0);
	fcntl(fd, F_SETFL, flags | O_NONBLOCK);

	ssize_t bytes;
	while (!is_thr_done() || (bytes == 0)) {
		printf("~~~~ pre read\n");
		bytes = read(fd, &buf, sizeof(buf)-1);
		printf("~~~~ get %d bytes\n", bytes);
		if (bytes == -1) {
			if (errno == EAGAIN) {
				sleep(1);
				continue;
			}
			fprintf(stderr, "Could not read from pipe %s error: %s\n", cnxt->pipe_name, strerror(errno));
			close(fd);
			return (void *)2;		
		}
		buf[bytes] = 0;
		printf("~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
		printf(buf);
		printf("~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
	}
}		

void Getter::get()
{
	int err;
	pthread_t tid1, tid2;

	Loader loader(m_pipe_name);

	loader_cnxt lcnxt;
	lcnxt.loader = &loader;
	lcnxt.url = m_url;

	err = pthread_create(&tid1, NULL, loader_fun, &lcnxt);
	if (err) {
		fprintf(stderr, "Could not create thread - error: %d\n", err);
		return;
	}

	reader_cnxt rcnxt;
	rcnxt.pipe_name = m_pipe_name;

	err = pthread_create(&tid2, NULL, reader_fun, &rcnxt);
	if (err) {
		fprintf(stderr, "Could not create thread - error: %d\n", err);
		return;
	}

	void *thr_ret;
	err = pthread_join(tid1, &thr_ret);
	if (err) {
		fprintf(stderr, "Could not join thread - error: %d\n", err);
	}

	err = pthread_join(tid2, &thr_ret);
	if (err) {
		fprintf(stderr, "Could not join thread - error: %d\n", err);
	}
	
	return;
}
