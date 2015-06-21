#include <errno.h>
#include <fcntl.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/types.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <unistd.h>
#include <utils/System.h>

SystemErrorException::SystemErrorException () throw ()
	: _number (errno)
{
}

SystemErrorException::SystemErrorException (int err) throw ()
	: _number (err)
{
}

pid_t System::spawn (const char *file, char* const argv[])
{
	// Se utiliza un pipe para determinar, en el padre, si exec fallo o no
	// en el hijo.
	// ver: http://davmac.wordpress.com/2008/11/25/forkexec-is-forked-up/
	int fds[2];
	pid_t pid;

	if (pipe (fds) == -1) {
		return -1;
	}

	pid = fork ();
	if (pid == 0) {
		close (fds[0]);
		// Se establece el flag FD_CLOEXEC para que si exec se ejecuta
		// exitosamente el descriptor se cierre.
		// Esto provoca que en el padre read devuelva 0-
		fcntl (fds[1], F_SETFD, fcntl (fds[1], F_GETFD) | FD_CLOEXEC);
		int err = execvp (file, argv);
		(void) err;
		// Si llega hasta acá es que hubo error en exec
		// Primero logeamos el error 
		SystemErrorException e;

		// Notificamos al padre el código de error a traves
		// del pipe.
		int reason = e.number ();
		ssize_t written = write (fds[1], &reason, sizeof (reason));
		(void) written;
		close (fds[1]);

		// Finalmente salimos del proceso hijo
		_exit (1);
	} else if (pid == -1) {
		int forkError = errno;
		close (fds[0]);
		close (fds[1]);
		errno = forkError;
		return -1;
	} else {
		close (fds[1]);
		int childErr = 0;
		// El padre se bloquea hasta que el hijo haga exec
		ssize_t err = read (fds[0], &childErr, sizeof (childErr));
		close (fds[0]);
		// Si err es 0 el exec del hijo fue exitoso. Sino el valor
		// de errno que nos comunicó el hijo queda en la variable
		// childErr.
		if (err > 0) {
			errno = childErr;
			return -1;
		}
	}
	return pid;
}

pid_t System::spawn (const char* file, std::vector<const char*>& args)
{
	return spawn (file, (char * const *) &args[0]);
}

void System::semrm (const std::string& archivo, char letra)
{
	key_t token = ftok (archivo.c_str (), letra);
	System::check (token);

	int id = semget (token, 0, 0);
	System::check (id);

	int err = semctl (id, 0, IPC_RMID);
	System::check (err);
}

void System::shmrm (const std::string& archivo, char letra)
{
	key_t token = ftok (archivo.c_str (), letra);
	System::check (token);

	int id = shmget (token, 0, 0);
	System::check (id);

	int err = shmctl (id, IPC_RMID, NULL);
	System::check (err);
}

void System::mqrm (const std::string& archivo, char letra)
{
	key_t token = ftok (archivo.c_str (), letra);
	System::check (token);

	int id = msgget (token, 0);
	System::check (id);

	int err = msgctl (id, IPC_RMID, NULL);
	System::check (err);
}
