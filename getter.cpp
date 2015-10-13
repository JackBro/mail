#include "getter.h"
//#include "loader.h"

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

Getter::Getter(const char *pipe_name, unsigned chain_rate) : m_pipe_name(pipe_name), m_fd(0), m_loader(pipe_name), m_trainer(chain_rate)
{
//	int fd;

	printf("%s\n", __func__);

	m_fd = open(m_pipe_name, O_RDONLY);
	if (m_fd == -1) {
		fprintf(stderr, "Could not open pipe %s error: %s\n", m_pipe_name, strerror(errno));
		close(m_fd);
		//return (void *)1;
	}

}

Getter::~Getter()
{
	printf("%s\n", __func__);
	if (m_fd)
		close(m_fd);
}

struct loader_cnxt
{
	Loader *loader;
	const char *url;
};

struct reader_cnxt
{
	const char *pipe_name;
	int fd;
	Trainer *trainer;
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
	printf("******* loader_fun *******\n");
	loader_cnxt *cnxt = (loader_cnxt *)arg;
	cnxt->loader->load(cnxt->url);
//	Loader loader("mypipe");
//	loader.load(cnxt->url);
	// signal that loading is over
	thr_exit();
	printf("loader_fun: done\n");
}

bool is_thr_done()
{
	bool res;
	pthread_mutex_lock(&g_load_mutex);
	//while (g_load_done == 0)
	//	pthread_cond_wait(&g_load_cond, &g_load_mutex);
	res = g_load_done;
	//printf("is_thr_done: %d\n", res);
	pthread_mutex_unlock(&g_load_mutex);
	return res;
}

void *reader_fun(void *arg)
{
	reader_cnxt *cnxt = (reader_cnxt *)arg;

	printf("%s\n", __func__);

	if (!cnxt->fd)
		return (void *)3;

	printf("2\n");

	//int fd;
	char buf[255];

/*	fd = open(cnxt->pipe_name, O_RDONLY);
	if (fd == -1) {
		fprintf(stderr, "Could not open pipe %s error: %s\n", cnxt->pipe_name, strerror(errno));
		close(fd);
		return (void *)1;
	}*/

	int flags = fcntl(cnxt->fd, F_GETFL, 0);
	fcntl(cnxt->fd, F_SETFL, flags | O_NONBLOCK);

	ssize_t bytes = 0;
	while (!is_thr_done() || (bytes == 0)) {
	//	printf("~~~~ pre read\n");
		bytes = read(cnxt->fd, &buf, sizeof(buf)-1);
	//	printf("~~~~ get %d bytes\n", bytes);
		if (bytes == -1) {
			if (errno == EAGAIN) {
				//sleep(1);
				continue;
			}
			fprintf(stderr, "Could not read from pipe %s error: %s\n", cnxt->pipe_name, strerror(errno));
			//close(cnxt->fd);
			return (void *)2;		
		}
		buf[bytes] = 0;
		printf("~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
		printf(buf);
		printf("~~~~~~~~~~~~~~~~~~~~~~~~~~\n");

		cnxt->trainer->add_chunk(buf, bytes);
	}

	printf("reader_fun: done\n");

	//close(fd);
}		

void Getter::get(const char *url)
{
	int err;
	pthread_t tid1, tid2;

	printf("Getter::get\n");

	pthread_mutex_lock(&g_load_mutex);
	g_load_done = 0;
	pthread_mutex_unlock(&g_load_mutex);

	//Loader loader(m_pipe_name);

	loader_cnxt lcnxt;
	lcnxt.loader = &m_loader;
	lcnxt.url = url;

	err = pthread_create(&tid1, NULL, loader_fun, &lcnxt);
	if (err) {
		fprintf(stderr, "Could not create thread - error: %d\n", err);
		return;
	}

	reader_cnxt rcnxt;
	rcnxt.pipe_name = m_pipe_name;
	rcnxt.fd = m_fd;
	rcnxt.trainer = &m_trainer;

	err = pthread_create(&tid2, NULL, reader_fun, &rcnxt);
	if (err) {
		fprintf(stderr, "Could not create thread - error: %d\n", err);
		return;
	}

	printf("waiting for threads\n");

	void *thr_ret;
	err = pthread_join(tid1, &thr_ret);
	if (err) {
		fprintf(stderr, "Could not join thread - error: %d\n", err);
	}
	printf("tid1 join\n");

	err = pthread_join(tid2, &thr_ret);
	if (err) {
		fprintf(stderr, "Could not join thread - error: %d\n", err);
	}
	printf("tid2 join\n");
	
	return;
}
